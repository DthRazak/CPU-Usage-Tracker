#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <threads.h>

#include "core/reader.h"
#include "core/analyzer.h"
#include "core/ring_buffer.h"
#include "termination_handler.h"


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

    // Starting modules' jobs in separate threads
    thrd_t thrd[2];
    thrd_create(&thrd[0], reader_start,   0);
    thrd_create(&thrd[1], analyzer_start, 0);

    // Joining all threads
    thrd_join(thrd[0], 0);
    thrd_join(thrd[1], 0);

    // Destroing modules
    analyzer_destroy();
    reader_destroy();

    return EXIT_SUCCESS;
}