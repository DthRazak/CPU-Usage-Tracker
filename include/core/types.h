#ifndef CPUUT_TYPES_H
#define CPUUT_TYPES_H

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

void cpu_stat_init(cpu_stat *cpu_data, size_t cores);
cpu_stat* cpu_stat_new(size_t cores);
void cpu_stat_destroy(cpu_stat *cpu_data);
void cpu_stat_delete(cpu_stat *cpu_data);
void cpu_stat_copy(cpu_stat *dest, cpu_stat *src);

typedef struct cpu_usage cpu_usage;
struct cpu_usage {
    size_t core_num;
    uint32_t* usage_data;
};

void cpu_usage_init(cpu_usage *usage_data, size_t cores);
cpu_usage* cpu_usage_new(size_t cores);
void cpu_usage_destroy(cpu_usage* usage_data);
void cpu_usage_delete(cpu_usage* usage_data);
void cpu_usage_copy(cpu_usage *dest, cpu_usage *src);

#define CPU_STAT_INIT                   \
{                                       \
    .core_num = 0,                      \
    .time_data = (void *)0              \
}

#define CPU_USAGE_INIT                  \
{                                       \
    .core_num = 0,                      \
    .usage_data = (void *)0             \
}

#endif // CPUUT_TYPES_H