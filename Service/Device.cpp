#include "Device.h"
#include "httpda.h"
#include "Server.h"
#include "WSDiscovery.h"
#include "Media.h"
#include "PTZ.h"
#include "Interface/Capabilities.h"
#include "Interface/Information.h"

Device::Device(Server* server, int port, bool tls)
    : Service(server, port, tls)
{
    printf("++++++ Device\n");
    this->soap->bind_flags = SO_REUSEADDR;
    soap_register_plugin(this->soap, http_da);
    m_bStop = false;
    m_thread = std::unique_ptr<std::thread>(new std::thread(&Device::threadProc, this));
}

Device::~Device()
{
    printf("------ Device\n");
    m_bStop = true;
    printf("wait Device thread join\n");
    shutdown(this->soap->master, SHUT_RDWR);
    m_thread->join();
    printf("Device thread joined\n");
}

void Device::threadProc()
{
    while (!m_bStop)
    {
#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
        if (m_btls)
        {
            ssl_run(m_port);
        }
        else
#endif
        {
            run(m_port);
        }
        printf("Device (ssl_)run sopted\n");
    }
    printf("Device threadProc exit\n");
}

const char* Device::getPath() const
{
    return "/onvif/device_service";
}

const char* Device::getNamespace() const
{
    return "http://www.onvif.org/ver10/device/wsdl";
}

const char* Device::getVersion() const
{
#ifdef ONVIF_CORE_SPECIFICATION_VERSION
    return ONVIF_CORE_SPECIFICATION_VERSION;
#else
    return "";
#endif
}

int Device::run(int port, int backlog)
{	if (!soap_valid_socket(this->soap->master) && !soap_valid_socket(this->bind(NULL, port, backlog)))
		return this->soap->error;
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

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
int Device::ssl_run(int port, int backlog)
{	if (!soap_valid_socket(this->soap->master) && !soap_valid_socket(this->bind(NULL, port, backlog)))
		return this->soap->error;
	for (;;)
	{	if (!soap_valid_socket(this->accept()))
		{	if (this->soap->errnum == 0) // timeout?
				this->soap->error = SOAP_OK;
			break;
		}
		if (this->ssl_accept() || this->serve());
			// break;
		this->destroy();
	}
	return this->soap->error;
}
#endif

int Device::dispatch()
{
    int err = authenticate(this->soap);
    if (err != SOAP_OK)
    {
        return err;
    }

    return DeviceBindingService::dispatch();
}

int Device::GetServices(_tds__GetServices *tds__GetServices, _tds__GetServicesResponse &tds__GetServicesResponse)
{
    std::vector<std::shared_ptr<Service>> services = m_server->getServices();
    for (auto service : services)
    {
        if (std::dynamic_pointer_cast<WSDiscovery>(service))
        {
            continue;
        }
        tds__Service *item = soap_new_tds__Service(tds__GetServicesResponse.soap);
        if (!item)
        {
            return SOAP_EOM;
        }
        item->Namespace = service->getNamespace();
        item->XAddr = service->getXAddr(tds__GetServicesResponse.soap);
        item->Version = soap_new_tt__OnvifVersion(tds__GetServicesResponse.soap);
        if (item->Version)
        {
            item->Version->Major = getVersionMajor(service->getVersion());
            item->Version->Minor = getVersionMinor(service->getVersion());
        }
        tds__GetServicesResponse.Service.push_back(item);
    }
    return SOAP_OK;
}

int Device::GetServiceCapabilities(_tds__GetServiceCapabilities *tds__GetServiceCapabilities, _tds__GetServiceCapabilitiesResponse &tds__GetServiceCapabilitiesResponse)
{
    std::shared_ptr<Capabilities> caps = Capabilities::create();
    if (!caps)
    {
        return SOAP_EOM;
    }

    tds__DeviceServiceCapabilities *capabilities = soap_new_tds__DeviceServiceCapabilities(tds__GetServiceCapabilitiesResponse.soap);
    if (!capabilities)
    {
        return SOAP_EOM;
    }

    capabilities->Network = soap_new_tds__NetworkCapabilities(tds__GetServiceCapabilitiesResponse.soap);
    if (capabilities->Network)
    {
        capabilities->Network->IPFilter = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isIPFilter());
        capabilities->Network->ZeroConfiguration = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isZeroConfiguration());
        capabilities->Network->IPVersion6 = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isIPVersion6());
        capabilities->Network->DynDNS = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isDynDNS());
        capabilities->Network->Dot11Configuration = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isDot11Configuration());
        capabilities->Network->Dot1XConfigurations = makeInt(tds__GetServiceCapabilitiesResponse.soap, caps->getDot1XConfigurations());
        capabilities->Network->HostnameFromDHCP = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isHostnameFromDHCP());
        capabilities->Network->NTP = makeInt(tds__GetServiceCapabilitiesResponse.soap, caps->getNTP());
        capabilities->Network->DHCPv6 = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isDHCPv6());
    }
    else
    {
        return SOAP_EOM;
    }

    capabilities->Security = soap_new_tds__SecurityCapabilities(tds__GetServiceCapabilitiesResponse.soap);
    if (capabilities->Security)
    {
        capabilities->Security->TLS1_x002e0 = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isTLS1_x002e0());
        capabilities->Security->TLS1_x002e1 = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isTLS1_x002e1());
        capabilities->Security->TLS1_x002e2 = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isTLS1_x002e2());
        capabilities->Security->OnboardKeyGeneration = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isOnboardKeyGeneration());
        capabilities->Security->AccessPolicyConfig = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isAccessPolicyConfig());
        capabilities->Security->DefaultAccessPolicy = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isDefaultAccessPolicy());
        capabilities->Security->Dot1X = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isDot1X());
        capabilities->Security->RemoteUserHandling = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isRemoteUserHandling());
        capabilities->Security->X_x002e509Token = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isX_x002e509Token());
        capabilities->Security->SAMLToken = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isSAMLToken());
        capabilities->Security->KerberosToken = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isKerberosToken());
        capabilities->Security->UsernameToken = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isUsernameToken());
        capabilities->Security->HttpDigest = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isHttpDigest());
        capabilities->Security->RELToken = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isRELToken());
        capabilities->Security->JsonWebToken = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isJsonWebToken());
        capabilities->Security->SupportedEAPMethods = makeString(tds__GetServiceCapabilitiesResponse.soap, caps->getSupportedEAPMethods());
        capabilities->Security->MaxUsers = makeInt(tds__GetServiceCapabilitiesResponse.soap, caps->getMaxUsers());
        capabilities->Security->MaxUserNameLength = makeInt(tds__GetServiceCapabilitiesResponse.soap, caps->getMaxUserNameLength());
        capabilities->Security->MaxPasswordLength = makeInt(tds__GetServiceCapabilitiesResponse.soap, caps->getMaxPasswordLength());
        capabilities->Security->SecurityPolicies = makeString(tds__GetServiceCapabilitiesResponse.soap, caps->getSecurityPolicies());
        capabilities->Security->MaxPasswordHistory = makeInt(tds__GetServiceCapabilitiesResponse.soap, caps->getMaxPasswordHistory());
        capabilities->Security->HashingAlgorithms = makeString(tds__GetServiceCapabilitiesResponse.soap, caps->getHashingAlgorithms());
    }
    else
    {
        return SOAP_EOM;
    }

    capabilities->System = soap_new_tds__SystemCapabilities(tds__GetServiceCapabilitiesResponse.soap);
    if (capabilities->System)
    {
        capabilities->System->DiscoveryResolve = makeBool(tds__GetServiceCapabilitiesResponse.soap, false);
        capabilities->System->DiscoveryBye = makeBool(tds__GetServiceCapabilitiesResponse.soap, false);
        capabilities->System->RemoteDiscovery = makeBool(tds__GetServiceCapabilitiesResponse.soap, false);
        capabilities->System->SystemBackup = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isSystemBackup());
        capabilities->System->SystemLogging = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isSystemLogging());
        capabilities->System->FirmwareUpgrade = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isFirmwareUpgrade());
        capabilities->System->HttpFirmwareUpgrade = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isHttpFirmwareUpgrade());
        capabilities->System->HttpSystemBackup = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isHttpSystemBackup());
        capabilities->System->HttpSystemLogging = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isHttpSystemLogging());
        capabilities->System->HttpSupportInformation = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isHttpSupportInformation());
        capabilities->System->StorageConfiguration = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isStorageConfiguration());
        capabilities->System->MaxStorageConfigurations = makeInt(tds__GetServiceCapabilitiesResponse.soap, caps->getMaxStorageConfigurations());
        capabilities->System->StorageConfigurationRenewal = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isStorageConfigurationRenewal());
        capabilities->System->GeoLocationEntries = makeInt(tds__GetServiceCapabilitiesResponse.soap, caps->getGeoLocationEntries());
        capabilities->System->AutoGeo = makeString(tds__GetServiceCapabilitiesResponse.soap, caps->getAutoGeo());
        capabilities->System->StorageTypesSupported = makeString(tds__GetServiceCapabilitiesResponse.soap, caps->getStorageTypesSupported());
        capabilities->System->DiscoveryNotSupported = makeBool(tds__GetServiceCapabilitiesResponse.soap, false);
        capabilities->System->NetworkConfigNotSupported = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isNetworkConfigNotSupported());
        capabilities->System->UserConfigNotSupported = makeBool(tds__GetServiceCapabilitiesResponse.soap, caps->isUserConfigNotSupported());
        capabilities->System->Addons = makeString(tds__GetServiceCapabilitiesResponse.soap, caps->getAddons());
        capabilities->System->HardwareType = makeString(tds__GetServiceCapabilitiesResponse.soap, caps->getHardwareType());
    }
    else
    {
        return SOAP_EOM;
    }

    tds__GetServiceCapabilitiesResponse.Capabilities = capabilities;
    return SOAP_OK;
}

int Device::GetDeviceInformation(_tds__GetDeviceInformation *tds__GetDeviceInformation, _tds__GetDeviceInformationResponse &tds__GetDeviceInformationResponse)
{
    std::shared_ptr<Information> info = Information::create();
    if (!info)
    {
        return SOAP_EOM;
    }

    tds__GetDeviceInformationResponse.Manufacturer = info->getManufacturer();
    tds__GetDeviceInformationResponse.Model = info->getModel();
    tds__GetDeviceInformationResponse.FirmwareVersion = info->getFirmwareVersion();
    tds__GetDeviceInformationResponse.SerialNumber = info->getSerialNumber();
    tds__GetDeviceInformationResponse.HardwareId = info->getHardwareId();

    return SOAP_OK;
}

int Device::GetCapabilities(_tds__GetCapabilities *tds__GetCapabilities, _tds__GetCapabilitiesResponse &tds__GetCapabilitiesResponse)
{
    tt__Capabilities *capabilities = soap_new_tt__Capabilities(tds__GetCapabilitiesResponse.soap);
    if (!capabilities)
    {
        return SOAP_EOM;
    }

    std::vector<std::shared_ptr<Service>> services = m_server->getServices();
    for (auto service : services)
    {
        if (std::dynamic_pointer_cast<Device>(service))
        {
            capabilities->Device = soap_new_tt__DeviceCapabilities(tds__GetCapabilitiesResponse.soap);
            if (capabilities->Device)
            {
                capabilities->Device->XAddr = service->getXAddr(tds__GetCapabilitiesResponse.soap);

                std::shared_ptr<Capabilities> caps = Capabilities::create();
                if (!caps)
                {
                    continue;
                }

                capabilities->Device->Network = soap_new_tt__NetworkCapabilities(tds__GetCapabilitiesResponse.soap);
                if (capabilities->Device->Network)
                {
                    capabilities->Device->Network->IPFilter = makeBool(tds__GetCapabilitiesResponse.soap, caps->isIPFilter());
                    capabilities->Device->Network->ZeroConfiguration = makeBool(tds__GetCapabilitiesResponse.soap, caps->isZeroConfiguration());
                    capabilities->Device->Network->IPVersion6 = makeBool(tds__GetCapabilitiesResponse.soap, caps->isIPVersion6());
                    capabilities->Device->Network->DynDNS = makeBool(tds__GetCapabilitiesResponse.soap, caps->isDynDNS());
                }

                capabilities->Device->System = soap_new_tt__SystemCapabilities(tds__GetCapabilitiesResponse.soap);
                if (capabilities->Device->System)
                {
                    capabilities->Device->System->DiscoveryResolve = false;
                    capabilities->Device->System->DiscoveryBye = false;
                    capabilities->Device->System->RemoteDiscovery = false;
                    capabilities->Device->System->SystemBackup = caps->isSystemBackup();
                    capabilities->Device->System->SystemLogging = caps->isSystemLogging();
                    capabilities->Device->System->FirmwareUpgrade = caps->isFirmwareUpgrade();
                    tt__OnvifVersion *version = soap_new_tt__OnvifVersion(tds__GetCapabilitiesResponse.soap);
                    if (version)
                    {
                        version->Major = getVersionMajor(service->getVersion());
                        version->Minor = getVersionMinor(service->getVersion());
                        capabilities->Device->System->SupportedVersions.push_back(version);
                    }
                    else
                    {
                        return SOAP_EOM;
                    }
                }

                capabilities->Device->IO = soap_new_tt__IOCapabilities(tds__GetCapabilitiesResponse.soap);
                if (capabilities->Device->IO)
                {
                    capabilities->Device->IO->InputConnectors = makeInt(tds__GetCapabilitiesResponse.soap, caps->getInputConnectors());
                    capabilities->Device->IO->RelayOutputs = makeInt(tds__GetCapabilitiesResponse.soap, caps->getRelayOutputs());
                }

                capabilities->Device->Security = soap_new_tt__SecurityCapabilities(tds__GetCapabilitiesResponse.soap);
                if (capabilities->Device->Security)
                {
                    capabilities->Device->Security->TLS1_x002e1 = caps->isTLS1_x002e1();
                    capabilities->Device->Security->TLS1_x002e2 = caps->isTLS1_x002e2();
                    capabilities->Device->Security->OnboardKeyGeneration = caps->isOnboardKeyGeneration();
                    capabilities->Device->Security->AccessPolicyConfig = caps->isAccessPolicyConfig();
                    capabilities->Device->Security->X_x002e509Token = caps->isX_x002e509Token();
                    capabilities->Device->Security->SAMLToken = caps->isSAMLToken();
                    capabilities->Device->Security->KerberosToken = caps->isKerberosToken();
                    capabilities->Device->Security->RELToken = caps->isRELToken();
                }
            }
        }
        else if (std::dynamic_pointer_cast<Media>(service))
        {
            capabilities->Media = soap_new_tt__MediaCapabilities(tds__GetCapabilitiesResponse.soap);
            if (capabilities->Media)
            {
                capabilities->Media->XAddr = service->getXAddr(tds__GetCapabilitiesResponse.soap);

                capabilities->Media->StreamingCapabilities = soap_new_tt__RealTimeStreamingCapabilities(tds__GetCapabilitiesResponse.soap);
                if (capabilities->Media->StreamingCapabilities)
                {
                    capabilities->Media->StreamingCapabilities->RTPMulticast = makeBool(tds__GetCapabilitiesResponse.soap, true);
                    capabilities->Media->StreamingCapabilities->RTP_USCORETCP = makeBool(tds__GetCapabilitiesResponse.soap, true);
                    capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = makeBool(tds__GetCapabilitiesResponse.soap, true);
                }
                else
                {
                    return SOAP_EOM;
                }
            }
        }
        else if (std::dynamic_pointer_cast<PTZ>(service))
        {
            capabilities->PTZ = soap_new_tt__PTZCapabilities(tds__GetCapabilitiesResponse.soap);
            if (capabilities->PTZ)
            {
                capabilities->PTZ->XAddr = service->getXAddr(tds__GetCapabilitiesResponse.soap);
            }
        }
    }

    tds__GetCapabilitiesResponse.Capabilities = capabilities;
    return SOAP_OK;
}
