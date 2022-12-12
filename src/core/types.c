#include "core/types.h"


cpu_stat* cpu_stat_init(cpu_stat* cpu_data, size_t cores){
    if (cpu_data){
        if (cores){
            *cpu_data = (cpu_stat) {
                .core_num = cores,
                .time_data = malloc(sizeof(cpu_time) * (cores+1))
            };
        } else{
            *cpu_data = (cpu_stat) CPU_STAT_INIT;
        }
    }
    return cpu_data;
}

cpu_stat* cpu_stat_new(size_t cores){
    return cpu_stat_init(malloc(sizeof(cpu_stat)), cores);
}

void cpu_stat_destroy(cpu_stat* cpu_data){
    if (cpu_data) {
        free(cpu_data->time_data);
        cpu_stat_init(cpu_data, 0);
    }
    free(cpu_data);
}

cpu_usage* cpu_usage_init(cpu_usage* usage_data, size_t cores){
    if (usage_data){
        if (cores){
            *usage_data = (cpu_usage) {
                .core_num = cores,
                .usage_data = malloc(sizeof(uint32_t) * (cores+1))
            };
        } else{
            *usage_data = (cpu_usage) CPU_USAGE_INIT;
        }
    }
    return usage_data;
}

cpu_stat* cpu_usage_new(size_t cores){
    return cpu_stat_init(malloc(sizeof(cpu_usage)), cores);
}

void cpu_usage_delete(cpu_stat* usage_data){
    if (usage_data) {
        free(usage_data->time_data);
        cpu_stat_init(usage_data, 0);
    }
    free(usage_data);
}
