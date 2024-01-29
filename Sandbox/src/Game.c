#include "Game.h"
#include <core/Log/log.h>

    
b8 game_initialize(game* game_instance) {
    LLE_INFO("game_initialize() called");
    return TRUE;
}

b8 game_update(game* game_instance, f32 delta_time) {

    LLE_INFO("game_update() called");
    return TRUE;
}

b8 game_render(game* game_instance, f32 delta_time) {

    LLE_INFO("game_render() called");
    return TRUE;
}

void game_on_resize(game* game_instance, u32 width, u32 height) {}
