#ifndef CPUUT_READER_H
#define CPUUT_READER_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdatomic.h>

#include "core/types.h"
#include "core/ring_buffer.h"

typedef struct Reader Reader;
struct Reader {
    RingBuffer *cpu_stat_buffer;
    _Atomic(clock_t) last_update;
};

extern Reader reader;

int reader_init(void);
void reader_start(void);
void reader_destroy(void);

#endif // CPUUT_READER_H