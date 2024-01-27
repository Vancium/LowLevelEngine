#pragma once

#include "defines.h"

//macros to enabled esay enabling and disabling of certain logging types

#define LOG_WARN_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_TRACE_ENABLED 1


// We dont want debug or trace logging during release build
#ifdef NDEBUG
    #define LOG_DEBUG_ENABLED 0
    #define LOG_TRACE_ENABLED 0
#endif


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

// These are always enabled
#define LLE_FATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
#define LLE_ERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);

#if LOG_WARN_ENABLED == 1
    #define LLE_WARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else 
    #define LLE_WARN(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
    #define LLE_DEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else 
    #define LLE_DEBUG(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
    #define LLE_INFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else 
    #define LLE_INFO(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
    #define LLE_TRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else 
    #define LLE_TRACE(message, ...)
#endif

