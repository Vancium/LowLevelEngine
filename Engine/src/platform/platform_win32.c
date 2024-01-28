#include "platform.h"


#if LLPLATFORM_WINDOWS

    #include <stdio.h>
    #include "../core/Log/log.h"
    #include <windows.h>
    #include <windowsx.h>
    #include <stdlib.h>

    typedef struct windows_layer_state {
        HINSTANCE h_instance; //Instance handle
        HWND h_windowHandle; // window handle
    } windows_layer_state;

    //Clock
    static f64 clock_frequency;
    static LARGE_INTEGER start_time;

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
        window_class.style = CS_DBLCLKS; // Recieve double click information
        window_class.lpfnWndProc = win32_process_message; //pointer to window procedure, handles events within the system
        window_class.cbClsExtra = 0; // Number of extra bytes to allocate following the window class-structure
        window_class.cbWndExtra = 0; // Number of extra bytes to allocate following the window instance
        window_class.hInstance = curr_state->h_instance; // 
        window_class.hIcon = icon;
        window_class.hCursor = LoadCursor(NULL, IDC_ARROW); //Manage cursor manually
        window_class.hbrBackground = NULL; // Transparent background and allows current application to paint to background
        window_class.lpszClassName = "lle_window_class";

        // Registar window class
        if(!RegisterClassA(&window_class)) {
            MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
            return FALSE;
        }
        
        // Create Window

        // Client window values
        u32 client_x = x;
        u32 client_y = y;
        u32 client_height = height;
        u32 client_width = width;

        // Window 'window' values, seperating values between client and window allows for 
        u32 window_x = client_x;
        u32 window_y = client_y;
        u32 window_height = client_height;
        u32 window_width = client_width;

        u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        u32 window_ex_style = WS_EX_APPWINDOW;

        window_style |= WS_MAXIMIZEBOX;
        window_style |= WS_MINIMIZEBOX;
        window_style |= WS_THICKFRAME;

        // Obtain the size of the border
        RECT border_rect = { 0, 0, 0, 0 };
        AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

        // Border rectangle is negative and client window should be smaller than window 'window'
        window_x += border_rect.right;
        window_y += border_rect.top;

        //Grow window size by the size of the OS window border
        window_height += border_rect.right - border_rect.left;
        window_width += border_rect.bottom - border_rect.top;


        HWND window_handle = CreateWindowExA(
            window_ex_style,
            "lle_window_class",
            name,
            window_style,
            window_x,
            window_y,
            window_width,
            window_height,
            0,
            0,
            curr_state->h_instance,
            0
        );

        if(window_handle == 0) {
            MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            LLE_FATAL("Window creation failed!");
            return FALSE;
        } else {
            curr_state->h_windowHandle = window_handle; 
        }

        // Show window
        b32 should_activate = 1; // TODO if window should accept input this should be FALSE
        i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
        ShowWindow(curr_state->h_windowHandle, show_window_command_flags);

        //Clock setup
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        clock_frequency = 1.0 / (f64)frequency.QuadPart;
        QueryPerformanceCounter(&start_time);

        return TRUE;
    }

    void platform_destroy(platform_state* plat_state) {
        
        windows_layer_state* curr_state = (windows_layer_state*)plat_state->internal_state;
        
        if(curr_state->h_windowHandle) {
            DestroyWindow(curr_state->h_windowHandle);
            curr_state->h_windowHandle = 0;
        }
    }
     

    //Mostly to handle printing to console on Windows, called from main application loop
    b8 platform_pump_messages(platform_state* state) {

        /*
        *   Windows message system is a stack, if stack is not empty remove item from stack and display
        */
        MSG message;
        while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        return TRUE;
    }


    void* platform_allocate(u64 size, b8 aligned) {
        return malloc(size);
    }
    void platform_free(void* block, b8 aligned) {
        free(block);
    }
    void* platform_zero_memory(void* block, u64 size) {
        return memset(block, 0, sizeof(size));
    }   
    void* platform_copy_memory(void* dest, const void* src, u64 size) {
        return memcpy(dest, src, size);
    }
    void* platform_set_memory(void* dest, i32 value, u64 size) {
        return memset(dest, value, size);
    }

    void platform_console_write(const char* message, u8 color) {
        
        HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
        SetConsoleTextAttribute(console_handle, levels[color]);

        OutputDebugStringA(message); // Windows specific output stream
        u64 message_length = sizeof(message);
        LPDWORD number_written = 0;
        WriteConsoleA(console_handle, message, (DWORD)message_length, number_written, 0);
        
    }

    void platform_console_write_error(const char* message, u8 color) {
        HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
        static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
        SetConsoleTextAttribute(console_handle, levels[color]);

        OutputDebugStringA(message); // Windows specific output stream
        u64 message_length = sizeof(message);
        LPDWORD number_written = 0;
        WriteConsoleA(console_handle, message, (DWORD)message_length, number_written, 0);

    }

    f64 platform_get_absolute_time() {
        LARGE_INTEGER now_time;
        QueryPerformanceCounter(&now_time);
        return (f64)now_time.QuadPart * clock_frequency;
    }

    void platform_sleep(u64 ms) {
       Sleep(ms); 
    }

    LRESULT CALLBACK win32_process_message(HWND window_handle, u32 message, WPARAM w_param, LPARAM l_param) {
        switch(message) {
            case WM_ERASEBKGND: {
                // Notify the OS that ereasing will be handled by the application to prevent flickering
                return 1;
            } break;
            case WM_CLOSE: {
                //TODO fire application close event
                return 0;
            } break;
            case WM_DESTROY: {
                PostQuitMessage(0);
                return 0;
            } break;
            case WM_SIZE: {
                //Get updated window size
                /*
                    RECT r;
                    GetClientRect(window_handle, &r);
                    u32 width = r.right - r.left;
                    u32 height = r.bottom - r.top;
                */

                //TODO fire window resized event
                
            } break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                // b8 pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
                // TODO handle input
            } break;
            case WM_MOUSEMOVE: {
                //i32 x_pos = GET_X_LPARAM(l_param);
                //i32 y_pos = GET_Y_LPARAM(l_param);
                //TODO handle mouse move event

            } break;
            case WM_MOUSEWHEEL: {
                /*
                i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
                if (z_delta != 0) {
                    // flatten z_delta to OS agnostic value
                    z_delta = (z_delta < 0) ? -1 : 1;
                    // TODO handle input
                }
                */
            } break;
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP: {
                //b8 pressed = message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN;
                // TODO handle input
            } break;
        }
        return DefWindowProcA(window_handle, message, w_param, l_param);
    }
#endif // LLPLATFORM_WINDOWS



