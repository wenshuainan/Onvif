#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include "Service.h"
#include "stdsoap2.h"
#include "httpda.h"
#include "wsseapi.h"
#include "Interface/Authentication.h"

Service::Service(Server* server, int port, bool tls)
    : m_server(server)
    , m_port(port)
    , m_btls(tls)
{}

Service::~Service()
{}

int Service::verifyHttpDigest(soap *soap)
{
    const char *authrealm = soap->authrealm ? soap->authrealm : "";
    const char *userid = soap->userid ? soap->userid : "";
    if (soap->authrealm && soap->userid) // Digest authentication
    {
        std::shared_ptr<Authentication> auth = Authentication::create();
        std::string passwd;
        auth->getUserPasswd(userid, authrealm, passwd); // database lookup on userid and authrealm to find passwd
        if (!strcmp(soap->authrealm, authrealm) && !strcmp(soap->userid, userid))
        { 
            if (!http_da_verify_post(soap, passwd.c_str())) // HTTP POST DA verification
            {
                printf("http digest verify succeed realm:(%s) userid:(%s)\n", authrealm, userid);
                return SOAP_OK;
            }
        }
    }
    printf("http digest verify failed realm:(%s) userid:(%s)\n", authrealm, userid);
    soap->authrealm = authrealm; // realm to send to client
    return 401; // Not authorized, challenge with digest authentication
}

int Service::verifyWSS(soap *soap)
{
    const char *username = soap_wsse_get_Username(soap);
    if (!username)
    {
        soap_wsse_delete_Security(soap); // remove old security headers, but keep other headers
        return soap->error; // no username: return FailedAuthentication (from soap_wsse_get_Username)
    }
    std::shared_ptr<Authentication> auth = Authentication::create();
    std::string password;
    auth->getUserPasswd(username, password); 
    if (soap_wsse_verify_Password(soap, password.c_str()))
    {
        printf("WSS verify failed userid:(%s)\n", username);
        int err = soap->error;
        soap_wsse_delete_Security(soap); // remove old security headers, but keep other headers
        return err; // password verification failed: return FailedAuthentication
    }
    printf("WSS verify succeed userid:(%s)\n", username);
    soap_wsse_delete_Security(soap);
    return SOAP_OK;
}

/* ONVIF Core Specification: 5.9 Security
 * Figure 4 summarizes the authentication of a web service request by a server over HTTP and HTTPS.
 */
int Service::authenticate(soap *soap)
{
    int err = SOAP_OK;

    if (soap->authrealm && soap->userid)
    {
        err = verifyHttpDigest(soap);
    }

    if (err == SOAP_OK)
    {
        if (soap->header && soap->header->wsse__Security)
        {
            err = verifyWSS(soap);
        }
        else
        {
            err = 401;
        }
    }

    return err;
}

const char* Service::getProtocol() const
{
    return m_btls ? "https" : "http";
}

int Service::getPort() const
{
    return m_port;
}

void Service::getHost(soap *soap, char host[128])
{
    struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in addrReq;
    int sockfd;

    char hostReq[128], portReq[16];
    getnameinfo((struct sockaddr*)&soap->peer, soap->peerlen
        , hostReq, sizeof(hostReq), portReq, sizeof(portReq)
        , NI_NUMERICHOST | NI_NUMERICSERV);

    inet_pton(AF_INET, hostReq, &(addrReq.sin_addr));

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        return;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        freeifaddrs(ifaddr);
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
        {
            continue;
        }

        struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
        struct sockaddr_in *netmask = (struct sockaddr_in *)ifa->ifa_netmask;

        // 检查接口是否与目标 IP 在同一子网
        if ((addr->sin_addr.s_addr & netmask->sin_addr.s_addr) ==
            (addrReq.sin_addr.s_addr & netmask->sin_addr.s_addr))
        {
            memcpy(host, inet_ntoa(addr->sin_addr), 16);
            printf("Interface: %s\n", ifa->ifa_name);
            printf("IP Address: %s\n", host);
            break;
        }
    }

    close(sockfd);
    freeifaddrs(ifaddr);
}

const char* Service::getXAddr(soap *soap)
{
    char host[128];
    getHost(soap, host);
    m_XAddr = std::string(getProtocol()) + "://" + host + ":" + std::to_string(getPort()) + getPath();
    return m_XAddr.c_str();
}

bool* Service::make_bool(soap *soap, bool value) const
{
    bool *b = soap_new_bool(soap);
    if (b)
    {
        *b = value;
    }
    return b;
}

int* Service::make_int(soap *soap, int value) const
{
    int *i = soap_new_int(soap);
    if (i)
    {
        *i = value;
    }
    return i;
}

std::string* Service::make_string(soap *soap, const std::string& value) const
{
    std::string *s = soap_new_std__string(soap);
    if (s)
    {
        *s = value;
    }
    return s;
}