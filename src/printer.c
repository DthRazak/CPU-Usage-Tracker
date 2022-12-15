#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "core/analyzer.h"
#include "termination_handler.h"
#include "printer.h"


#define PRINTER_WAIT_TIME_MCS   200000
#define PRINTER_TABLE_BREAK_IT  10

Printer printer;
extern Analyzer analyzer;

static cpu_usage usage_data;

void printer_init(int core_num){
    cpu_usage_init(&usage_data, core_num);
}

void printer_destroy(void){
    cpu_usage_destroy(&usage_data);
}

extern volatile sig_atomic_t is_active;

void print_header(size_t core_num){
    printf("\n\n| CPU Total |");
    for (size_t i = 0; i < core_num; ++i){
        printf(" CPU%-5d|", i);
    }
}

int printer_start(void *args){
    double *usage_pct = malloc(sizeof(double[usage_data.core_num + 1]));
    for (size_t i = 0; i < usage_data.core_num+1; ++i){
        usage_pct[i] = 0.0;
    }

    size_t data_per_sec = 0;

    time_t cur_sec = time(0);
    size_t print_cnt = 0;
    printer.last_update = time(0);
    while (is_active) {
        while (is_active && (cur_sec == time(0) || data_per_sec == 0)){
            LOG(DEBUG, printer.last_update, "Reading data by `Printer`");
            RingBuffer_read(analyzer.cpu_usage_buffer, &usage_data);
            for (size_t i = 0; i < usage_data.core_num+1; ++i){
                usage_pct[i] += usage_data.usage_data[i];
            }
            data_per_sec++;
        }

        if (!print_cnt++){
            print_header(usage_data.core_num);
        }
        print_cnt %= PRINTER_TABLE_BREAK_IT;

        printf("\n| %8.2f%% |", usage_pct[0]/data_per_sec);
        for (size_t i = 0; i < usage_data.core_num; ++i){
            printf(" %6.2f%% |", usage_pct[i+1]/data_per_sec);
        }
        printer.last_update = time(0);
        LOG(DEBUG, printer.last_update, "Displaying data by `Printer`");

        memcpy(usage_pct, usage_data.usage_data, sizeof(double[usage_data.core_num+1]));

        cur_sec = time(0);
        data_per_sec = 1;
    }

    free(usage_pct);
    
    return EXIT_SUCCESS;
}