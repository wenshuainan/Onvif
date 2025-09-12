#ifndef DEV_CAPABILITIES_H
#define DEV_CAPABILITIES_H

#include "Interface/Capabilities.h"

class DevCapabilities : public Capabilities
{
public:
    /* NetworkCapabilities */
    virtual bool isIPFilter() const;
    virtual bool isZeroConfiguration() const;
    virtual bool isIPVersion6() const;
    virtual bool isDynDNS() const;
    virtual bool isDot11Configuration() const;
    virtual int  getDot1XConfigurations() const;
    virtual bool isHostnameFromDHCP() const;
    virtual int  getNTP() const;
    virtual bool isDHCPv6() const;

    /* SecurityCapabilities */
    virtual bool isTLS1_x002e0() const;
    virtual bool isTLS1_x002e1() const;
    virtual bool isTLS1_x002e2() const;
    virtual bool isOnboardKeyGeneration() const;
    virtual bool isAccessPolicyConfig() const;
    virtual bool isDefaultAccessPolicy() const;
    virtual bool isDot1X() const;
    virtual bool isRemoteUserHandling() const;
    virtual bool isX_x002e509Token() const;
    virtual bool isSAMLToken() const;
    virtual bool isKerberosToken() const;
    virtual bool isUsernameToken() const;
    virtual bool isHttpDigest() const;
    virtual bool isRELToken() const;
    virtual bool isJsonWebToken() const;
    virtual const char* getSupportedEAPMethods() const;
    virtual int  getMaxUsers() const;
    virtual int  getMaxUserNameLength() const;
    virtual int  getMaxPasswordLength() const;
    virtual const char* getSecurityPolicies() const;
    virtual int  getMaxPasswordHistory() const;
    virtual const char* getHashingAlgorithms() const;

    /* SystemCapabilities */
    virtual bool isDiscoveryResolve() const;
    virtual bool isDiscoveryBye() const;
    virtual bool isRemoteDiscovery() const;
    virtual bool isSystemBackup() const;
    virtual bool isSystemLogging() const;
    virtual bool isFirmwareUpgrade() const;
    virtual bool isHttpFirmwareUpgrade() const;
    virtual bool isHttpSystemBackup() const;
    virtual bool isHttpSystemLogging() const;
    virtual bool isHttpSupportInformation() const;
    virtual bool isStorageConfiguration() const;
    virtual int  getMaxStorageConfigurations() const;
    virtual bool isStorageConfigurationRenewal() const;
    virtual int  getGeoLocationEntries() const;
    virtual const char* getAutoGeo() const;
    virtual const char* getStorageTypesSupported() const;
    virtual bool isDiscoveryNotSupported() const;
    virtual bool isNetworkConfigNotSupported() const;
    virtual bool isUserConfigNotSupported() const;
    virtual const char* getAddons() const;
    virtual const char* getHardwareType() const;

    /* IOCapabilities */
    virtual int  getInputConnectors() const;
    virtual int  getRelayOutputs() const;
};

#endif