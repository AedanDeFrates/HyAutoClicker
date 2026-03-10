#include "shared_vars.h"
#include "auto_click.h"

void start_global_listen()
{
    g_print("Global Listen Thread Started\n");
    if(!listening) { return; }
    
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
        XNextEvent(d, &ev);
        if (ev.type == KeyPress)
        {
            g_print("Hotkey Pressed\n");
            
            hotkeyIsActive = !hotkeyIsActive;
            if(hotkeyIsActive) { g_thread_new("autoclicker_global", (GThreadFunc)start_auto_clicker, NULL); }
        }
        g_usleep(100);
    }
}
