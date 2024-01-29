
#include "LLE_Interface.h"
#include "core/Log/log.h"
#include "core/Application/application.h"

int main(void) {

    application_config config;
    config.name = "Low Level Engine";
    config.start_x_pos = 100;
    config.start_y_pos = 100;
    config.start_width = 1280;
    config.start_height = 720;

    application_create(&config);
    application_run();

    return 0;
}
