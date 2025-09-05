#include "WSDiscovery.h"

WSDiscovery::WSDiscovery()
    : wsddService(SOAP_IO_UDP | SOAP_XML_IGNORENS)
{}

WSDiscovery::~WSDiscovery()
{}

int WSDiscovery::run(int port, int backlog)
{	if (!soap_valid_socket(this->soap->master) && !soap_valid_socket(this->bind(NULL, port, backlog)))
		return this->soap->error;
    
    // 加入组播组
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250"); // 组播地址
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);           // 使用默认网络接口
    if (setsockopt(this->soap->master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt IP_ADD_MEMBERSHIP failed");
        return this->soap->error = SOAP_UDP_ERROR;
    }

	for (;;)
	{	if (!soap_valid_socket(this->accept()))
		{	if (this->soap->errnum == 0) // timeout?
				this->soap->error = SOAP_OK;
			break;
		}
		if (this->serve())
			break;
		this->destroy();
	}
	return this->soap->error;
}

int WSDiscovery::Probe(struct wsdd__ProbeType *wsdd__Probe)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Probe\n");
    return SOAP_OK;
}