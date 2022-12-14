/**
 ** @file watchdog.h
 ** @brief Watchdog declaration
 */

#ifndef CPUUT_WATCHDOG_H
#define CPUUT_WATCHDOG_H


/** @brief Thead funtion that checks if no errors occured in
 ** @c Reader, @c Analyzer and @c Printer
 */
int watchdog_start(void *args);

#endif // CPUUT_WATCHDOG_H