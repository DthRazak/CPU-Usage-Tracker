/**
 ** @file printer.h
 ** @brief Printer type declaration
 */

#ifndef CPUUT_PRINTER_H
#define CPUUT_PRINTER_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdatomic.h>

#include "core/types.h"
#include "core/ring_buffer.h"


/** @brief Structure that represent thread for printing cpu load data */
typedef struct Printer Printer;
struct Printer {
    _Atomic(clock_t) last_update;   /**< Atomic time of last read */
};

extern Printer printer;

/**
 ** @name Initialization and destruction
 ** @{
 **/

/** @brief Initialize analyzer module.
 ** 
 ** Receive @a core_num and allocate inner values
 **/
void printer_init(int core_num);

/** @brief Thread function for @c Printer.
 ** 
 ** Print cpu load data in the form of table to @a stdout
 **
 ** @returns @a EXIT_SUCCESS if no error occurred
 **/
int printer_start(void *args);

/** @brief Destroys the module by freeing the memory */
void printer_destroy(void);
/**
 ** @}
 **/

#endif // CPUUT_PRINTER_H