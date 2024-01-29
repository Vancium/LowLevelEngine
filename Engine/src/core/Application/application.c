#include "application.h"
#include "platform/platform.h"
#include "core/Log/log.h"

typedef struct application_state {
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialized = FALSE;

static application_state app_state;


b8 application_create(application_config *config) {
    if (initialized) {
        LLE_ERROR("application_create() called more than once");
        return FALSE;
    }
    // Initialize subsytems
    init_logging(); //Logging capabilities

    //TODO remove
    LLE_FATAL("Fatal: %f", 3.14);
    LLE_ERROR("Error: %f", 3.14);
    LLE_WARN("Warn: %f", 3.14);
    LLE_DEBUG("Debug: %f", 3.14);
    LLE_INFO("Info: %f", 3.14);
    LLE_TRACE("Trace: %f", 3.14);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if(!platform_create(&app_state.platform, config->name, config->start_x_pos, config->start_y_pos, config->start_width, config->start_height)) {
        return FALSE;
    }
    platform_destroy(&app_state.platform);
    
    initialized = TRUE;

    return TRUE;

}

b8 application_run() {
    while(app_state.is_running) {
        if(!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }
    }

    app_state.is_running = FALSE;

    platform_destroy(&app_state.platform);

    return TRUE;
}


