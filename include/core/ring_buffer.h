#ifndef CPUUT_RINGBUFFER_H
#define CPUUT_RINGBUFFER_H

#include <stdint.h>
#include <stdlib.h>

#include "core/types.h"

typedef struct RingBuffer RingBuffer;

typedef enum BufferItemType BufferItemType;
enum BufferItemType {
    CPU_STAT_T = 0, 
    CPU_USAGE_T
};

typedef struct BufferItemParams BufferItemParams;
struct BufferItemParams {
    BufferItemType type;
    size_t core_num;
};

RingBuffer* RingBuffer_new(BufferItemParams params, size_t size);
void RingBuffer_destroy(RingBuffer* buffer);

#endif // CPUUT_RINGBUFFER_H