#ifndef ONVIF_SERVER_H
#define ONVIF_SERVER_H

#include <memory>
#include <vector>
#include <string>
#include "Service/Service.h"

class Server
{
private:
    static Server *m_instance;
    std::vector<std::shared_ptr<Service>> m_services;
    std::string m_guid;

public:
    Server(bool tls = false);
    virtual ~Server();

public:
    static Server* create(bool tls = false);
    static void destroy();

    template<typename T>
    std::shared_ptr<T> getService()
    {
        for (auto& service : m_services)
        {
            std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(service);
            if (ptr)
            {
                return ptr;
            }
        }

        return nullptr;
    }

    const std::vector<std::shared_ptr<Service>>& getServices() const;
    void getMac(soap *soap, unsigned char mac[6]) const;
    const char* getGuid(soap *soap);
};

#endif