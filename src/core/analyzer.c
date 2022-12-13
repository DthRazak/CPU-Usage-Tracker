#include <unistd.h>
#include <stdio.h>

#include "core/reader.h"
#include "core/analyzer.h"
#include "termination_handler.h"

#define CPU_USAGE_BUFFER_SIZE   10
#define ANALYZER_WAIT_TIME_MCS  250000


Analyzer analyzer;
extern Reader reader;

static BufferItemParams params;

void analyzer_init(int core_num){
    // RingBuffer initialization for global `analyzer` object
    params.type = CPU_USAGE_T;
    params.core_num = core_num;

    analyzer.cpu_usage_buffer = RingBuffer_new(params, CPU_USAGE_BUFFER_SIZE);
}

void analyzer_destroy(void){
    RingBuffer_delete(analyzer.cpu_usage_buffer);
}

extern volatile sig_atomic_t is_active;

int analyzer_start(void *args){
    cpu_stat *cpu_data_prev = cpu_stat_new(params.core_num);
    cpu_stat *cpu_data_now  = cpu_stat_new(params.core_num);

    cpu_usage *usage_data = cpu_usage_new(params.core_num);

    RingBuffer_read(reader.cpu_stat_buffer, cpu_data_prev);

    while (is_active) {
        RingBuffer_read(reader.cpu_stat_buffer, cpu_data_now);

        for (size_t i = 0; i < usage_data->core_num + 1; ++i){
            double cpu_pct = calc_usage(&cpu_data_prev->time_data[i], 
                                        &cpu_data_now->time_data[i]);
            
            usage_data->usage_data[i] = cpu_pct;
        }

        printf("Analyzer: Total CPU  Usage = %.2f %%\n", usage_data->usage_data[0]);
        printf("Analyzer: Total CPU0 Usage = %.2f %%\n", usage_data->usage_data[1]);
        printf("Analyzer: Total CPU1 Usage = %.2f %%\n", usage_data->usage_data[2]);
        printf("Analyzer: Total CPU2 Usage = %.2f %%\n", usage_data->usage_data[3]);
        printf("Analyzer: Total CPU3 Usage = %.2f %%\n\n", usage_data->usage_data[4]);
        
        // TODO: Send `usage_data` to printer

        cpu_stat_copy(cpu_data_prev, cpu_data_now);

        usleep(ANALYZER_WAIT_TIME_MCS);
    }
    
    cpu_usage_delete(usage_data);
    cpu_stat_delete(cpu_data_prev);
    cpu_stat_delete(cpu_data_now);
}

double calc_usage(cpu_time *time_prev, cpu_time *time_now){
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

    if (total_d){
        return (total_d - idle_d) / (double)total_d * 100;
    }else{
        return 0.0;
    }
}
