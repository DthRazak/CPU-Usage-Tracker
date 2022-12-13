#ifndef CPUUT_PRINTER_H
#define CPUUT_PRINTER_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdatomic.h>

#include "core/types.h"
#include "core/ring_buffer.h"

typedef struct Printer Printer;
struct Printer {
    RingBuffer *cpu_usage_buffer;
    _Atomic(clock_t) last_update;
};

extern Printer printer;

void printer_init(int core_num);
int printer_start(void *args);
void printer_destroy(void);

#endif // CPUUT_PRINTER_H