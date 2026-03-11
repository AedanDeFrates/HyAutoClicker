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
#include <glib.h>

//Mutex for thread-safe access to shared variables
extern GMutex shared_mutex;

//Globally used boolean variables for different program states
extern volatile bool hotkeyIsActive;
extern volatile gboolean listening;
extern volatile gboolean hotkeyChangeMode;

//Global variable for the click interval. To be assigned in main, and used in the auto click
extern volatile gint clickIntervalMilliseconds;
extern volatile gint clickIntervalSeconds;
extern volatile gint clickIntervalMinutes;
extern volatile gint clickIntervalHours;

extern volatile gint clickIntervalTotal;

//Global variable to set the type of click to be used
extern volatile struct input_event clickType;

#endif