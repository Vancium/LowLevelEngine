#pragma once

#include "defines.h"

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_DEBUG = 3,
    LOG_LEVEL_INFO = 4,
    LOG_LEVEL_TRACE = 5
} log_level;

LLAPI b8 init_logging();

LLAPI void shutdown_logging();

LLAPI void log_output(log_level level, const char* msg, ...);


//Logging macros

#define LLE_FATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
#define LLE_ERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#define LLE_WARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#define LLE_DEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#define LLE_INFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#define LLE_TRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
