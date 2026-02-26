#ifndef SHARED_H
#define SHARED_H

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

extern volatile bool hotkeyIsActive;
extern volatile gboolean listening;
extern volatile gboolean window1IsActive;
extern volatile gboolean hotkeyChangeMode;
extern volatile gint cpsVal;

gboolean on_hotkey_press(GtkWidget *w, GdkEventKey *e);
gpointer start_auto_clicker(gpointer data);

#endif