#ifndef INTERFACE_CAPABILITIES_H
#define INTERFACE_CAPABILITIES_H

#include <memory>

class Capabilities
{
public:
    static std::shared_ptr<Capabilities> create();

    /* NetworkCapabilities */
    virtual bool isIPFilter() const = 0;
    virtual bool isZeroConfiguration() const = 0;
    virtual bool isIPVersion6() const = 0;
    virtual bool isDynDNS() const = 0;
    virtual bool isDot11Configuration() const = 0;
    virtual int  getDot1XConfigurations() const = 0;
    virtual bool isHostnameFromDHCP() const = 0;
    virtual int  getNTP() const = 0;
    virtual bool isDHCPv6() const = 0;

    /* SecurityCapabilities */
    virtual bool isTLS1_x002e0() const = 0;
    virtual bool isTLS1_x002e1() const = 0;
    virtual bool isTLS1_x002e2() const = 0;
    virtual bool isOnboardKeyGeneration() const = 0;
    virtual bool isAccessPolicyConfig() const = 0;
    virtual bool isDefaultAccessPolicy() const = 0;
    virtual bool isDot1X() const = 0;
    virtual bool isRemoteUserHandling() const = 0;
    virtual bool isX_x002e509Token() const = 0;
    virtual bool isSAMLToken() const = 0;
    virtual bool isKerberosToken() const = 0;
    virtual bool isUsernameToken() const = 0;
    virtual bool isHttpDigest() const = 0;
    virtual bool isRELToken() const = 0;
    virtual bool isJsonWebToken() const = 0;
    virtual const char* getSupportedEAPMethods() const = 0;
    virtual int  getMaxUsers() const = 0;
    virtual int  getMaxUserNameLength() const = 0;
    virtual int  getMaxPasswordLength() const = 0;
    virtual const char* getSecurityPolicies() const = 0;
    virtual int  getMaxPasswordHistory() const = 0;
    virtual const char* getHashingAlgorithms() const = 0;

    /* SystemCapabilities */
    virtual bool isDiscoveryResolve() const = 0;
    virtual bool isDiscoveryBye() const = 0;
    virtual bool isRemoteDiscovery() const = 0;
    virtual bool isSystemBackup() const = 0;
    virtual bool isSystemLogging() const = 0;
    virtual bool isFirmwareUpgrade() const = 0;
    virtual bool isHttpFirmwareUpgrade() const = 0;
    virtual bool isHttpSystemBackup() const = 0;
    virtual bool isHttpSystemLogging() const = 0;
    virtual bool isHttpSupportInformation() const = 0;
    virtual bool isStorageConfiguration() const = 0;
    virtual int  getMaxStorageConfigurations() const = 0;
    virtual bool isStorageConfigurationRenewal() const = 0;
    virtual int  getGeoLocationEntries() const = 0;
    virtual const char* getAutoGeo() const = 0;
    virtual const char* getStorageTypesSupported() const = 0;
    virtual bool isDiscoveryNotSupported() const = 0;
    virtual bool isNetworkConfigNotSupported() const = 0;
    virtual bool isUserConfigNotSupported() const = 0;
    virtual const char* getAddons() const = 0;
    virtual const char* getHardwareType() const = 0;

    /* IOCapabilities */
    virtual int  getInputConnectors() const = 0;
    virtual int  getRelayOutputs() const = 0;
};

#endif