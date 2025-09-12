#include "Onvif.h"
#include "Server.h"

Onvif::Onvif()
{}

Onvif::~Onvif()
{}

int Onvif::start()
{
    printf("Onvif start\n");
    Server::create();
    return 0;
}

int Onvif::stop()
{
    Server::destroy();
    return 0;
}
