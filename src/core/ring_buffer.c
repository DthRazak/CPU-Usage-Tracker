#include <assert.h>
#include <stdio.h>
#include <threads.h>

#include "core/ring_buffer.h"


struct RingBuffer {
    size_t size;
    size_t head;
    size_t tail;
    size_t count;

    BufferItemType type;
    void* data;

    cnd_t non_empty;
    cnd_t non_full;
    mtx_t mtx;
};

RingBuffer* RingBuffer_init(RingBuffer* buffer, BufferItemParams params, size_t size){
    if (buffer){
        if (size){
            *buffer = (RingBuffer) {
                .size = size,
                .head = 0,
                .tail = 0,
                .count = 0,
                .type = params.type,
            };
            if (params.type == CPU_STAT_T){
                buffer->data = malloc(sizeof(cpu_stat[size]));
                for (size_t i = 0; i < size; ++i){
                    cpu_stat *ptr = (cpu_stat*)(buffer->data);
                    cpu_stat_init(&ptr[i], params.core_num);
                }
            } else {
                buffer->data = malloc(sizeof(cpu_usage[size]));
                for (size_t i = 0; i < size; ++i){
                    cpu_usage *ptr = (cpu_usage*)(buffer->data);
                    cpu_usage_init(&ptr[i], params.core_num);
                }
            }

            mtx_init(&buffer->mtx, mtx_plain);
            cnd_init(&buffer->non_empty);
            cnd_init(&buffer->non_full);
        } else{
            *buffer = (RingBuffer) { 0 };
        }
    }
    return buffer;
}

RingBuffer* RingBuffer_new(BufferItemParams params, size_t size){
    return RingBuffer_init(malloc(sizeof(RingBuffer)), params, size);
}

void RingBuffer_delete(RingBuffer* buffer){
    if (buffer) {
        if (buffer->type == CPU_STAT_T){
            for (int i = 0; i < buffer->size; ++i){
                cpu_stat *ptr = (cpu_stat*)(buffer->data);
                cpu_stat_destroy(&ptr[i]);
            }
        } else {
            for (size_t i = 0; i < buffer->size; ++i){
                cpu_usage *ptr = (cpu_usage*)(buffer->data);
                cpu_usage_destroy(&ptr[i]);
            }
        }
        free(buffer->data);
        BufferItemParams default_params = {
            .type = buffer->type,
            .core_num = 0
        };
        RingBuffer_init(buffer, default_params, 0);
    }
    free(buffer);
}

void RingBuffer_write(RingBuffer *buffer, void *item){
    mtx_lock(&buffer->mtx);
    if (buffer->count == buffer->size){
        cnd_wait(&buffer->non_full, &buffer->mtx);
    }
    assert(0 <= buffer->count && buffer->count < buffer->size);

    if (buffer->type == CPU_STAT_T){
        cpu_stat *ptr = (cpu_stat*)(buffer->data) + buffer->tail++;
        cpu_stat_copy(ptr, item);
    } else {
        cpu_usage *ptr = (cpu_usage*)(buffer->data) + buffer->tail++;
        cpu_usage_copy(ptr, item);
    }
    buffer->tail %= buffer->size;
    buffer->count++;

    cnd_signal(&buffer->non_empty);
    mtx_unlock(&buffer->mtx);
}

void RingBuffer_read(RingBuffer *buffer, void *item){
    mtx_lock(&buffer->mtx);
    if (buffer->count == 0){
        cnd_wait(&buffer->non_empty, &buffer->mtx);
    }
    assert(0 < buffer->count && buffer->count <= buffer->size);

    if (buffer->type == CPU_STAT_T){
        cpu_stat *ptr = (cpu_stat*)(buffer->data) + buffer->head++;
        cpu_stat_copy(item, ptr);
    } else {
        cpu_usage *ptr = (cpu_usage*)(buffer->data) + buffer->head++;
        cpu_usage_copy(item, ptr);
    }
    buffer->head %= buffer->size;
    buffer->count--;
    
    cnd_signal(&buffer->non_full);
    mtx_unlock(&buffer->mtx);
}
