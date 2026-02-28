#include "shared.h"
#include "global_listen.h"
#include "change_hotkey.h"


//=============================================================
//  Global pointer delcarations for gtk widgets and the builder
//=============================================================

//Main window and its containers
GtkWidget   *window1;

GtkWidget   *box1;
GtkWidget   *menu;
GtkStack    *stack1;
GtkWidget   *actionBar1;

GtkWidget   *fixed1;
GtkWidget   *fixed2;
GtkWidget   *fixed3;

GtkWidget   *autoClickerTab;
GtkWidget   *settingsTab;
GtkWidget   *helpTab;

GtkWidget   *spinCPS;
GtkWidget   *toggleListen;
GtkWidget   *changeHotkeyToggle;
GtkWidget   *rightClickRadio;
GtkWidget   *leftClickRadio;

GtkBuilder  *builder;

//====================================
//  Global variables for gtk functions
//====================================

volatile bool hotkeyIsActive = false;
volatile gboolean listening = FALSE;
volatile gboolean hotkeyChangeMode = FALSE;
volatile gboolean window1IsActive = FALSE;
volatile gint cpsVal = 0;

volatile struct input_event clickType;

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

    //Main Window and its initial containers
    box1         = GTK_WIDGET(gtk_builder_get_object(builder, "box1"));
    actionBar1   = GTK_WIDGET(gtk_builder_get_object(builder, "actionBar1"));
    stack1       = GTK_STACK(gtk_builder_get_object(builder, "stack1"));
    menu         = GTK_WIDGET(gtk_builder_get_object(builder, "menu"));

    //Buttons, spin buttons, and radio buttons
    spinCPS      = GTK_WIDGET(gtk_builder_get_object(builder, "spinCPS"));
    toggleListen = GTK_WIDGET(gtk_builder_get_object(builder, "toggleListen"));
    rightClickRadio = GTK_WIDGET(gtk_builder_get_object(builder, "rightClickRadio"));
    leftClickRadio = GTK_WIDGET(gtk_builder_get_object(builder, "leftClickRadio"));
    changeHotkeyToggle = GTK_WIDGET(gtk_builder_get_object(builder, "changeHotkeyToggle"));
    
    //Tabs that switch container stack
    settingsTab = GTK_WIDGET(gtk_builder_get_object(builder, "settingsTab"));
    autoClickerTab   = GTK_WIDGET(gtk_builder_get_object(builder, "autoClickerTab"));
    helpTab   = GTK_WIDGET(gtk_builder_get_object(builder, "helpTab"));

    cpsVal = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinCPS));
    
    //Containers for the stack
    fixed1   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    fixed2   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed2"));
    fixed3   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed3"));


    g_print("stack1 pointer: %p\n", stack1);
    g_print("settingsTab pointer: %p\n", settingsTab);
    
    gtk_stack_set_visible_child(stack1, fixed1);

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
    g_print("=======Listening toggled=======\n");
    listening = gtk_toggle_button_get_active(b);
    g_print("Listening: %d\n", listening);
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

/*
 * Function recieves signal from the activation of the settings tab.
 * It sets the visible child of the stack to fixed2, which contains the settings tab
*/
void on_settingsTab_activate(GtkWidget *w)
{
    gtk_stack_set_visible_child(stack1, fixed2);
    if(listening)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggleListen), FALSE);
    }
    g_print("Settings Activated\n");
}

/*
 * Function recieves signal from the activation of the auto clicker tab.
 * It sets the visible child of the stack to fixed1, which contains the auto clicker tab
*/
void on_autoClickerTab_activate(GtkWidget *w)
{
    gtk_stack_set_visible_child(stack1, fixed1);
    if(hotkeyChangeMode)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(changeHotkeyToggle), FALSE);
    }
    g_print("Auto Clicker Activated\n");
}

void on_helpTab_activate(GtkWidget *w)
{
    gtk_stack_set_visible_child(stack1, fixed3);
    if(listening){ gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggleListen), FALSE); }
    else if(hotkeyChangeMode) { gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(changeHotkeyToggle), FALSE); }
    g_print("Help Activated\n");
}
void on_changeHotkeyToggle_toggled()
{
    GtkWidget *curr = gtk_stack_get_visible_child(GTK_STACK(stack1));
    hotkeyChangeMode = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(changeHotkeyToggle));
    if(curr != fixed2)
    {
        g_print("Hotkey Change Toggled, but not in settings tab\n"); return;
    }
    if(!hotkeyChangeMode)
    {
        g_print("Hotkey Change Toggled OFF\n"); return;
    }

    g_thread_new("hotkeyChangeListen", (GThreadFunc)start_hotkey_change_listen, NULL);
    g_print("listening for hotkey change\n");
}

void on_rightClickRadio_toggled()
{
    if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(rightClickRadio))) 
    { g_print("Right Click Toggled OFF\n"); return; }
    g_print("Right Click Toggled ON\n");

    clickType.code = BTN_RIGHT;
    g_print("clickType.code set to: %d\n", clickType.code);
}

void on_leftClickRadio_toggled()
{
    if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(leftClickRadio))) 
    { g_print("Left Click Radio Toggled OFF\n"); return; }
    g_print("Left Click Radio Toggled ON\n");

    clickType.code = BTN_LEFT;
    g_print("clickType.code set to: %d\n", clickType.code); 
}