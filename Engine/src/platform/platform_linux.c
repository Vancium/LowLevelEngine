#include "platform.h"
#include <bits/time.h>
#include <xcb/xproto.h>


#if LLPLATFORM_LINUX

    #include <xcb/xcb.h>
    #include <X11/keysym.h>
    #include <X11/XKBlib.h>
    #include <X11/Xlib.h>
    #include <X11/Xlib-xcb.h>
    #include <sys/time.h>
    #include "core/Log/log.h"

    #if _POSIX_C_SOURCE >= 199309L //Allows the use of functions that are not part of the standard C library but are part of the POSIX.
        #include <time.h> //nanosleep
    #else
        #include <unistd.h> //usleep
    #endif

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    typedef struct linux_platform_layer {
        Display *display;
        xcb_connection_t* connection;
        xcb_window_t window;
        xcb_screen_t* screen;
        xcb_atom_t wm_protocols;
        xcb_atom_t wm_delete_win;
    } linux_platform_layer;

    b8 platform_create(
        platform_state* plat_state,
        const char* name, 
        i32 x,
        i32 y,
        i32 height,
        i32 width
    ) {
        //Create linux state
        plat_state->internal_state = malloc(sizeof(linux_platform_layer));
        linux_platform_layer* curr_state = (linux_platform_layer*)plat_state->internal_state;

        //Connect to X server
        curr_state->display = XOpenDisplay(NULL);
        
        //Turn of key repeats, global across the OS, must enable when application closes
        XAutoRepeatOff(curr_state->display);

        //Get connection object from display
        curr_state->connection = XGetXCBConnection(curr_state->display);

        if (xcb_connection_has_error(curr_state->connection)) {
            LLE_FATAL("Failed to connect to the X server!");
            return FALSE;
        }
            
        //Retrieve setup from X
        const struct xcb_setup_t *setup = xcb_get_setup(curr_state->connection);


        //Loop through screens usign iterator
        int screen_p = 0;
        xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

        for (i32 s = screen_p; s > 0; s--) {
            xcb_screen_next(&iter);    
        }

        
        // After iterating assign screen data to linux_layer_state
        curr_state->screen = iter.data; 


        // Allocate a X-ID for the windows to be created
        curr_state->window = xcb_generate_id(curr_state->connection);

        
        // Registar event types
        
        u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

        u32 event_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE
                            | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE 
                            | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION
                            | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

        // Values to be sent to XCB (bg, color, events)
        u32 value_list[] = { curr_state->screen->black_pixel, event_values };

        // Create Window Cookie
        xcb_void_cookie_t window_cookie = xcb_create_window(
            curr_state->connection,
            XCB_COPY_FROM_PARENT, // depth
            curr_state->window,
            curr_state->screen->root, // parent
            x,
            y,
            width,
            height,
            0,                          // no border
            XCB_WINDOW_CLASS_INPUT_OUTPUT, // class
            curr_state->screen->root_visual,
            event_mask,
            value_list
        );

        // Set window title  
        xcb_change_property(
            curr_state->connection, 
            XCB_PROP_MODE_REPLACE, 
            curr_state->window, 
            XCB_ATOM_WM_NAME, 
            XCB_ATOM_STRING, 
            8, // data should be viewed 8 bits at a time 
            strlen(name), 
            name
        );

        // Tells server to notify when the window manager attempts to destroy the window
        xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(
            curr_state->connection,
            0,
            strlen("WM_DELETE_WINDOW"),
            "WM_DELETE_WINDOW"
        );

        xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(
            curr_state->connection,
            0,
            strlen("WM_PROTOCOLS"),
            "WM_PROTOCOLS"
        );
        
        xcb_intern_atom_reply_t* wm_delete_reply = xcb_intern_atom_reply(
            curr_state->connection, 
            wm_delete_cookie, 
            NULL
        );

        xcb_intern_atom_reply_t* wm_protocols_reply = xcb_intern_atom_reply(
            curr_state->connection,
            wm_protocols_cookie,
            NULL
        );
        
        curr_state->wm_delete_win = wm_delete_reply->atom;
        curr_state->wm_protocols = wm_protocols_reply->atom;

        xcb_change_property(
            curr_state->connection, 
            XCB_PROP_MODE_REPLACE, 
            curr_state->window, 
            wm_protocols_reply->atom, 
            4, 
            32, 
            1, 
            &wm_delete_reply->atom
        );

        // Map window to screen
        xcb_map_window(curr_state->connection, curr_state->window);

        // Flush stream
        
        i32 stream_result = xcb_flush(curr_state->connection);
        if (stream_result <= 0) {
            LLE_FATAL("An error occured when flushing the stream: %d", stream_result);
            return FALSE;
        }


            return TRUE;
        }

        void platform_destroy(platform_state* plat_state) {
            // cold cast to known type
            linux_platform_layer* curr_state = (linux_platform_layer*)plat_state->internal_state;
            XAutoRepeatOn(curr_state->display);

            xcb_destroy_window(curr_state->connection, curr_state->window);
        }


        //Mostly to handle printing to console on Windows
        b8 platform_pump_messages(platform_state* plat_state) {
            
            // cold cast to known type
            linux_platform_layer* curr_state = (linux_platform_layer*)plat_state->internal_state;

            xcb_generic_event_t* event;
            xcb_client_message_event_t* client_message;
            
            b8 quit_flagged = FALSE;

            while(event != 0) {
                event = xcb_poll_for_event(curr_state->connection);
                if (event == 0) {
                    break;
                }

                // Input events
                switch(event->response_type & ~0x80) {
                    case XCB_KEY_PRESS:
                    case XCB_KEY_RELEASE: {
                        // TODO handle input events
                    } break;
                    case XCB_BUTTON_PRESS:
                    case XCB_BUTTON_RELEASE: {
                        // TODO handle mouse button events
                    } break;
                    case XCB_MOTION_NOTIFY: {
                        // TODO handle mouse movement event
                    } break;
                    case XCB_CONFIGURE_NOTIFY: {
                        // TODO hadnle window resize event
                    } break;
                    case XCB_CLIENT_MESSAGE: {
                        client_message = (xcb_client_message_event_t*)event;
                        if (client_message->data.data32[0] == curr_state->wm_delete_win) {
                            quit_flagged = TRUE;  
                        }
                    } break;
                    default: break;
                    
                }
                free(event);
        
            }
            return !quit_flagged;
        }


        void* platform_allocate(u64 size, b8 aligned) {
            return malloc(size);
        }

        void platform_free(void* block, b8 aligned) {
            free(block);
        }
        void* platform_zero_memory(void* block, u64 size) {
            memset(block, 0, size);
        }
        void* platform_copy_memory(void* dest, const void* src, u64 size) {
            return memcpy(dest, src, size);
        }
        void* platform_set_memory(void* dest, i32 value, u64 size) {
            return memset(dest, value, size);
        }

        void platform_console_write(const char* message, u8 color) {
            const char* color_strings[] = { "0;41", "1;31", "1;33", "1;32", "1;34", "1;30" };
            printf("\033[%sm%s\033[0m", color_strings[color], message);
        }
        void platform_console_write_error(const char* message, u8 color) {

            const char* color_strings[] = { "0;41", "1;31", "1;33", "1;32", "1;34", "1;30" };
            printf("\033[%sm%s\033[0m", color_strings[color], message);
        }

        f64 platform_get_absolute_time() {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            return now.tv_sec + now.tv_nsec * 0.000000001;
        }

        void platform_sleep(u64 ms) {
            #if _POSIX_C_SOURCE >= 199309L
                struct timespec ts;
                ts.tv_sec = ms / 1000;
                ts.tv_nsec = (ms % 1000) * 1000 * 1000;
                nanosleep(&ts, 0);
            #else
                if (ms >= 1000) {
                    sleep(ms / 1000);
                }
                usleep((ms % 1000) * 1000);
            #endif

        }
#endif

