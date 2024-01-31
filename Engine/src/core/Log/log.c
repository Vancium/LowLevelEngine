#include "log.h"

//Temporary
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "../Assert/assertions.h"
#include "platform/platform.h"


void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}
b8 init_logging() { return TRUE; };

void shutdown_logging() {};

void log_output(log_level level, const char *msg, ...) {
    const char* log_level_strings[6] = {"[FATAL]: ","[ERROR]: ", "[WARN]: ","[DEBUG]: ", "[INFO]: ","[TRACE]: "};
    b8 is_error = level < LOG_LEVEL_WARN;

    const u16 MAX_MESSAGE_LENGTH = 32000;

    char temp_message[MAX_MESSAGE_LENGTH];
    char out_message[MAX_MESSAGE_LENGTH];
    memset(temp_message, 0, MAX_MESSAGE_LENGTH);

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(temp_message, MAX_MESSAGE_LENGTH, msg, arg_ptr);
    va_end(arg_ptr);

    sprintf(out_message, "%s%s", log_level_strings[level], temp_message);

    if(is_error) {
        platform_console_write_error(out_message, level);
    } else {
        platform_console_write(out_message, level);
    }

    
}
