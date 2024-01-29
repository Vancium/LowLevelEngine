#include "application.h"
#include "platform/platform.h"
#include "core/Log/log.h"
#include "game_types.h"

typedef struct application_state {
    game* game_instance;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialized = FALSE;

static application_state app_state;


b8 application_create(game* game_instance) {
    if (initialized) {
        LLE_ERROR("application_create() called more than once");
        return FALSE;
    }

    app_state.game_instance = game_instance;


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

    if(!platform_create(
            &app_state.platform, 
            game_instance->app_config.name, 
            game_instance->app_config.start_x_pos, 
            game_instance->app_config.start_y_pos, 
            game_instance->app_config.start_width, 
            game_instance->app_config.start_height)) {
        return FALSE;
    }

    if (!app_state.game_instance->initialize(app_state.game_instance)) {
        LLE_FATAL("Game failed to initialize");
        return FALSE;
    }

    app_state.game_instance->on_resize(app_state.game_instance, app_state.width, app_state.height);
    
    initialized = TRUE;

    return TRUE;

}

b8 application_run() {
    while(app_state.is_running) {
        if(!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if(!app_state.is_suspended) {
            if(!app_state.game_instance->update(app_state.game_instance, (f32)0)) {
                LLE_FATAL("Game update failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }
        }

        if(!app_state.game_instance->render(app_state.game_instance, (f32)0)) {
            LLE_FATAL("Game render failed, shutting down");
            app_state.is_running = FALSE;
            break;
        }
    }

    app_state.is_running = FALSE;

    platform_destroy(&app_state.platform);

    return TRUE;
}


