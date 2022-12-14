#include <criterion/criterion.h>

#include "core/types.h"
#include "core/ring_buffer.h"

#define BUFFER_SIZE 10


static RingBuffer *cpu_stat_buffer;
static RingBuffer *cpu_usage_buffer;

static cpu_stat   *stat_data; 
static cpu_usage  *usage_data;

void setup(void) {
    // Buffers setup
    BufferItemParams cpu_stat_params = {
        .type = CPU_STAT_T,
        .core_num = 4
    };
    cpu_stat_buffer = RingBuffer_new(cpu_stat_params, BUFFER_SIZE);

    BufferItemParams cpu_usage_params = {
        .type = CPU_USAGE_T,
        .core_num = 4
    };
    cpu_usage_buffer = RingBuffer_new(cpu_usage_params, BUFFER_SIZE);

    // Values for read and write setup
    stat_data  = cpu_stat_new(4);
    usage_data = cpu_usage_new(4);

    for (size_t i = 0; i < stat_data->core_num + 1; ++i){
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

        stat_data->time_data[i] = time;
    }

    for (size_t i = 0; i < usage_data->core_num + 1; ++i){
        usage_data->usage_data[i] = 10.0;
    }
}

void teardown(void) {
    RingBuffer_delete(cpu_stat_buffer);
    RingBuffer_delete(cpu_usage_buffer);

    cpu_stat_delete(stat_data);
    cpu_usage_delete(usage_data);
}

Test(ring_buffer_test, read_write, .init = setup, .fini = teardown){
    cpu_stat  *read_stat_data  = cpu_stat_new(4);
    cpu_usage *read_usage_data = cpu_usage_new(4);

    RingBuffer_write(cpu_stat_buffer, stat_data);
    RingBuffer_read(cpu_stat_buffer, read_stat_data);
    cr_assert(read_stat_data->core_num == stat_data->core_num);

    RingBuffer_write(cpu_usage_buffer, usage_data);
    RingBuffer_read(cpu_usage_buffer, read_usage_data);
    cr_assert(read_usage_data->core_num == usage_data->core_num);

    cpu_stat_delete(read_stat_data);
    cpu_usage_delete(read_usage_data);
}