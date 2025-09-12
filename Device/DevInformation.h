#ifndef DEV_INFORMATION_H
#define DEV_INFORMATION_H

#include "Interface/Information.h"

class DevInformation : public Information
{
public:
    virtual const char* getManufacturer() const;
    virtual const char* getModel() const;
    virtual const char* getFirmwareVersion() const;
    virtual const char* getSerialNumber() const;
    virtual const char* getHardwareId() const;
};

#endif
