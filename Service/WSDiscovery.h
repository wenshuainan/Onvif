#ifndef WS_DISCOVERY_H
#define WS_DISCOVERY_H

#include <memory>
#include <thread>
#include "soapwsddService.h"
#include "Service.h"

class WSDiscovery : public wsddService, public Service
{
private:
    std::unique_ptr<std::thread> m_thread;
    bool m_bStop;

public:
    WSDiscovery(Server *server, int port = 3702);
    virtual ~WSDiscovery();

private:
    void threadProc();

public:
    virtual int run(int port, int backlog = 1);
    virtual int Probe(struct wsdd__ProbeType *wsdd__Probe);
};

#endif