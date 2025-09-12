#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include "Server.h"
#include "openssl/md5.h"
#include "Service/WSDiscovery.h"
#include "Service/Device.h"
#include "Service/Media.h"
#include "Service/Media2.h"
#include "Service/PTZ.h"

Server* Server::m_instance = nullptr;

Server::Server(bool tls)
{
    printf("++++++ Server\n");
    m_services.push_back(std::make_shared<WSDiscovery>(this));
    m_services.push_back(std::make_shared<Device>(this, 8080, tls));
    m_services.push_back(std::make_shared<Media>(this, 8081, tls));
    m_services.push_back(std::make_shared<Media2>(this, 8082, tls));
    m_services.push_back(std::make_shared<PTZ>(this, 8083, tls));
}

Server::~Server()
{
    printf("------ Server\n");
}

Server* Server::create(bool tls)
{
    if (!m_instance)
    {
        m_instance = new Server(tls);
    }

    return m_instance;
}

void Server::destroy()
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

const std::vector<std::shared_ptr<Service>>& Server::getServices() const
{
    return m_services;
}

void Server::getMac(soap *soap, unsigned char mac[6]) const
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
            struct ifreq ifr;
            strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ - 1);

            // 获取 MAC 地址
            if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == 0)
            {
                memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
                printf("Interface: %s\n", ifa->ifa_name);
                printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                       mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            }
            break;
        }
    }

    close(sockfd);
    freeifaddrs(ifaddr);
}

const char* Server::getGuid(soap *soap)
{
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX md5_ctx;

    unsigned char mac[6];
    getMac(soap, mac);

    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, mac, strlen((char*)mac));
    MD5_Final(hash, &md5_ctx);

    m_guid.resize(37);
    snprintf(&m_guid[0], 37, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
             hash[0], hash[1], hash[2], hash[3],
             hash[4], hash[5], hash[6], hash[7],
             hash[8], hash[9], hash[10], hash[11],
             hash[12], hash[13], hash[14], hash[15]);
    return m_guid.c_str();
}

#include "DeviceBinding.nsmap"