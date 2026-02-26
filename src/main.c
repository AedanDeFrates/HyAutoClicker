#include "shared.h"
#include "global_listen.h"


//=============================================================
//  Global pointer delcarations for gtk widgets and the builder
//=============================================================
GtkWidget   *window1;
GtkWidget   *container1;
GtkWidget   *box1;
GtkWidget   *actionBar1;

GtkWidget   *menu;
GtkWidget   *settingsHeader;
GtkWidget   *viewHeader;
GtkWidget   *helpHeader;

GtkWidget   *spinCPS;
GtkWidget   *toggleListen;


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

    //=========================================
    // Setting the min and max for the window
    //=========================================
    GdkGeometry win1Geometry;
    win1Geometry.min_width = 400;
    win1Geometry.min_height = 200;

    win1Geometry.max_width = 800;
    win1Geometry.max_height = 400;

    gtk_window_set_geometry_hints(GTK_WINDOW(window1), NULL, &win1Geometry, GDK_HINT_MIN_SIZE | GDK_HINT_MAX_SIZE);

    //==========================================
    //  Connects signals to the main window
    //==========================================

    g_signal_connect(window1, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_builder_connect_signals(builder, NULL);

    //================================================================
    //Gives the global pointers the values from the used GTK widgets
    //================================================================
    container1   = GTK_WIDGET(gtk_builder_get_object(builder, "container1"));
    box1         = GTK_WIDGET(gtk_builder_get_object(builder, "box1"));
    actionBar1   = GTK_WIDGET(gtk_builder_get_object(builder, "actionBar1"));

    spinCPS      = GTK_WIDGET(gtk_builder_get_object(builder, "spinCPS"));
    toggleListen = GTK_WIDGET(gtk_builder_get_object(builder, "toggleListen"));

    menu         = GTK_WIDGET(gtk_builder_get_object(builder, "menu"));
    settingsHeader = GTK_WIDGET(gtk_builder_get_object(builder, "settingsHeader"));
    viewHeader   = GTK_WIDGET(gtk_builder_get_object(builder, "viewHeader"));
    helpHeader   = GTK_WIDGET(gtk_builder_get_object(builder, "helpHeader"));

    cpsVal = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS));
    
    gtk_widget_show(window1);
    gtk_main();
    return EXIT_SUCCESS;
}

/*
 *  Function recieves signal from the toggle button.
 *  After setting the val of listening, it will change the label of the button to "Listening..." if listening is TRUE 
 *  And hotkeyIsActive is FALSE, otherwise it will set the label to "Start". 
 *  It also updates the global variable cpsVal with the current value of the spin button.
 */
void on_toggleListen_toggled(GtkToggleButton *b)
{  
    listening = gtk_toggle_button_get_active(b);
    if(listening && !hotkeyIsActive){gtk_button_set_label(GTK_BUTTON(b), "Listening...");}
    else gtk_button_set_label(GTK_BUTTON(b), "Start");
    gint spinVal = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS)); 
}

/*
 * Function returns TRUE if the event is a key press of the hotkey (F8) and listening is TRUE, otherwise it returns FALSE.
*/
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

/*
 * Function recieves signal from a change in the spin button.
 * It updates the global var cpsVal with the new CPS value and prints it to the console.
 */
void on_spinCPS_value_changed()
{
    g_print("CPS set to: %d", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS)));
    cpsVal = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS));
}

/*
 * Function recieves signal from a change in the focus of the main window.
 * It updates the global var window1IsActive with the new activity status of the main window and prints it to the console.
 * If the main window is not active, it starts a new thread to listen for the hotkey globally. Otherwise, it returns.
*/
void on_window1_focus_changed(GObject *o, GParamSpec *gpspec, gpointer user_data)
{
    window1IsActive = gtk_window_is_active(GTK_WINDOW(window1));
    //get activity status of current window
    if (!window1IsActive) { g_thread_new("globalListen", (GThreadFunc)start_global_listen, NULL);}
    else {return;}
}

void on_settings_activate()
{

}

void on_autoclicker_activate()
{

}