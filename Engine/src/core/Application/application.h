#pragma once

#include "defines.h"

struct game;


typedef struct application_config {
    i16 start_x_pos;
    i16 start_y_pos;
    i16 start_width;
    i16 start_height;
    char* name;
} application_config;


LLAPI b8 application_create(struct game* game_instance);

LLAPI b8 application_run();
