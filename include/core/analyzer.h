#ifndef CPUUT_ANALYZER_H
#define CPUUT_ANALYZER_H

#include <stdint.h>
#include <stdlib.h>

#include "core/types.h"


uint32_t calc_usage(cpu_time *time_prev, cpu_time *time_now);

#endif // CPUUT_ANALYZER_H