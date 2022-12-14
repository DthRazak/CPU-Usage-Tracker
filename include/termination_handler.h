/**
 ** @file termination_handler.h
 ** @brief Termination handler declaration
 */

#ifndef CPUUT_SIGNAL_HANDLER_H
#define CPUUT_SIGNAL_HANDLER_H

#include <stdbool.h>
#include <signal.h>


/** @brief Atomic variable the represent threads active status */
extern volatile sig_atomic_t is_active;

/** @brief Set @a is_active to @a false if triggered by a signal */
void termination_handler(int singum);

#endif // CPUUT_SIGNAL_HANDLER_H