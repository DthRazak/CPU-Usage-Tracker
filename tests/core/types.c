#include <criterion/criterion.h>

#include "core/types.h"


Test(types_test, create_empty_cpu_stat){
    cpu_stat *ptr = cpu_stat_new(0);
    
    cr_assert(ptr->core_num == 0);
    cr_assert(ptr->time_data == (void *)0);

    cpu_stat_delete(ptr);
}

Test(types_test, create_non_empty_cpu_stat){
    cpu_stat *ptr = cpu_stat_new(10);
    
    cr_assert(ptr->core_num == 10);
    cr_assert_neq(ptr->time_data, (void *)0);

    cpu_stat_delete(ptr);
}

Test(types_test, copy_cpu_stat){
    cpu_stat *ptr1 = cpu_stat_new(5);
    cpu_stat *ptr2 = cpu_stat_new(5);

    for (size_t i = 0; i < ptr1->core_num + 1; ++i){
        cpu_time time = {
            .user = 10, 
            .nice = 10,
            .system = 10,
            .idle = 10,
            .iowait = 10,
            .irq = 10,
            .softirq = 10,
            .steal = 10,
            .guest = 10,
            .guestnice = 10
        };

        ptr1->time_data[i] = time;
    }

    cpu_stat_copy(ptr2, ptr1);

    cr_assert_arr_eq(ptr1->time_data, ptr2->time_data, ptr1->core_num+1);

    cpu_stat_delete(ptr1);
    cpu_stat_delete(ptr2);
}

Test(types_test, create_empty_cpu_usage){
    cpu_usage *ptr = cpu_usage_new(0);
    
    cr_assert(ptr->core_num == 0);
    cr_assert(ptr->usage_data == (void *)0);

    cpu_usage_delete(ptr);
}

Test(types_test, create_non_empty_cpu_usage){
    cpu_usage *ptr = cpu_usage_new(10);
    
    cr_assert(ptr->core_num == 10);
    cr_assert(ptr->usage_data != (void *)0);

    cpu_usage_delete(ptr);
}

Test(types_test, copy_cpu_usage){
    cpu_usage *ptr1 = cpu_usage_new(5);
    cpu_usage *ptr2 = cpu_usage_new(5);

    for (size_t i = 0; i < ptr1->core_num + 1; ++i){
        ptr1->usage_data[i] = 10.0;
    }

    cpu_usage_copy(ptr2, ptr1);

    cr_assert_arr_eq(ptr1->usage_data, ptr2->usage_data, ptr1->core_num+1);

    ptr1->usage_data[0] = 20.0;
    cr_assert_neq(ptr1->usage_data[0], ptr2->usage_data[0]);

    cpu_usage_delete(ptr1);
    cpu_usage_delete(ptr2);
}
