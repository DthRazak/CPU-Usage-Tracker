#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "watchdog.h"
#include "core/reader.h"
#include "core/analyzer.h"
#include "printer.h"
#include "termination_handler.h"
#include "logger.h"

#define STOP_DELAY_SEC  2
#define START_DELAY_SEC 2
#define WAIT_TIME_MCS   500000


extern Reader reader;
extern Analyzer analyzer;
extern Printer printer;

extern volatile sig_atomic_t is_active;

int watchdog_start(void *args){
    sleep(START_DELAY_SEC);

    time_t t_now;

    int it = 0;
    while (is_active) {
        t_now = time(0);

        if ((uintmax_t)t_now-reader.last_update   > STOP_DELAY_SEC) {
            LOG(ERROR, time(0), "Reader update time is too long, killing process");
            kill(getpid(), SIGINT);
        } 
        if ((uintmax_t)t_now-analyzer.last_update > STOP_DELAY_SEC) {
            LOG(ERROR, time(0), "Analyzer update time is too long, killing process");
            kill(getpid(), SIGINT);
        }
        if ((uintmax_t)t_now-printer.last_update  > STOP_DELAY_SEC){
            LOG(ERROR, time(0), "Printer update time is too long, killing process");
            kill(getpid(), SIGINT);
        }

        LOG(DEBUG, t_now, "Watchdog (Threads status is OK)");

        usleep(WAIT_TIME_MCS);
    }

    return EXIT_SUCCESS;   
}