#include "Onvif.h"
#include "Device/WSDiscovery.h"

Onvif::Onvif()
{}

Onvif::~Onvif()
{}

int Onvif::start()
{
    printf("Onvif start\n");
    WSDiscovery discovery;
    discovery.run(3702);
    return 0;
}

int Onvif::stop()
{
    return 0;
}

#include "Service/DeviceBinding.nsmap"
