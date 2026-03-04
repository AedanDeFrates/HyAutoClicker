include "shared.h"

gint sum_all_intervals()
{
    clickIntervalTotal = clickIntervalMilliseconds + (clickIntervalSeconds * 1000) + (clickIntervalMinutes * 60000) + (clickIntervalHours * 3600000);
    g_print("Total Click Interval: %d milliseconds\n", clickIntervalTotal);
    if(clickIntervalTotal == 0)
    {
        g_print("Error: Click Interval is 0... resetting to 1000 milliseconds\n");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(millisecondSpin), 0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(secondSpin), 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(minuteSpin), 0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(hourSpin), 0);
        
        clickIntervalTotal = 1000;
    }
    return clickIntervalTotal;
}