#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <threads.h>
#include <string.h>
#include <stdbool.h>

#include "core/reader.h"
#include "core/analyzer.h"
#include "core/ring_buffer.h"
#include "termination_handler.h"
#include "printer.h"
#include "watchdog.h"
#include "logger.h"


int main(int argc, char* argv[argc+1]){
    // Setting the termination handler
    struct sigaction action;
    sigset_t block_mask;

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGQUIT);
    sigaddset(&block_mask, SIGTERM);
    
    action.sa_handler = termination_handler;
    action.sa_mask = block_mask;
    action.sa_flags = 0;
    
    sigaction(SIGINT,  &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);

    // Initializing modules
    int core_num = reader_init();
    if (core_num < 1){
        return EXIT_FAILURE;
    }
    analyzer_init(core_num);
    printer_init(core_num);
    
    if (argc == 2 && !strcmp(argv[1], "DEBUG")){
        logger_init(DEBUG);
    }else{
        logger_init(ERROR);
    }

    // Starting modules' jobs in separate threads
    thrd_t thrd[5];
    thrd_create(&thrd[0], reader_start,   0);
    thrd_create(&thrd[1], analyzer_start, 0);
    thrd_create(&thrd[2], printer_start,  0);
    thrd_create(&thrd[3], watchdog_start, 0);
    thrd_create(&thrd[4], logger_start,   "cpu-ut.log");

    // Joining all threads
    thrd_join(thrd[0], 0);
    thrd_join(thrd[1], 0);
    thrd_join(thrd[2], 0);
    thrd_join(thrd[3], 0);
    thrd_join(thrd[4], 0);

    // Destroing modules
    analyzer_destroy();
    reader_destroy();
    printer_destroy();
    logger_destroy();

    return EXIT_SUCCESS;
}