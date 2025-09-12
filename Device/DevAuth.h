#ifndef ONVIF_DEV_AUTH_H
#define ONVIF_DEV_AUTH_H

#include "Interface/Authentication.h"

class DevAuth : public Authentication
{
public:
    virtual bool getUserPasswd(const std::string& name, const std::string& realm, std::string& passwd);
    virtual bool getUserPasswd(const std::string& name, std::string& passwd);
};

#endif