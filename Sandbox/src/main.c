
#include "LLE_Interface.h"
#include "platform/platform.h"

int main(void) {

    platform_state state;
    if(platform_create(&state, "LowLevelEngine Sandbox", 100, 100, 1280, 720)) {
        while(TRUE) {
            platform_pump_messages(&state);
        }
    }
    platform_destroy(&state);

    return 0;
}
