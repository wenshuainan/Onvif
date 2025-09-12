#ifndef INTERFACE_AUTHENTICATION_H
#define INTERFACE_AUTHENTICATION_H

#include <memory>
#include <string>

class Authentication
{
public:
    static std::shared_ptr<Authentication> create();
    virtual bool getUserPasswd(const std::string& name, const std::string& realm, std::string& passwd) = 0;
    virtual bool getUserPasswd(const std::string& name, std::string& passwd) = 0;
};

#endif