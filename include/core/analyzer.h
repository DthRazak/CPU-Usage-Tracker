#ifndef CPUUT_ANALYZER_H
#define CPUUT_ANALYZER_H

#include <stdint.h>
#include <stdlib.h>

#include "core/types.h"
#include "core/ring_buffer.h"

typedef struct Analyzer Analyzer;
struct Analyzer {
    RingBuffer *cpu_usage_buffer;
    _Atomic(clock_t) last_update;
};

extern Analyzer analyzer;

void analyzer_init(int core_num);
int analyzer_start(void *args);
void analyzer_destroy(void);

uint32_t calc_usage(cpu_time *time_prev, cpu_time *time_now);

#endif // CPUUT_ANALYZER_H