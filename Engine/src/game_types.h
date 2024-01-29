#pragma once

#include "core/Application/application.h"
#include "defines.h"

/*
*
*  High level state of a game, called for creation by the application
*
*/

typedef struct game {
    application_config app_config;
    // Function pointer to game initialization
    b8 (*initialize)(struct game* game_instance);
    // Function pointer to game's update function
    b8 (*update)(struct game* game_instance, f32 delta_time);
    // Function pointer to game's render function
    b8 (*render)(struct game* game_instance, f32 delta_time);
    // Function pointer to handle resize
    void (*on_resize)(struct game* game_instance, u32 width, u32 height);
    
    // Game specific state, handled and created by game 
    void* state;
} game;
