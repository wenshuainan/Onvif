#ifndef DEVICE_MEDIA_CONFIGURATION_H
#define DEVICE_MEDIA_CONFIGURATION_H

#include "Interface/MediaConfiguration.h"

class DevVideoSourceConfiguration : public VideoSourceConfiguration
{
public:
    DevVideoSourceConfiguration(int ch);

public:
    virtual tt__VideoSourceConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__VideoSourceConfiguration *configuration);
};

class DevAudioSourceConfiguration : public AudioSourceConfiguration
{
public:
    DevAudioSourceConfiguration(int ch);

public:
    virtual tt__AudioSourceConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__AudioSourceConfiguration *configuration);
};

class DevVideoEncoderConfiguration : public VideoEncoderConfiguration
{
public:
    DevVideoEncoderConfiguration(int ch, int stream);

public:
    virtual tt__VideoEncoderConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__VideoEncoderConfiguration *configuration);
};

class DevAudioEncoderConfiguration : public AudioEncoderConfiguration
{
public:
    DevAudioEncoderConfiguration(int ch);

public:
    virtual tt__AudioEncoderConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__AudioEncoderConfiguration *configuration);
};

class DevVideoAnalyticsConfiguration : public VideoAnalyticsConfiguration
{
public:
    DevVideoAnalyticsConfiguration(int ch);

public:
    virtual tt__VideoAnalyticsConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__VideoAnalyticsConfiguration *configuration);
};

class DevPTZConfiguration : public PTZConfiguration
{
public:
    DevPTZConfiguration(int ch);

public:
    virtual tt__PTZConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__PTZConfiguration *configuration);
};

class DevMetadataConfiguration : public MetadataConfiguration
{
public:
    DevMetadataConfiguration(int ch);

public:
    virtual tt__MetadataConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__MetadataConfiguration *configuration);
};

class DevAudioOutputConfiguration : public AudioOutputConfiguration
{
public:
    DevAudioOutputConfiguration(int ch);

public:
    virtual tt__AudioOutputConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__AudioOutputConfiguration *configuration);
};

class DevAudioDecoderConfiguration : public AudioDecoderConfiguration
{
public:
    DevAudioDecoderConfiguration(int ch);
    
public:
    virtual tt__AudioDecoderConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__AudioDecoderConfiguration *configuration);
};

class DevMediaProfile : public MediaProfile
{
public:
    DevMediaProfile(int ch, int stream, bool fixed = true);
};

class DevMediaConfiguration : public MediaConfiguration
{
public:
    DevMediaConfiguration();

public:
    virtual std::shared_ptr<MediaProfile> getProfile(const std::string& profileToken) const;
};

#endif