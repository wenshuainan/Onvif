#include "DevCapabilities.h"

std::shared_ptr<Capabilities> Capabilities::create()
{
    return std::make_shared<DevCapabilities>();
}

bool DevCapabilities::isIPFilter() const
{
    return false;
}

bool DevCapabilities::isZeroConfiguration() const
{
    return false;
}

bool DevCapabilities::isIPVersion6() const
{
    return false;
}

bool DevCapabilities::isDynDNS() const
{
    return false;
}

bool DevCapabilities::isDot11Configuration() const
{
    return false;
}

int DevCapabilities::getDot1XConfigurations() const
{
    return 0;
}

bool DevCapabilities::isHostnameFromDHCP() const
{
    return false;
}

int DevCapabilities::getNTP() const
{
    return 0;
}

bool DevCapabilities::isDHCPv6() const
{
    return false;
}

/* SecurityCapabilities */
bool DevCapabilities::isTLS1_x002e0() const
{
    return false;
}

bool DevCapabilities::isTLS1_x002e1() const
{
    return false;
}

bool DevCapabilities::isTLS1_x002e2() const
{
    return false;
}

bool DevCapabilities::isOnboardKeyGeneration() const
{
    return false;
}

bool DevCapabilities::isAccessPolicyConfig() const
{
    return false;
}

bool DevCapabilities::isDefaultAccessPolicy() const
{
    return false;
}

bool DevCapabilities::isDot1X() const
{
    return false;
}

bool DevCapabilities::isRemoteUserHandling() const
{
    return false;
}

bool DevCapabilities::isX_x002e509Token() const
{
    return false;
}

bool DevCapabilities::isSAMLToken() const
{
    return false;
}

bool DevCapabilities::isKerberosToken() const
{
    return false;
}

bool DevCapabilities::isUsernameToken() const
{
    return false;
}

bool DevCapabilities::isHttpDigest() const
{
    return false;
}

bool DevCapabilities::isRELToken() const
{
    return false;
}

bool DevCapabilities::isJsonWebToken() const
{
    return false;
}

const char* DevCapabilities::getSupportedEAPMethods() const
{
    return "";
}

int DevCapabilities::getMaxUsers() const
{
    return 0;
}

int DevCapabilities::getMaxUserNameLength() const
{
    return 0;
}

int DevCapabilities::getMaxPasswordLength() const
{
    return 0;
}

const char* DevCapabilities::getSecurityPolicies() const
{
    return "";
}

int DevCapabilities::getMaxPasswordHistory() const
{
    return 0;
}

const char* DevCapabilities::getHashingAlgorithms() const
{
    return "";
}

/* SystemCapabilities */
bool DevCapabilities::isDiscoveryResolve() const
{
    return false;
}

bool DevCapabilities::isDiscoveryBye() const
{
    return false;
}

bool DevCapabilities::isRemoteDiscovery() const
{
    return false;
}

bool DevCapabilities::isSystemBackup() const
{
    return false;
}

bool DevCapabilities::isSystemLogging() const
{
    return false;
}

bool DevCapabilities::isFirmwareUpgrade() const
{
    return false;
}

bool DevCapabilities::isHttpFirmwareUpgrade() const
{
    return false;
}

bool DevCapabilities::isHttpSystemBackup() const
{
    return false;
}

bool DevCapabilities::isHttpSystemLogging() const
{
    return false;
}

bool DevCapabilities::isHttpSupportInformation() const
{
    return false;
}

bool DevCapabilities::isStorageConfiguration() const
{
    return false;
}

int DevCapabilities::getMaxStorageConfigurations() const
{
    return 0;
}

bool DevCapabilities::isStorageConfigurationRenewal() const
{
    return false;
}

int DevCapabilities::getGeoLocationEntries() const
{
    return 0;
}

const char* DevCapabilities::getAutoGeo() const
{
    return "";
}

const char* DevCapabilities::getStorageTypesSupported() const
{
    return "";
}

bool DevCapabilities::isDiscoveryNotSupported() const
{
    return false;
}

bool DevCapabilities::isNetworkConfigNotSupported() const
{
    return false;
}

bool DevCapabilities::isUserConfigNotSupported() const
{
    return false;
}

const char* DevCapabilities::getAddons() const
{
    return "";
}

const char* DevCapabilities::getHardwareType() const
{
    return "";
}

int DevCapabilities::getInputConnectors() const
{
    return 0;
}

int DevCapabilities::getRelayOutputs() const
{
    return 0;
}
