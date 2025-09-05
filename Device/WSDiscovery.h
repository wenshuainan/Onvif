#ifndef WS_DISCOVERY_H
#define WS_DISCOVERY_H

#include "Service/soapwsddService.h"

class WSDiscovery : public wsddService
{
public:
    WSDiscovery();
    virtual ~WSDiscovery();

public:
    virtual int run(int port, int backlog = 1);
    virtual int Probe(struct wsdd__ProbeType *wsdd__Probe);
};

#endif