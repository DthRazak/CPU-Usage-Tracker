#include <unistd.h>
#include <time.h>
#include <stdio.h>

#include "core/reader.h"
#include "core/analyzer.h"
#include "termination_handler.h"

#define CPU_USAGE_BUFFER_SIZE   10


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

    LOG(DEBUG, analyzer.last_update, "Reading data by `Analyzer`");
    RingBuffer_read(reader.cpu_stat_buffer, cpu_data_prev);

    analyzer.last_update = time(0);
    while (is_active) {
        LOG(DEBUG, analyzer.last_update, "Reading data by `Analyzer`");
        RingBuffer_read(reader.cpu_stat_buffer, cpu_data_now);

        for (size_t i = 0; i < usage_data->core_num + 1; ++i){
            double cpu_pct = calc_usage(&cpu_data_prev->time_data[i], 
                                        &cpu_data_now->time_data[i]);

            usage_data->usage_data[i] = cpu_pct;
        }
        analyzer.last_update = time(0);
        RingBuffer_write(analyzer.cpu_usage_buffer, usage_data);
        LOG(DEBUG, analyzer.last_update, "Sending data by `Analyzer`");

        cpu_stat_copy(cpu_data_prev, cpu_data_now);
    }

    // Write one last time to unlock printer thread
    RingBuffer_write(analyzer.cpu_usage_buffer, usage_data);
    LOG(DEBUG, analyzer.last_update, "Sending data by `Analyzer`");
    
    cpu_usage_delete(usage_data);
    cpu_stat_delete(cpu_data_prev);
    cpu_stat_delete(cpu_data_now);

    return EXIT_SUCCESS;
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
