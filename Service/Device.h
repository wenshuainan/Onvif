#ifndef ONVIF_DEVICE_H
#define ONVIF_DEVICE_H

#include <memory>
#include <thread>
#include "soapDeviceBindingService.h"
#include "Service.h"

class Device : public DeviceBindingService, public Service
{
private:
    std::unique_ptr<std::thread> m_thread;
    bool m_bStop;

public:
    Device(Server* server, int port, bool tls = false);
    virtual ~Device();

private:
    void threadProc();

public:
    virtual const char* getPath() const;
    virtual const char* getNamespace() const;
    virtual const char* getVersion() const;

public:
    virtual int run(int port, int backlog = 1);
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
    /// Run simple single-thread SSL service on port until a connection error occurs (returns SOAP_OK or error code), use this->bind_flag = SO_REUSEADDR to rebind for immediate rerun
    virtual int ssl_run(int port, int backlog = 1);
#endif
    virtual int dispatch();
    virtual int GetServices(_tds__GetServices *tds__GetServices, _tds__GetServicesResponse &tds__GetServicesResponse);
    virtual int GetServiceCapabilities(_tds__GetServiceCapabilities *tds__GetServiceCapabilities, _tds__GetServiceCapabilitiesResponse &tds__GetServiceCapabilitiesResponse);
    virtual int GetDeviceInformation(_tds__GetDeviceInformation *tds__GetDeviceInformation, _tds__GetDeviceInformationResponse &tds__GetDeviceInformationResponse);
    virtual int GetCapabilities(_tds__GetCapabilities *tds__GetCapabilities, _tds__GetCapabilitiesResponse &tds__GetCapabilitiesResponse);
};

#endif