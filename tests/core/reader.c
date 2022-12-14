#include <criterion/criterion.h>

#include "core/reader.h"


extern Reader reader;

Test(reader_test, reader_initialization){
    int ret = reader_init();

    cr_assert_geq(ret, 1);
    cr_assert_neq(reader.cpu_stat_buffer, (void *)0);

    reader_destroy();
}
