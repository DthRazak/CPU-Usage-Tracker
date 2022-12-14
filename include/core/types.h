/**
 ** @file types.h
 ** @brief Core types declaration
 */


#ifndef CPUUT_TYPES_H
#define CPUUT_TYPES_H

#include <stdint.h>
#include <stdlib.h>


/** @brief Structure that represent cpu line in /proc/stat file */
typedef struct cpu_time cpu_time;
struct cpu_time {
    uint32_t user;          /**< Normal processes executing in user mode 0 */
    uint32_t nice;          /**< Niced processes executing in user mode */
    uint32_t system;        /**< Processes executing in kernel mode */
    uint32_t idle;          /**< Twiddling thumbs */
    uint32_t iowait;        /**< Waiting for I/O to complete 0 */
    uint32_t irq;           /**< Servicing interrupts */
    uint32_t softirq;       /**< Servicing softirqs */
    uint32_t steal;         /**< Involuntary wait */
    uint32_t guest;         /**< Running a normal guest */
    uint32_t guestnice;     /**< Running a niced guest */
};

/** @brief Structure that contains @c cpu_time array of size @a core_num + 1 */
typedef struct cpu_stat cpu_stat;
struct cpu_stat {
    size_t core_num;        /**< Number of cores */
    cpu_time* time_data;    /**< Array of cpu_time per core + total cpu_time */
};

/**
 ** @name Initialization and destruction
 ** @{
 **/

/** @brief Initialize values of @a cpu_data for @a cores.
 **
 ** If @a cores equal to @c 0 assign @a CPU_STAT_INIT
 **
 ** @param cpu_data Pointer to @a cpu_data structure
 ** @param cores    Number of cores 
 **/
void cpu_stat_init(cpu_stat *cpu_data, size_t cores);

/** @brief Create new object of type @c cpu_stat
 **
 ** @param cores    Number of cores 
 **/
cpu_stat* cpu_stat_new(size_t cores);

/** @brief Destroy inner values contained in @a cpu_data */
void cpu_stat_destroy(cpu_stat *cpu_data);

/** @brief Destroy inner values contained in @a cpu_data and free pointer */
void cpu_stat_delete(cpu_stat *cpu_data);
/**
 ** @}
 **/

/**
 ** @name Util fulnctions for @c cpu_stat
 ** @{
 **/

/** @brief Deep copy of two @a cpu_data structures */
void cpu_stat_copy(cpu_stat *dest, cpu_stat *src);
/**
 ** @}
 **/

/** @brief Structure that contains @c double array of size @a core_num + 1.
 ** Each element of array contains percentage of cpu usage
 */
typedef struct cpu_usage cpu_usage;
struct cpu_usage {
    size_t core_num;        /**< Number of cores */
    double* usage_data;     /**< Array containing the CPU load */
};

/**
 ** @name Initialization and destruction
 ** @{
 **/

/** @brief Initialize values of @a usage_data for @a cores.
 **
 ** If @a cores equal to @c 0 assign @a CPU_USAGE_INIT
 **
 ** @param usage_data   Pointer to @a cpu_usage structure
 ** @param cores        Number of cores 
 **/
void cpu_usage_init(cpu_usage *usage_data, size_t cores);

/** @brief Create new object of type @c cpu_usage
 **
 ** @param cores    Number of cores 
 **/
cpu_usage* cpu_usage_new(size_t cores);

/** @brief Destroy inner values contained in @a usage_data */
void cpu_usage_destroy(cpu_usage* usage_data);

/** @brief Destroy inner values contained in @a cpu_usage and free pointer */
void cpu_usage_delete(cpu_usage* usage_data);
/**
 ** @}
 **/

/**
 ** @name Util fulnctions for @c cpu_stat
 ** @{
 **/

/** @brief Deep copy of two @a cpu_usage structures */
void cpu_usage_copy(cpu_usage *dest, cpu_usage *src);
/**
 ** @}
 **/

/** @brief Default values for @a cpu_stat structure */
#define CPU_STAT_INIT                   \
{                                       \
    .core_num = 0,                      \
    .time_data = (void *)0              \
}

/** @brief Default values for @a cpu_usage structure */
#define CPU_USAGE_INIT                  \
{                                       \
    .core_num = 0,                      \
    .usage_data = (void *)0             \
}

#endif // CPUUT_TYPES_H