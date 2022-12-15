#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "core/reader.h"
#include "termination_handler.h"
#include "logger.h"

#define CORE_PARAMS_NUM         11
#define CPU_STAT_BUFFER_SIZE    10
#define READER_WAIT_TIME_MCS    250000

int scan_line(FILE* file, char* cpu_stat_name, cpu_time* core_time);
int read_cpu_data(cpu_stat *cpu_data);
int get_cpu_core_num(void);

Reader reader;
static cpu_stat cpu_data;

int reader_init(void){
    int core_num = get_cpu_core_num();
    if (core_num < 1){
        perror("Invalid number of cores");
        return -1;
    }

    // Initialization of global `cpu_data` object
    cpu_data.core_num  = core_num;
    cpu_data.time_data = malloc(sizeof(cpu_time[core_num+1]));

    // RingBuffer initialization for global `reader` object
    BufferItemParams params = {
        .type = CPU_STAT_T,
        .core_num = core_num
    };
    reader.cpu_stat_buffer = RingBuffer_new(params, CPU_STAT_BUFFER_SIZE);

    return core_num;
}

void reader_destroy(void){
    RingBuffer_delete(reader.cpu_stat_buffer);
    free(cpu_data.time_data);
}

extern volatile sig_atomic_t is_active;

int reader_start(void *args){
    reader.last_update = time(0);
    while (is_active){
        LOG(DEBUG, reader.last_update, "Reading data by `Reader`");
        if (read_cpu_data(&cpu_data) == EXIT_FAILURE){
            is_active = false;
            // Unlocking analyzer thread
            RingBuffer_write(reader.cpu_stat_buffer, &cpu_data);

            LOG(ERROR, 
                reader.last_update, 
                "Error occuread while reading `/stat/proc` file");

            return EXIT_FAILURE;
        }
        reader.last_update = time(0);
        RingBuffer_write(reader.cpu_stat_buffer, &cpu_data);
        LOG(DEBUG, reader.last_update, "Sending data by `Reader`");

        usleep(READER_WAIT_TIME_MCS);
    }

    // Write one last time to unlock analyzer thread
    read_cpu_data(&cpu_data);
    RingBuffer_write(reader.cpu_stat_buffer, &cpu_data);

    return EXIT_SUCCESS;
}

int read_cpu_data(cpu_stat *cpu_data){
    FILE* file = fopen("/proc/stat", "r");
    if (file == NULL) {
        is_active = false;
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < cpu_data->core_num+1; ++i){
        char cpu_stat_name[8];
        int ret = scan_line(file, cpu_stat_name, &cpu_data->time_data[i]);
    }

    fclose(file);

    return EXIT_SUCCESS;
}

int scan_line(FILE* file, char* cpu_stat_name, cpu_time* core_time){
    int ret = fscanf(file, "%s %16u %16u %16u %16u %16u %16u %16u %16u %16u %16u",
                    cpu_stat_name, 
                    &core_time->user, 
                    &core_time->nice, 
                    &core_time->system, 
                    &core_time->idle, 
                    &core_time->iowait, 
                    &core_time->irq, 
                    &core_time->softirq, 
                    &core_time->steal, 
                    &core_time->guest, 
                    &core_time->guestnice);

    return ret;
}

int get_cpu_core_num(void){
    FILE* file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Could not open stat file");
        return -1;
    }

    int cpu_data_cnt = 0;           // Number of `cpu` lines
    char cpu_stat_name[8] = "cpu";

    // Read file while the stat name begins with `cpu`
    while (!strncmp(cpu_stat_name, "cpu", 3)){
        cpu_time core_time;
        int params_num = scan_line(file, cpu_stat_name, &core_time);

        if (params_num != CORE_PARAMS_NUM){
            perror("Could not read stat file");
            cpu_data_cnt = 0;
            break;
        }

        cpu_data_cnt++;
    }

    fclose(file);

    // Number of cores equal to number of scanned lines 
    // without first and last one
    return cpu_data_cnt-2;
}
