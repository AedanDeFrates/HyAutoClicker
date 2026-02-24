#include "shared.h"
#include "auto_click.h"
#include "global_listen.h"


//=============================================================
//  global pointer delcarations for gtk widgets and the builder
//=============================================================
GtkWidget   *window1;
GtkWidget   *container1;
GtkWidget   *spinCPS;
GtkWidget   *toggleListen;
GtkWidget   *listenLabel;

GtkBuilder  *builder;

//====================================
//  Global variables for gtk functions
//====================================

volatile bool hotkeyIsActive = false;
volatile gboolean listening = FALSE;
volatile gboolean window1IsActive = FALSE;

//==============================
// Global function delclarations
//==============================

gboolean on_hotkey_press(GtkWidget *w, GdkEventKey *e);
void on_window1_focus_changed(GObject *o, GParamSpec *gpspec, gpointer user_data);


int main(int argc, char *argv[])
{  
    //Initializes GTK
    gtk_init(&argc, &argv);

    //Initializes the builder from the glade application (from the xml)
    builder = gtk_builder_new_from_file ("HyAutoClicker.glade");

    //Initializes the window Widget
    window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    
    g_signal_connect(window1, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window1, "notify::is-active", G_CALLBACK(on_window1_focus_changed), NULL);

    gtk_builder_connect_signals(builder, NULL);

    //Gives the global pointers the values from the used GTK widgets
    container1   = GTK_WIDGET(gtk_builder_get_object(builder, "container1"));
    spinCPS      = GTK_WIDGET(gtk_builder_get_object(builder, "spinCPS"));
    toggleListen = GTK_WIDGET(gtk_builder_get_object(builder, "toggleListen"));
    listenLabel  = GTK_WIDGET(gtk_builder_get_object(builder, "listenLabel"));
    
    

    gtk_widget_show(window1);
    gtk_main();
    return EXIT_SUCCESS;
}

void on_toggleListen_toggled(GtkToggleButton *b)
{  
    listening = gtk_toggle_button_get_active(b);
    
    if(listening && hotkeyIsActive) { hotkeyIsActive = !hotkeyIsActive; }
    
    gint spinVal = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS)); 
    
    if (listening)
    {
        gtk_label_set_text (GTK_LABEL(listenLabel), (const gchar*) "Listening");
        g_print("Program is listening for hotkey...\n");
    }
    else 
    { 
        gtk_label_set_text (GTK_LABEL(listenLabel), (const gchar*) "fin"); 
        g_print("Program is not longer listening for hotkey... \n");
    }
}

gboolean on_hotkey_press(GtkWidget *w, GdkEventKey *e)
{
    //Exits the function if listening is FALSE or if the event -> keyval is not the correct hotkey
    if (e -> keyval != GDK_KEY_a || !(listening))
    {
        return FALSE;
    }
    
    g_print("The hotkey has been pressed and registered...\n");
    
    //Negates (!) the value of hotkeyIsActive
    hotkeyIsActive = !hotkeyIsActive;
    
    if (hotkeyIsActive)
    {
        g_print("Hotkey is active, starting subprocess script... \n");
        
        g_thread_new("autoclicker", (GThreadFunc)start_auto_clicker, GINT_TO_POINTER(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS))));
        return TRUE;
    }
    else { g_print("Hotkey is inactive, and the auto clicker has stopped...\n"); }
    return FALSE;
}

void on_spinCPS_value_changed()
{
    g_print("CPS set to: %d", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS)));
}

void on_window1_focus_changed(GObject *o, GParamSpec *gpspec, gpointer user_data)
{
    window1IsActive = gtk_window_is_active(GTK_WINDOW(window1));
    
    g_print("Window focus has updated.\n Window 1 activity status = %d\n" , window1IsActive);
    
    //get activity status of current window
    if (!window1IsActive)
    {
        g_thread_new("globalListen", (GThreadFunc)start_global_listen, NULL);
    }
    
    //if the status is not in focus and listening is true
        //start listening for hotkey in a global hotkey listener script
            //(this should end once focus comes back to the app window...
            //because the program is still listening and there is no way to undo listening unless in focus)
    //if the status has changed back to focus
        //end the global listener
}

/*
other things to add:
1. add Title
2. add a setting to choose hotkey
3. ...

*/















