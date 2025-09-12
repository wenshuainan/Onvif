#ifndef INTERFACE_INFORMATION_H
#define INTERFACE_INFORMATION_H

#include <memory>

class Information
{
public:
    static std::shared_ptr<Information> create();
    virtual const char* getManufacturer() const = 0;
    virtual const char* getModel() const = 0;
    virtual const char* getFirmwareVersion() const = 0;
    virtual const char* getSerialNumber() const = 0;
    virtual const char* getHardwareId() const = 0;
};

#endif