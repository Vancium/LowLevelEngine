
#include <stdio.h>

#include "LLE_Interface.h"
#include "core/Log/log.h"
#include "core/Test/test.h"

int main(void) {
    printf("Sandbox\n");
    print_engine();

    LLE_FATAL("Fatal: %f", 3.14);
    LLE_ERROR("Error: %f", 3.14);
    LLE_WARN("Warn: %f", 3.14);
    LLE_DEBUG("Debug: %f", 3.14);
    LLE_INFO("Info: %f", 3.14);
    LLE_TRACE("Trace: %f", 3.14);
}
