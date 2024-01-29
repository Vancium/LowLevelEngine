#pragma once

#include "core/Application/application.h"
#include "core/Log/log.h"
#include "game_types.h"


extern b8 create_game(game* out_game);


int main(void) {

    game game_instance;

    if (!create_game(&game_instance)) {
        LLE_FATAL("Could not create game");
        return -1;
    }

    if (!game_instance.render || !game_instance.update || !game_instance.initialize || !game_instance.on_resize) {
        LLE_FATAL("The game's function pointers must be assigned");
        return -2;
    }

    // Initialization
    if(!application_create(&game_instance)) {
        LLE_INFO("Application failed to be created");
        return 1;
    }
    // Game loop
    if(!application_run()) {
        LLE_INFO("Application did not shutdown correctly");
        return 2;
    }

    return 0;
}
