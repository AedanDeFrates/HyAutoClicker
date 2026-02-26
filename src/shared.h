#ifndef SHARED_H
#define SHARED_H

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>


//Globally used boolean variables for different program states
extern volatile bool hotkeyIsActive;
extern volatile gboolean listening;
extern volatile gboolean window1IsActive;
extern volatile gboolean hotkeyChangeMode;

//Global variable for CPS value, used in the auto clicker thread
extern volatile gint cpsVal;

//Global variable to set the type of click to be used
extern volatile struct input_event clickType;

//Global function declarations for gtk signal handlers and threads
gboolean on_hotkey_press(GtkWidget *w, GdkEventKey *e);
gpointer start_auto_clicker(gpointer data);

#endif