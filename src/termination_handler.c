#include <stdio.h>

#include "termination_handler.h"
#include "logger.h"


volatile sig_atomic_t is_active = true;

void termination_handler(int singum){
    is_active = false;

    LOG(DEBUG, time(0), "Termination signal handled");
}