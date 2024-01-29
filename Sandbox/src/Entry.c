#include <EntryPoint.h>
#include "Game.h"

#include <core/LLE_Memory/lle_memory.h>

b8 create_game(game *out_game) {
    out_game->app_config.name = "Low Level Engine";
    out_game->app_config.start_x_pos = 100;
    out_game->app_config.start_y_pos = 100;
    out_game->app_config.start_width = 1280;
    out_game->app_config.start_height = 720;

    out_game->initialize = game_initialize;
    out_game->render = game_render;
    out_game->update = game_update;
    out_game->on_resize = game_on_resize;

    // Create game state
    out_game->state = lle_allocate(sizeof(game_state), MEMORY_TAG_GAME);

    return TRUE;



}
