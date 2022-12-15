#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "logger.h"
#include "core/ring_buffer.h"
#include "termination_handler.h"

#define LOG_BUFFER_SIZE     30

static RingBuffer *buffer;
static log_level   level;

void logger_init(log_level lvl){
    level = lvl;
    
    BufferItemParams params = {
        .type = LOG_MSG_T,
        .core_num = 1           // Dummy var > 0 
    };
    buffer = RingBuffer_new(params, LOG_BUFFER_SIZE);
}

void logger_destroy(void){
    RingBuffer_delete(buffer);
}

extern volatile sig_atomic_t is_active;

int logger_start(void *args){
    FILE *file = fopen((char *)args, "w");

    logmsg message = {
        .level = level,
        .msd_time = time(0)
    };
    while (is_active){
        RingBuffer_read(buffer, &message);

        if (message.level >= level){
            if (message.level == DEBUG){
                fprintf(file, 
                    "DEBUG: %s - %s",
                    message.msg,
                    asctime(gmtime(&message.msd_time)));
            }else if (message.level == ERROR){
                fprintf(file,
                        "ERROR: %s - %s", 
                        message.msg,
                        asctime(gmtime(&message.msd_time)));
            }
        }
    }

    fclose(file);
}

void LOG(log_level level, time_t msg_time, char *msg){
    logmsg message = {
        .level = level,
        .msd_time = msg_time
    };
    strncpy(message.msg, msg, strlen(msg));

    RingBuffer_write(buffer, &message);
}

void logmsg_copy(logmsg *dest, logmsg *src){
    dest->level = src->level;
    dest->msd_time = src->msd_time;

    memset(dest->msg, 0, LOG_BUFFER_SIZE);
    strncpy(dest->msg, src->msg, strlen(src->msg)+1);
}