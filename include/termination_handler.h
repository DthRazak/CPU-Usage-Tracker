#ifndef CPUUT_SIGNAL_HANDLER_H
#define CPUUT_SIGNAL_HANDLER_H

#include <stdbool.h>
#include <signal.h>

extern volatile sig_atomic_t is_active;

void termination_handler(int singum);

#endif // CPUUT_SIGNAL_HANDLER_H