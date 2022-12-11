#include <stdio.h>
#include <string.h>

#include "core/reader.h"

#define CORE_PARAMS_NUM 11


int scan_line(const char* buffer, cpu_time* core_time);
int get_cpu_core_num(void);

Reader reader;

int reader_init(void){
    int core_num = get_cpu_core_num();
    if (core_num < 1){
        perror("Invalid number of cores");
        return -1;
    }

    cpu_stat cpu_data = {
        .core_num = core_num,
        .time_data = malloc(sizeof(cpu_time) * (core_num+1))
    };
    reader.cpu_data = cpu_data;

    return 0;
}

void reader_destroy(void){
    free(reader.cpu_data.time_data);
}

int scan_line(const char* buffer, cpu_time* core_time){
    char cpu_stat_name[8];

    int ret = sscanf(buffer,
        "%s %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",
        cpu_stat_name, 
        &core_time->user, &core_time->nice, &core_time->system, &core_time->idle, 
        &core_time->iowait, &core_time->irq, &core_time->softirq, &core_time->steal, 
        &core_time->guest, &core_time->guestnice);

    return ret;
}

int get_cpu_core_num(void){
    FILE* file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Could not open stat file");
        return -1;
    }

    int cpu_data_cnt = 0;   // Number of `cpu` lines
    char buffer[1024];      // Buffer to store one line from `/proc/stat` file 

    // Read file while the stat name begins with `cpu`
    while (fgets(buffer, sizeof(buffer)-1, file) && !strncmp(buffer, "cpu", 3)){
        cpu_time core_time;
        int params_num = scan_line(buffer, &core_time);

        if (params_num != CORE_PARAMS_NUM){
            perror("Could not read stat file");
            cpu_data_cnt = 0;
            break;
        }

        cpu_data_cnt++;
    }

    fclose(file);

    // Number of cores equal to number of scanned lines without first one
    return cpu_data_cnt-1;
}
