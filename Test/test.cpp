#include <unistd.h>
#include "Onvif.h"

int main()
{
    Onvif onvif;
    onvif.start();

    while (1)
    {
        sleep(1);
    }
    
    return 0;
}