#include "shared.h"

void start_global_listen()
{
    
    if(!listening) {g_print("Program is not listening... terminating global listening process\n"); return;}
    g_print("global listening has started\n");
    
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
        g_print("loop has started\n");
        XNextEvent(d, &ev);
        if (ev.type == KeyPress)
        {
            g_print("key has been pressed\n");
            hotkeyIsActive = !hotkeyIsActive;
            g_print("hotkey is: %d\n" , hotkeyIsActive);
            g_thread_new("autoclicker_global", (GThreadFunc)start_auto_clicker, GINT_TO_POINTER(cpsVal));
        }
        else { g_print("hotkey has not been pressed\n");}
        g_usleep(1000);
    }
    g_print("loop has ended\n");
}
