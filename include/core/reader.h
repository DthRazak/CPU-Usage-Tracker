#ifndef CPUUT_READER_H
#define CPUUT_READER_H

#include <stdint.h>
#include <stdlib.h>

typedef struct cpu_time cpu_time;
struct cpu_time {
    uint32_t user;
    uint32_t nice;
    uint32_t system;
    uint32_t idle;
    uint32_t iowait;
    uint32_t irq;
    uint32_t softirq;
    uint32_t steal;
    uint32_t guest;
    uint32_t guestnice;
};

typedef struct cpu_stat cpu_stat;
struct cpu_stat {
    size_t core_num;
    cpu_time* time_data;
};

typedef struct Reader Reader;
struct Reader {
    cpu_stat cpu_data;
};

int reader_init(void);
void reader_destroy(void);

#endif // CPUUT_READER_H