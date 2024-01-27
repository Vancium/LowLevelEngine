#pragma once

#include "defines.h"

//Disable assertions
#define LLE_ASSERTIONS_ENABLED

#ifdef LLE_ASSERTIONS_ENABLED
    #if _MSC_VER
        #include <intrin.h>
        #define debugBreak() _debugbreak()
    #else
        #define debugBreak() __builtin_trap()
    #endif

    LLAPI void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

    #define LLASSERT(expr) { if(expr) {} else { report_assertion_failure(#expr, "", __FILE__, __LINE__); debugBreak(); }}
    #define LLASSERT_MESSAGE(expr, message) { if(expr) {} else { report_assertion_failure(#expr, message, __FILE__, __LINE__); debugBreak(); }}
    
    #ifdef LLE_DEBUG
        #define LLASSERT_DEBUG(expr) { if(expr) {} else { report_assertion_failure(#expr, "", __FILE__, __LINE__); debugBreak(); }}
    #else
        #define LLASSERT_DEBUG(expr)
    #endif
    #else
        #define LLASSLLASSERT(expr) //no op
        #define LLASLLASSERT_MESSAGE(expr, message) //no op
        #define LLASLLASSERT_DEBUG(expr)// no op
    
#endif


