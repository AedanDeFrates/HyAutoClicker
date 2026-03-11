#include "shared_vars.h"
#include "auto_click.h"

void start_global_listen()
{
    g_print("============Global Listen Thread Started=============\n");
    
    Display *d = XOpenDisplay(NULL); if(!d){g_print("Failed to open display"); return;}
    Window root = DefaultRootWindow(d);
    XEvent ev;
    
    int keycode = XKeysymToKeycode(d, XK_F8);
    unsigned int modifiers = AnyModifier; //Can add settings or functionality for modifiers later
    
    XGrabKey(d, keycode, modifiers, root, True, GrabModeAsync, GrabModeAsync);
    
    XSelectInput(d, root, KeyPressMask);
    XSync(d, False);
    
    while(listening)
    {
        g_print("============Waiting for Hotkey Press=============\n");
        XNextEvent(d, &ev);
        if (ev.type == KeyPress)
        {
            g_print("============Hotkey Pressed=============\n");
            
            hotkeyIsActive = !hotkeyIsActive;
            if(hotkeyIsActive) { g_thread_new("autoclicker_global", (GThreadFunc)start_auto_clicker, NULL); }
        }
        g_usleep(100);
    }

    XUngrabKey(d, keycode, modifiers, root);
    XSync(d, False);
    XCloseDisplay(d);
}
