#include "shared.h"
#include "global_listen.h"


//=============================================================
//  Global pointer delcarations for gtk widgets and the builder
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
volatile gint cpsVal = 0;

//==============================
// Global function delclarations
//==============================

void on_window1_focus_changed(GObject *o, GParamSpec *gpspec, gpointer user_data);


int main(int argc, char *argv[])
{  
    XInitThreads();   
     
    //Initializes GTK
    gtk_init(&argc, &argv);

    //Initializes the builder from the glade application (from the xml)
    builder = gtk_builder_new_from_file ("HyAutoClicker.glade");

    //Initializes the window Widget
    window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    
    g_signal_connect(window1, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_builder_connect_signals(builder, NULL);

    //Gives the global pointers the values from the used GTK widgets
    container1   = GTK_WIDGET(gtk_builder_get_object(builder, "container1"));
    spinCPS      = GTK_WIDGET(gtk_builder_get_object(builder, "spinCPS"));
    toggleListen = GTK_WIDGET(gtk_builder_get_object(builder, "toggleListen"));
    listenLabel  = GTK_WIDGET(gtk_builder_get_object(builder, "listenLabel"));
    
    cpsVal = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS));
    

    gtk_widget_show(window1);
    gtk_main();
    return EXIT_SUCCESS;
}

void on_toggleListen_toggled(GtkToggleButton *b)
{  
    listening = gtk_toggle_button_get_active(b);
    
    if(listening && !hotkeyIsActive){gtk_button_set_label(GTK_BUTTON(b), "Listening...");}
    else gtk_button_set_label(GTK_BUTTON(b), "Start");
    
    gint spinVal = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS)); 
}

gboolean on_hotkey_press(GtkWidget *w, GdkEventKey *e)
{
    //Exits the function if listening is FALSE or if the event -> keyval is not the correct hotkey
    if (e -> keyval != GDK_KEY_F8 || !(listening)) { return FALSE; }
    
    //Negates (!) the value of hotkeyIsActive
    hotkeyIsActive = !hotkeyIsActive;
    
    if (hotkeyIsActive)
    {
        g_thread_new("autoclicker", (GThreadFunc)start_auto_clicker, GINT_TO_POINTER(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS))));
        return TRUE;
    }
    return FALSE;
}

void on_spinCPS_value_changed()
{
    g_print("CPS set to: %d", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS)));
    cpsVal = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS));
}

void on_window1_focus_changed(GObject *o, GParamSpec *gpspec, gpointer user_data)
{
    window1IsActive = gtk_window_is_active(GTK_WINDOW(window1));

    //get activity status of current window
    if (!window1IsActive)
    {
        g_thread_new("globalListen", (GThreadFunc)start_global_listen, NULL);
    }
    else {return;}
}
