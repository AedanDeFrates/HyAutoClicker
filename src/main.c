#include "shared_vars.h"
#include "global_listen.h"
#include "change_hotkey.h"
#include "auto_click.h"


//=============================================================
//  Global pointer delcarations for gtk widgets and the builder
//=============================================================

//Main window and its containers
GtkWidget   *window1;

GtkWidget   *box1;
GtkWidget   *menu;
GtkStack    *stack1;
GtkWidget   *actionBar1;

//==============================================
// New Interval Spin Button Pointer Declarations
//==============================================

GtkWidget   *intervalGrid;

GtkWidget   *millisecondSpin;
GtkWidget   *secondSpin;
GtkWidget   *minuteSpin;
GtkWidget   *hourSpin;

//==============================================
//==============================================

GtkWidget   *fixed1;
GtkWidget   *fixed2;
GtkWidget   *fixed3;

GtkWidget   *autoClickerTab;
GtkWidget   *settingsTab;
GtkWidget   *helpTab;

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

//==========================================
// New Global Variables for Click Interval
//==========================================

volatile gint clickIntervalMilliseconds = 0;
volatile gint clickIntervalSeconds = 1;
volatile gint clickIntervalMinutes = 0;
volatile gint clickIntervalHours = 0;

volatile gint clickIntervalTotal = 1000;

//Input event for click type
volatile struct input_event clickType = {
    .type = EV_KEY,
    .code = BTN_LEFT,
    .value = 0
};


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
    win1Geometry.min_width = 600;
    win1Geometry.min_height = 300;

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
    toggleListen        = GTK_WIDGET(gtk_builder_get_object(builder, "toggleListen"));
    rightClickRadio     = GTK_WIDGET(gtk_builder_get_object(builder, "rightClickRadio"));
    leftClickRadio      = GTK_WIDGET(gtk_builder_get_object(builder, "leftClickRadio"));
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(leftClickRadio), TRUE);

    changeHotkeyToggle = GTK_WIDGET(gtk_builder_get_object(builder, "changeHotkeyToggle"));
    
    //Tabs that switch container stack
    settingsTab     = GTK_WIDGET(gtk_builder_get_object(builder, "settingsTab"));
    autoClickerTab  = GTK_WIDGET(gtk_builder_get_object(builder, "autoClickerTab"));
    helpTab         = GTK_WIDGET(gtk_builder_get_object(builder, "helpTab"));

    //Containers for the stack
    fixed1   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    fixed2   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed2"));
    fixed3   = GTK_WIDGET(gtk_builder_get_object(builder, "fixed3"));

    //=============================================================
    // New Interval CPS Spin Button Pointer Assignments
    //=============================================================

    intervalGrid = GTK_WIDGET(gtk_builder_get_object(builder, "intervalGrid"));

    millisecondSpin = GTK_WIDGET(gtk_builder_get_object(builder, "millisecondSpin"));
    secondSpin      = GTK_WIDGET(gtk_builder_get_object(builder, "secondSpin"));
    minuteSpin      = GTK_WIDGET(gtk_builder_get_object(builder, "minuteSpin"));
    hourSpin        = GTK_WIDGET(gtk_builder_get_object(builder, "hourSpin"));

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
    if(listening && !hotkeyIsActive)
    {
        gtk_button_set_label(GTK_BUTTON(b), "Listening...");
        g_thread_new("globalListen", (GThreadFunc)start_global_listen, NULL);
    }
    else gtk_button_set_label(GTK_BUTTON(b), "Start");
}

//==============================================================
// New Signal Handler Functions for Interval Spin Buttons
//==============================================================

/*
Millisecond -> 1
Second -> 1000
Minute -> 60000
Hour -> 3600000
*/
void interval_value_changed()
{
    clickIntervalTotal = clickIntervalMilliseconds + clickIntervalSeconds * 1000 + clickIntervalMinutes * 60000 + clickIntervalHours * 3600000;

    g_print("Click Interval set to: %d milliseconds\n", clickIntervalTotal);
}

void on_adjustMillisecond_value_changed()
{
    clickIntervalMilliseconds = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(millisecondSpin));
    g_print("Millisecond Interval set to: %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(millisecondSpin)));

    interval_value_changed();
}
void on_adjustSecond_value_changed()
{
    clickIntervalSeconds = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(secondSpin));
    g_print("Second Interval set to: %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(secondSpin)));

    interval_value_changed();
}
void on_adjustMinute_value_changed()
{
    clickIntervalMinutes = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(minuteSpin));
    g_print("Minute Interval set to: %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(minuteSpin)));

    interval_value_changed();
}
void on_adjustHour_value_changed()
{
    clickIntervalHours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(hourSpin));
    g_print("Hour Interval set to: %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(hourSpin)));

    interval_value_changed();
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
    g_print("Auto Clicker Tab Activated\n");
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