#ifndef ONVIF_MEDIA2_H
#define ONVIF_MEDIA2_H

#include <memory>
#include <thread>
#include "soapMedia2BindingService.h"
#include "Service.h"

class Media2 : public Media2BindingService, public Service
{
private:
    std::unique_ptr<std::thread> m_thread;
    bool m_bStop;

public:
    Media2(Server* server, int port, bool tls = false);
    virtual ~Media2();

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
    virtual int GetProfiles(_tr2__GetProfiles *tr2__GetProfiles, _tr2__GetProfilesResponse &tr2__GetProfilesResponse);
};

#endif