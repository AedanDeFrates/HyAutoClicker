#include "shared.h"

void start_hotkey_change_listen()
{
    g_print("Started hotkey change listen thread\n");

    Display *d = XOpenDisplay(NULL);
    if(!d){g_print("Failed to open display"); return;}
    Window root = DefaultRootWindow(d);
    XEvent ev;

    XSelectInput(d, root, KeyPressMask);
    XSync(d, False);

    while(hotkeyChangeMode)
    {
        g_print("Waiting for key press to change hotkey\n");
        XNextEvent(d, &ev);
        if (ev.type == KeyPress)
        {
            g_print("Key Press Detected\n");
            //Here we can add functionality to change the hotkey, but for now it just turns off hotkeyChangeMode
            hotkeyChangeMode = FALSE;
        }
        g_usleep(100);
    }
}