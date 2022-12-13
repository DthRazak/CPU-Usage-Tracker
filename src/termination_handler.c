#include <stdio.h>

#include "termination_handler.h"


volatile sig_atomic_t is_active = true;

void termination_handler(int singum){
    is_active = false;
}