#ifndef ONVIF_MEDIA_H
#define ONVIF_MEDIA_H

#include <memory>
#include <thread>
#include "soapMediaBindingService.h"
#include "Service.h"

class Media : public MediaBindingService, public Service
{
private:
    std::unique_ptr<std::thread> m_thread;
    bool m_bStop;

public:
    Media(Server* server, int port, bool tls = false);
    virtual ~Media();

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
    virtual int GetProfiles(_trt__GetProfiles *trt__GetProfiles, _trt__GetProfilesResponse &trt__GetProfilesResponse);
    virtual int GetStreamUri(_trt__GetStreamUri *trt__GetStreamUri, _trt__GetStreamUriResponse &trt__GetStreamUriResponse);
};

#endif