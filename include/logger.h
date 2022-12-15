/**
 ** @file logger.h
 ** @brief logger type declaration
 */

#ifndef CPUUT_LOGGER_H
#define CPUUT_LOGGER_H

#include <time.h>

#define LOG_MSG_SIZE    40


/** @brief Enum that represent message severity */
typedef enum log_level log_level;
enum log_level {
    DEBUG = 0, 
    ERROR
};

/** @brief Struct that represent log message */
typedef struct logmsg logmsg;
struct logmsg {
    log_level   level;              /**< Message log level */
    time_t      msd_time;           /**< Message time */
    char        msg[LOG_MSG_SIZE];  /**< Message */
};

/**
 ** @name Initialization and destruction
 ** @{
 **/

/** @brief Initialize logger module.
 ** 
 ** Receive @c log_level
 **/
void logger_init(log_level level);

/** @brief Thread function for logging.
 ** 
 ** Write log information from different threads
 **
 ** @returns @a EXIT_SUCCESS if no error occurred
 **/
int logger_start(void *args);

/** @brief Destroys the module by freeing the memory */
void logger_destroy(void);
/**
 ** @}
 **/

/** @brief Log message according to parameters */
void LOG(log_level level, time_t msg_time, char *msg);

/** @brief Deep copy of two @a logmsg structures */
void logmsg_copy(logmsg *dest, logmsg *src);

#endif // CPUUT_LOGGER_H