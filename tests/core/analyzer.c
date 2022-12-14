#include <criterion/criterion.h>

#include "core/reader.h"
#include "core/analyzer.h"


extern Analyzer analyzer;

Test(reader_test, analyzer_initialization){
    int cores = reader_init();
    analyzer_init(cores);

    cr_assert_neq(analyzer.cpu_usage_buffer, (void *)0);

    analyzer_destroy();
    reader_destroy();
}
