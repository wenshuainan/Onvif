#include "WSDiscovery.h"
#include "wsddapi.h"
#include "wsaapi.h"
#include "Server.h"
#include "Device.h"

WSDiscovery::WSDiscovery(Server *server, int port)
    : wsddService(SOAP_IO_UDP | SOAP_XML_IGNORENS)
    , Service(server, port)
{
    printf("++++++ WSDiscovery\n");
    m_bStop = false;
    m_thread = std::unique_ptr<std::thread>(new std::thread(&WSDiscovery::threadProc, this));
}

WSDiscovery::~WSDiscovery()
{
    printf("------ WSDiscovery\n");
    m_bStop = true;
    printf("wait WSDiscovery thread join\n");
    soap->omode &= ~SOAP_IO_UDP; /* to force close the socket */
    (void)soap_closesock(soap);
    m_thread->join();
    printf("WSDiscovery thread joined\n");
}

void WSDiscovery::threadProc()
{
    while (!m_bStop)
    {
        run(m_port);
        printf("WSDiscovery run sopted\n");
    }
    printf("WSDiscovery threadProc exit\n");
}

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
		if (this->serve());
			// break;
		this->destroy();
	}
	return this->soap->error;
}

int WSDiscovery::Probe(struct wsdd__ProbeType *wsdd__Probe)
{
    std::shared_ptr<Device> device = m_server->getService<Device>();
    struct wsdd__ProbeMatchesType matches;
    soap_wsdd_init_ProbeMatches(soap, &matches);

    // repeat this to add multiple matches:
    if (soap_wsdd_add_ProbeMatch(soap, &matches
            , m_server->getGuid(soap)
            , "tds:Device"
            , "onvif://www.onvif.org/Profile/Streaming"
            , NULL
            , device ? device->getXAddr(soap) : NULL
            , 0))
    {
        printf("soap_wsdd_add_ProbeMatch out of memory\n");
        return soap->error;
    }

    // send the ProbeMatches message
    if (soap_wsdd_ProbeMatches(soap
            , ""
            , soap_wsa_rand_uuid(soap)
            , soap->header ? soap->header->wsa__MessageID : NULL
            , NULL
            , &matches))
    {
        printf("soap_wsdd_ProbeMatches() failed: %d\n", soap->error);
        return soap->error;
    }

    return SOAP_OK;
}
