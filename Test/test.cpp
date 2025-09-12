#include <stdio.h>
#include <unistd.h>
#include "Onvif.h"

int main()
{
    Onvif onvif;
    onvif.start();
    getchar();
    onvif.stop();
    getchar();
    return 1;

    while (1)
    {
        sleep(1);
    }
    
    return 0;
}