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
            if (type = CPU_STAT_T){
                buffer->data = malloc(sizeof(cpu_stat) * size);
                for (size_t i = 0; i < size; ++i){
                    cpu_stat *ptr = (cpu_stat*)(buffer->data) + i;
                    ptr = cpu_stat_new(0);
                }
            } else {
                for (size_t i = 0; i < size; ++i){
                    cpu_usage *ptr = (cpu_usage*)(buffer->data) + i;
                    ptr = cpu_usage_new(0);
                }
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
        if (buffer->type = CPU_STAT_T){
            for (size_t i = 0; i < buffer->size; ++i){
                cpu_stat *ptr = (cpu_stat*)(buffer->data) + i;
                cpu_stat_delete(ptr);
            }
        } else {
            for (size_t i = 0; i < buffer->size; ++i){
                cpu_usage *ptr = (cpu_usage*)(buffer->data) + i;
                cpu_usage_delete(ptr);
            }
        }
        free(buffer->data);
        RingBuffer_init(buffer, buffer->type, 0);
    }
    free(buffer);
}

void RingBugger_write(RingBuffer *buffer, void *item){
    // lock
    // nonfull wait
    assert(0 <= buffer->count && buffer->count < buffer->size);

    if (buffer->type == CPU_STAT_T){
        
    } else {

    }
}
