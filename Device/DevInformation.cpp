#include "DevInformation.h"

std::shared_ptr<Information> Information::create()
{
    return std::make_shared<DevInformation>();
}

const char* DevInformation::getManufacturer() const
{
    return "Demo";
}

const char* DevInformation::getModel() const
{
    return "Demo";
}

const char* DevInformation::getFirmwareVersion() const
{
    return "1.0.0";
}

const char* DevInformation::getSerialNumber() const
{
    return "123456789";
}

const char* DevInformation::getHardwareId() const
{
    return "00000000";
}