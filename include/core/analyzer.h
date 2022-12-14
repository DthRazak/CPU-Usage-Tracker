/**
 ** @file analyzer.h
 ** @brief Analyzer type declaration
 */

#ifndef CPUUT_ANALYZER_H
#define CPUUT_ANALYZER_H

#include <stdint.h>
#include <stdlib.h>

#include "core/types.h"
#include "core/ring_buffer.h"


/** @brief Structure that represent thread for receiving raw data
 ** from @c Reader and calualting cpu load
 **/
typedef struct Analyzer Analyzer;
struct Analyzer {
    RingBuffer *cpu_usage_buffer;   /**< @c RingBuffer of @c cpu_usage */
    _Atomic(time_t) last_update;   /**< Atomic time of last read */
};

extern Analyzer analyzer;

/**
 ** @name Initialization and destruction
 ** @{
 **/

/** @brief Initialize analyzer module.
 ** 
 ** Receive @a core_num and allocate RingBuffer
 **/
void analyzer_init(int core_num);

/** @brief Thread function for @c Analyzer.
 ** 
 ** Continuously read data from @c Reader @a buffer, calculate load in percentage
 ** and send new data through @c Analyzer @a buffer
 **
 ** @returns @a EXIT_SUCCESS if no error occurred
 **/
int analyzer_start(void *args);

/** @brief Destroys the module by freeing the memory */
void analyzer_destroy(void);
/**
 ** @}
 **/

/**
 ** @name Utils functions for @c RingBuffer
 ** @{
 **/

/** @brief Calculate load between two time points */
double calc_usage(cpu_time *time_prev, cpu_time *time_now);
/**
 ** @}
 **/

#endif // CPUUT_ANALYZER_H