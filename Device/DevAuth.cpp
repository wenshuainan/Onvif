#include "DevAuth.h"

std::shared_ptr<Authentication> Authentication::create()
{
    return std::make_shared<DevAuth>();
}

bool DevAuth::getUserPasswd(const std::string& name, const std::string& realm, std::string& passwd)
{
    if (name == "admin" && realm == "onvif")
    {
        passwd = "123456";
        return true;
    }
    return false;
}

bool DevAuth::getUserPasswd(const std::string& name, std::string& passwd)
{
    if (name == "admin")
    {
        passwd = "123456";
        return true;
    }
    return false;
}