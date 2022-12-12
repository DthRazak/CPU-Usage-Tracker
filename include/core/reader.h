#ifndef CPUUT_READER_H
#define CPUUT_READER_H

#include <stdint.h>
#include <stdlib.h>

#include "core/types.h"

typedef struct Reader Reader;
struct Reader {
    cpu_stat cpu_data;
};

int reader_init(void);
void reader_destroy(void);

#endif // CPUUT_READER_H