/**
 ** @file reader.h
 ** @brief Reader type declaration
 */

#ifndef CPUUT_READER_H
#define CPUUT_READER_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdatomic.h>

#include "core/types.h"
#include "core/ring_buffer.h"


/** @brief Structure that represent thread for reading /proc/stat file */
typedef struct Reader Reader;
struct Reader {
    RingBuffer *cpu_stat_buffer;    /**< @c RingBuffer of @c cpu_stat */
    _Atomic(clock_t) last_update;   /**< Atomic time of last read */
};

extern Reader reader;

/**
 ** @name Initialization and destruction
 ** @{
 **/

/** @brief Initialize reader module.
 ** 
 ** Open /proc/stat file to calculate number of cores and allocate RingBuffer
 **
 ** @returns Number of cores if SUCCESS, @a -1 otherwise
 **/
int reader_init(void);

/** @brief Thread function for @c Reader.
 ** 
 ** Continuously read /proc/stat file and send data through RingBuffer
 **
 ** @returns @a EXIT_SUCCESS if no error occurred, @a EXIT_FAILURE otherwise
 **/
int reader_start(void *args);

/** @brief Destroys the module by freeing the memory */
void reader_destroy(void);
/**
 ** @}
 **/

#endif // CPUUT_READER_H