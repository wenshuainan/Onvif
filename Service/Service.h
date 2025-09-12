#ifndef ONVIF_SERVICE_H
#define ONVIF_SERVICE_H

#include <string>

class Server;
struct soap;

class Service
{
protected:
    Server* m_server;
    int m_port;
    bool m_btls;
    std::string m_XAddr;

public:
    Service(Server* server, int port, bool tls = false);
    virtual ~Service();

protected:
    int verifyHttpDigest(soap *soap);
    int verifyWSS(soap *soap);
    int authenticate(soap *soap);

public:
    virtual const char* getProtocol() const;
    virtual int getPort() const;
    virtual const char* getPath() const { return ""; };
    void getHost(soap *soap, char host[128]);
    virtual const char* getNamespace() const { return ""; };
    virtual const char* getXAddr(soap *soap);
    virtual const char* getVersion() const { return ""; };
    virtual int getVersionMajor(const std::string& version) const
    {
        return version.size() < 2 ? 0 : std::stoi(version.substr(0, 2));
    }
    virtual int getVersionMinor(const std::string& version) const
    {
        return version.size() < 5 ? 0 : std::stoi(version.substr(3, 2));
    }
    virtual bool* makeBool(soap *soap, bool value) const;
    virtual int* makeInt(soap *soap, int value) const;
    virtual std::string* makeString(soap *soap, const std::string& value) const;
};

#endif