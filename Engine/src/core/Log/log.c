#include "log.h"

//Temporary
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 init_logging();

void shutdown_logging();

void log_output(log_level level, const char *msg, ...) {
    const char* log_level_strings[6] = {"[FATAL]: ","[ERROR]: ", "[WARN]: ","[DEBUG]: ", "[INFO]: ","[TRACE]: "};

    const u16 MAX_MESSAGE_LENGTH = 32000;

    char temp_message[MAX_MESSAGE_LENGTH];
    char out_message[MAX_MESSAGE_LENGTH];
    memset(temp_message, 0, MAX_MESSAGE_LENGTH);

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(temp_message, MAX_MESSAGE_LENGTH, msg, arg_ptr);
    va_end(arg_ptr);

    sprintf(out_message, "%s%s\n", log_level_strings[level], temp_message);

    printf("%s", out_message);
    
}
