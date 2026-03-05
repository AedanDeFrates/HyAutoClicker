#include "shared.h"
#include "interval_sum.h"

gpointer start_auto_clicker(gpointer arg)
{
    clickIntervalTotal = sum_all_intervals();

    g_print("Auto Clicker Thread Started\nClick Type: %d\nCPS: %d\n", clickType.code, GPOINTER_TO_INT(arg));
    int cps = GPOINTER_TO_INT(arg); 
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0)
    {
        perror("File Open Error");
        return NULL;
    }
    
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);

    ioctl(fd, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE);    
    
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);
    
    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "autoclicker-mouse");

    uidev.id.bustype = BUS_USB;
    uidev.id.product = 0x5678;
    uidev.id.version = 1;

    write(fd, &uidev, sizeof(uidev));
    ioctl(fd, UI_DEV_CREATE);
    
    
    struct input_event ev;    
    memset(&ev, 0, sizeof(ev));
    
    while(hotkeyIsActive && listening)
    {
        gettimeofday(&ev.time, NULL);
        ev.type = EV_KEY;
        ev.code = clickType.code; //BTN_LEFT or BTN_RIGHT
        ev.value = 1;
        write(fd, &ev, sizeof(ev));

        gettimeofday(&ev.time, NULL);
        ev.type = EV_SYN;
        ev.code = SYN_REPORT; //Indicates the end of an event sequence
        ev.value = 0;
        write(fd, &ev, sizeof(ev));        

        g_usleep(100);

        gettimeofday(&ev.time, NULL);
        ev.type = EV_KEY;
        ev.code = clickType.code; //BTN_LEFT or BTN_RIGHT
        ev.value = 0;
        write(fd, &ev, sizeof(ev));
        
        gettimeofday(&ev.time, NULL);
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        write(fd, &ev, sizeof(ev));
        
        g_usleep(1000000 / cps);

        if(clickType.code == BTN_LEFT)
        {
            g_print("Left Click Sent\n");
        }
        else if(clickType.code == BTN_RIGHT)
        {
            g_print("Right Click Sent\n");
        }
    }
    
    if(hotkeyIsActive && !listening)
    {
        hotkeyIsActive = !hotkeyIsActive;
    }
    
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
    return NULL;
}
