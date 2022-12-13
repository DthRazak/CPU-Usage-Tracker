#include <string.h>

#include "core/types.h"


void cpu_stat_init(cpu_stat* cpu_data, size_t cores){
    if (cpu_data){
        if (cores){
            *cpu_data = (cpu_stat) {
                .core_num = cores,
                .time_data = malloc(sizeof(cpu_time[cores+1]))
            };
        } else{
            *cpu_data = (cpu_stat) CPU_STAT_INIT;
        }
    }
}

cpu_stat* cpu_stat_new(size_t cores){
    cpu_stat* cpu_stat_obj = malloc(sizeof(cpu_stat));
    cpu_stat_init(cpu_stat_obj, cores);

    return cpu_stat_obj;
}

void cpu_stat_destroy(cpu_stat* cpu_data){
    if (cpu_data) {
        free(cpu_data->time_data);
        cpu_stat_init(cpu_data, 0);
    }
}

void cpu_stat_delete(cpu_stat* cpu_data){
    cpu_stat_destroy(cpu_data);
    free(cpu_data);
}

void cpu_stat_copy(cpu_stat *dest, cpu_stat *src){
    if (src->core_num){
        dest->core_num = src->core_num;
        memcpy(dest->time_data, src->time_data, sizeof(cpu_time[src->core_num+1]));
    }else{
        cpu_stat_init(dest, 0);
    }
}

void cpu_usage_init(cpu_usage* usage_data, size_t cores){
    if (usage_data){
        if (cores){
            *usage_data = (cpu_usage) {
                .core_num = cores,
                .usage_data = malloc(sizeof(double[cores+1]))
            };
        } else{
            *usage_data = (cpu_usage) CPU_USAGE_INIT;
        }
    }
}

cpu_usage* cpu_usage_new(size_t cores){
    cpu_usage* cpu_usage_obj = malloc(sizeof(cpu_usage));
    cpu_usage_init(cpu_usage_obj, cores);

    return cpu_usage_obj;
}

void cpu_usage_destroy(cpu_usage* usage_data){
    if (usage_data) {
        free(usage_data->usage_data);
        cpu_usage_init(usage_data, 0);
    }
}

void cpu_usage_delete(cpu_usage* usage_data){
    cpu_usage_destroy(usage_data);
    free(usage_data);
}

void cpu_usage_copy(cpu_usage *dest, cpu_usage *src){
    if (src->core_num){
        dest->core_num = src->core_num;
        memcpy(dest->usage_data, src->usage_data, sizeof(double[src->core_num+1]));
    }else{
        cpu_usage_init(dest, 0);
    }
}
