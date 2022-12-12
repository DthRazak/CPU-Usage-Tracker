#include <assert.h>
#include <stdio.h>
#include "core/ring_buffer.h"


struct RingBuffer {
    size_t size;
    size_t head;
    size_t tail;
    size_t count;

    BufferItemType type;
    void* data;

    // condition_variable nonempty, nonfull;
    // mutex
};

RingBuffer* RingBuffer_init(RingBuffer* buffer, BufferItemType type, size_t size){
    if (buffer){
        if (size){
            *buffer = (RingBuffer) {
                .size = size,
                .head = 0,
                .tail = 0,
                .count = 0,
                .type = type,
            };
            if (type == CPU_STAT_T){
                cpu_stat *init_val = cpu_stat_new(0);
                
                buffer->data = malloc(sizeof(cpu_stat[size]));
                for (size_t i = 0; i < size; ++i){
                    cpu_stat *ptr = (cpu_stat*)(buffer->data);
                    cpu_stat_copy(ptr+i, init_val);
                }

                cpu_stat_delete(init_val);
            } else {
                cpu_usage *init_val = cpu_usage_new(0);

                buffer->data = malloc(sizeof(cpu_usage) * size);
                for (size_t i = 0; i < size; ++i){
                    cpu_usage *ptr = (cpu_usage*)(buffer->data);
                    cpu_usage_copy(ptr+i, init_val);
                }

                cpu_usage_delete(init_val);
            }
        } else{
            *buffer = (RingBuffer) { 0 };
        }
    }
    return buffer;
}

RingBuffer* RingBuffer_new(BufferItemType type, size_t size){
    return RingBuffer_init(malloc(sizeof(RingBuffer)), type, size);
}

void RingBuffer_destroy(RingBuffer* buffer){
    if (buffer) {
        if (buffer->type == CPU_STAT_T){
            for (int i = 0; i < buffer->size; ++i){
                cpu_stat *ptr = (cpu_stat*)(buffer->data);
                free(ptr[i].time_data);
            }
        } else {
            for (size_t i = 0; i < buffer->size; ++i){
                cpu_usage *ptr = (cpu_usage*)(buffer->data);
                free(ptr[i].usage_data);
            }
        }
        free(buffer->data);
        RingBuffer_init(buffer, buffer->type, 0);
    }
    free(buffer);
}
