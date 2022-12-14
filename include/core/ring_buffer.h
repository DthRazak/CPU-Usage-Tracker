/**
 ** @file ring_buffer.h
 ** @brief RingBuffer type declaration
 */

#ifndef CPUUT_RINGBUFFER_H
#define CPUUT_RINGBUFFER_H

#include <stdint.h>
#include <stdlib.h>

#include "core/types.h"


/** @brief An opaque type for inter thread communication
 **  
 ** This data structure allows to send values of type @c cpu_stat or
 ** @c cpu_usage between threads.
 ** 
*/
typedef struct RingBuffer RingBuffer;

/** @brief Enum that allows to distinguish elements of @c RingBuffer */
typedef enum BufferItemType BufferItemType;
enum BufferItemType {
    CPU_STAT_T = 0, 
    CPU_USAGE_T
};

/** @brief Structure that contains settings for @c RingBuffer initialization */
typedef struct BufferItemParams BufferItemParams;
struct BufferItemParams {
    BufferItemType type;    /**< Item type: @c cpu_stat or @c cpu_usage*/
    size_t core_num;        /**< Number of cores */
};

/**
 ** @name Initialization and destruction
 ** @{
 **/

/** @brief Create new @c RingBuffer
 **
 ** @param params   Initialization parameters
 ** @param size     Buffer size
 **/
RingBuffer* RingBuffer_new(BufferItemParams params, size_t size);

/** @brief Destroy @a buffer and it's content, then free the pointer */
void RingBuffer_delete(RingBuffer* buffer);
/**
 ** @}
 **/

/**
 ** @name Utils functions for @c RingBuffer
 ** @{
 **/

/** @brief Read @a item from @a buffer.
 ** 
 ** If buffer is empty, current thread will wait until a new value arrives
 **/
void RingBuffer_read(RingBuffer *buffer, void *item);

/** @brief Write @a item to @a buffer.
 ** 
 ** If buffer is full, current thread will wait until a space becomes available
 **/
void RingBuffer_write(RingBuffer *buffer, void *item);
/**
 ** @}
 **/

#endif // CPUUT_RINGBUFFER_H