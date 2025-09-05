#ifndef ONVIF_H
#define ONVIF_H

class Onvif
{
public:
    Onvif();
    virtual ~Onvif();

public:
    int start();
    int stop();
};

#endif