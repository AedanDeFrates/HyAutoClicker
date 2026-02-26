#include "shared.h"

void start_global_listen()
{
    
    if(!listening) { return; }
    
    Display *d = XOpenDisplay(NULL); if(!d){g_print("Failed to open display"); return;}
    Window root = DefaultRootWindow(d);
    XEvent ev;
    
    int keycode = XKeysymToKeycode(d, XK_F8);
    unsigned int modifiers = AnyModifier; //Can add settings or functionality for modifiers later
    
    XGrabKey(d, keycode, modifiers, root, True, GrabModeAsync, GrabModeAsync);
    
    XSelectInput(d, root, KeyPressMask);
    XSync(d, False);
    
    while(!window1IsActive)
    {
        XNextEvent(d, &ev);
        if (ev.type == KeyPress)
        {
            hotkeyIsActive = !hotkeyIsActive;
            g_thread_new("autoclicker_global", (GThreadFunc)start_auto_clicker, GINT_TO_POINTER(cpsVal));
        }
        g_usleep(100);
    }
}
