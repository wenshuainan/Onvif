#ifndef ONVIF_PTZ_H
#define ONVIF_PTZ_H

#include <memory>
#include <thread>
#include "soapPTZBindingService.h"
#include "Service.h"

class PTZ : public PTZBindingService, public Service
{
private:
    std::unique_ptr<std::thread> m_thread;
    bool m_bStop;

public:
    PTZ(Server* server, int port, bool tls = false);
    virtual ~PTZ();

private:
    void threadProc();

public:
    virtual const char* getPath() const;
    virtual const char* getNamespace() const;
    virtual const char* getVersion() const;

public:
    virtual int run(int port, int backlog = 1);
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
    virtual int ssl_run(int port, int backlog = 1);
#endif
    virtual int dispatch();
};

#endif