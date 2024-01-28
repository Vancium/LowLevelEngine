#include "platform.h"


#if LLPLATFORM_WINDOWS

    #include <stdio.h>
    #include <windows.h>
    #include <windowsx.h>

    typedef struct windows_layer_state {
        HINSTANCE h_instance; //Instance handle
        HWND h_windowHandle; // window handle
    } windows_layer_state;

    LRESULT CALLBACK win32_process_message(HWND window_handle, u32 message, WPARAM w_param, LPARAM l_param);

    b8 platform_create(
        platform_state* plat_state,
        const char* name, 
        i32 x,
        i32 y,
        i32 height,
        i32 width
    ) {
        //Crate platform internal state and cast it to windows_layer_state
        plat_state->internal_state = malloc(sizeof(windows_layer_state));
        windows_layer_state* curr_state = (windows_layer_state*)plat_state->internal_state;

        curr_state->h_instance = GetModuleHandleA(0);

        //Setup and register window class
        HICON icon = LoadIcon(curr_state->h_instance, IDI_APPLICATION);
        WNDCLASSA window_class;
        memset(&window_class, 0, sizeof(window_class));
        window_class.style = CS_DBCLICKS; // Recieve double click information
        window_class.lpfnWndProc = win32_process_message; //pointer to window procedure, handles events within the system
        window_class.cbClsExtra = 0; // Number of extra bytes to allocate following the window class-structure
        window_class.cbWndExtra = 0; // Number of extra bytes to allocate following the window instance
        window_class.hInstance = curr_state->h_instance; // 
        window_class.icon = icon;
        window_class.hCursor = LoadCursor(NULL, IDC_ARROW); //Manage cursor manually
        window_class.hbrBackground = NULL; // Transparent background and allows current application to paint to background
        window_class.lpszClassName = "lle_window_class";

        // Registar window class
        if(!RegisterClassA(&window_class)) {
            MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLEMATION | MB_OK);
            return FALSE;
        }

    }

    void platform_destroy(platform_state* plat_state);
     

    //Mostly to handle printing to console on Windows
    b8 platfrom_pump_messages(platform_state* state);


    void* platform_allocate(u64 size, b8 aligned);
    void platform_free(void* block, b8 aligned);
    void* platform_zero_memory(void* block, u64 size);
    void* platform_copy_memory(void* dest, const void* src, u64 size);
    void* platform_set_memory(void* dest, i32 value, u64 size);

    void platform_console_write(const char* message, u8 color);
    void platform_console_write_error(const char* message, u8 color);

    f64 platform_get_absolute_time();

    void platform_sleep(u64 ms);

    }
#endif



