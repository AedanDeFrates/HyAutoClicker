#include "shared.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>

void start_global_listen()
{
    //If the start button is not activated then the program simply cannot be run with a hotkey
    //And since the program cannot listen unless the window is active, and the if the window is active
    //Then the standard gtk signal works fine
    //Thus this prog terminates
    if(!listening)
    {
        return;
    }
    
    Display *display = XOpenDisplay(NULL);
    if(!display){g_print("Failed to open display"); return;}
    
    Window root = DefaultRootWindow(display);
    unsigned int modifiers = ControlMask | Mod1Mask;
    int keycode = XKeysymToKeycode(display, XK_A);
    
    while(!window1IsActive)
    {
        
        g_usleep(10000);
        
    }
}

//VERY IMPORTANT
//g_idle_add((GSourceFunc)on_hotkey_press, NULL);
