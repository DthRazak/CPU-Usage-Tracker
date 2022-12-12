#include "core/analyzer.h"

#define CPU_USAGE_BUFFER_SIZE 10


Analyzer analyzer;

void analyzer_init(int core_num){
    // RingBuffer initialization for global `analyzer` object
    BufferItemParams params = {
        .type = CPU_USAGE_T,
        .core_num = core_num
    };
    analyzer.cpu_usage_buffer = RingBuffer_new(params, CPU_USAGE_BUFFER_SIZE);
}

void analyzer_destroy(void){
    RingBuffer_delete(analyzer.cpu_usage_buffer);
}

void analyzer_start(void){
    cpu_stat *cpu_data_prev = cpu_stat_new(0);
    cpu_stat *cpu_data_now  = cpu_stat_new(0);

    // TODO: Read `cpu_data_prev` from RingBuffer

    cpu_usage *usage_data = cpu_usage_new(cpu_data_prev->core_num);

    while (1) {
        // TODO: Read `cpu_data_now` from RingBuffer

        for (size_t i = 0; i < usage_data->core_num + 1; ++i){
            int cpu_pct = calc_usage(&cpu_data_prev->time_data[i], 
                                     &cpu_data_now->time_data[i]);
            
            usage_data->usage_data[i] = cpu_pct;
        }

        // TODO: Send `usage_data` to printer
    }
    
    cpu_usage_delete(usage_data);
    cpu_stat_delete(cpu_data_prev);
    cpu_stat_delete(cpu_data_now);
}

uint32_t calc_usage(cpu_time *time_prev, cpu_time *time_now){
    uint32_t idle_prev = time_prev->idle + time_prev->iowait;
    uint32_t idle_now  = time_now->idle  + time_now->iowait;

    uint32_t non_idle_prev = time_prev->user    + time_prev->nice   +
                             time_prev->system  + time_prev->irq    +
                             time_prev->softirq + time_prev->steal;
    uint32_t non_idle_now  = time_now->user     + time_now->nice    +
                             time_now->system   + time_now->irq     +
                             time_now->softirq  + time_now->steal;
                            
    uint32_t total_d = (idle_now + non_idle_now) - (idle_prev + non_idle_prev);
    uint32_t idle_d  =  idle_now - idle_prev;

    return (total_d - idle_d) / total_d;
}
