#ifndef INTERFACE_MEDIA_CONFIGURATION_H
#define INTERFACE_MEDIA_CONFIGURATION_H

#include <memory>
#include <vector>
#include <string>
#include "soapStub.h"

class ConfigurationEntity
{
protected:
    std::string m_Name;
    int m_UseCount;
    std::string m_token;

public:
    ConfigurationEntity()
        : m_UseCount(0)
    {}

public:
    virtual void addCount() { m_UseCount++; }
    virtual void delCount() { m_UseCount--; }
};

class VideoSourceConfiguration : public ConfigurationEntity
{
protected:
    int m_ch;
    std::string m_SourceToken;

public:
    VideoSourceConfiguration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tt__VideoSourceConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__VideoSourceConfiguration *configuration) = 0;
};

class AudioSourceConfiguration : public ConfigurationEntity
{
protected:
    int m_ch;
    std::string m_SourceToken;

public:
    AudioSourceConfiguration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tt__AudioSourceConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__AudioSourceConfiguration *configuration) = 0;
};

class VideoEncoderConfiguration : public ConfigurationEntity
{
protected:
    int m_ch;
    int m_stream;

public:
    VideoEncoderConfiguration(int ch, int stream)
        : m_ch(ch)
        , m_stream(stream)
    {}

public:
    virtual int getChannel() const { return m_ch; }
    virtual int getStream() const { return m_stream; }
    virtual tt__VideoEncoderConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__VideoEncoderConfiguration *configuration) = 0;
};

class AudioEncoderConfiguration : public ConfigurationEntity
{
protected:
    int m_ch;

public:
    AudioEncoderConfiguration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tt__AudioEncoderConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__AudioEncoderConfiguration *configuration) = 0;
};

class VideoAnalyticsConfiguration : public ConfigurationEntity
{
protected:
    int m_ch;

public:
    VideoAnalyticsConfiguration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tt__VideoAnalyticsConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__VideoAnalyticsConfiguration *configuration) = 0;
};

class PTZConfiguration : public ConfigurationEntity
{
protected:
    int m_ch;
    std::string m_NodeToken;

public:
    PTZConfiguration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tt__PTZConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__PTZConfiguration *configuration) = 0;
};

class MetadataConfiguration : public ConfigurationEntity
{
protected:
    int m_ch;

public:
    MetadataConfiguration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tt__MetadataConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__MetadataConfiguration *configuration) = 0;
};

class AudioOutputConfiguration : public ConfigurationEntity
{
protected:
    int m_ch;
    std::string m_OutputToken;

public:
    AudioOutputConfiguration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tt__AudioOutputConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__AudioOutputConfiguration *configuration) = 0;
};

class AudioDecoderConfiguration : public ConfigurationEntity
{
protected:
    int m_ch;

public:
    AudioDecoderConfiguration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tt__AudioDecoderConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__AudioDecoderConfiguration *configuration) = 0;
};

class MediaProfile
{
protected:
    int m_ch;
    int m_stream;
    std::string m_Name;
    std::string m_token;
    bool m_fixed;
    std::shared_ptr<VideoSourceConfiguration> m_videoSource;
    std::shared_ptr<AudioSourceConfiguration> m_audioSource;
    std::shared_ptr<VideoEncoderConfiguration> m_videoEncoder;
    std::shared_ptr<AudioEncoderConfiguration> m_audioEncoder;
    std::shared_ptr<VideoAnalyticsConfiguration> m_videoAnalytics;
    std::shared_ptr<PTZConfiguration> m_ptz;
    std::shared_ptr<MetadataConfiguration> m_metadata;
    std::shared_ptr<AudioOutputConfiguration> m_audioOutput;
    std::shared_ptr<AudioDecoderConfiguration> m_audioDecoder;

public:
    MediaProfile(int ch, int stream, bool fixed = true)
        : m_ch(ch)
        , m_stream(stream)
        , m_fixed(fixed)
    {}

public:
    virtual const std::string& getName() const { return m_Name; }
    virtual const std::string& getToken() const { return m_token; }
    virtual bool isFixed() const { return m_fixed; }
    virtual std::shared_ptr<VideoSourceConfiguration>& getVideoSource() { return m_videoSource; }
    virtual std::shared_ptr<AudioSourceConfiguration>& getAudioSource() { return m_audioSource; }
    virtual std::shared_ptr<VideoEncoderConfiguration>& getVideoEncoder() { return m_videoEncoder; }
    virtual std::shared_ptr<AudioEncoderConfiguration>& getAudioEncoder() { return m_audioEncoder; }
    virtual std::shared_ptr<VideoAnalyticsConfiguration>& getVideoAnalytics() { return m_videoAnalytics; }
    virtual std::shared_ptr<PTZConfiguration>& getPTZ() { return m_ptz; }
    virtual std::shared_ptr<MetadataConfiguration>& getMetadata() { return m_metadata; }
    virtual std::shared_ptr<AudioOutputConfiguration>& getAudioOutput() { return m_audioOutput; }
    virtual std::shared_ptr<AudioDecoderConfiguration>& getAudioDecoder() { return m_audioDecoder; }
};

class MediaConfiguration
{
protected:
    static std::shared_ptr<MediaConfiguration> m_instance;
    std::vector<std::shared_ptr<VideoSourceConfiguration>> m_videoSources;
    std::vector<std::shared_ptr<AudioSourceConfiguration>> m_audioSources;
    std::vector<std::shared_ptr<VideoEncoderConfiguration>> m_videoEncoders;
    std::vector<std::shared_ptr<AudioEncoderConfiguration>> m_audioEncoders;
    std::vector<std::shared_ptr<VideoAnalyticsConfiguration>> m_videoAnalytics;
    std::vector<std::shared_ptr<PTZConfiguration>> m_ptz;
    std::vector<std::shared_ptr<MetadataConfiguration>> m_metadata;
    std::vector<std::shared_ptr<AudioOutputConfiguration>> m_audioOutputs;
    std::vector<std::shared_ptr<AudioDecoderConfiguration>> m_audioDecoders;
    std::vector<std::shared_ptr<MediaProfile>> m_profiles;

public:
    static std::shared_ptr<MediaConfiguration> create();
    virtual const std::vector<std::shared_ptr<MediaProfile>>& getProfiles() const { return m_profiles; }
    virtual std::shared_ptr<MediaProfile> getProfile(const std::string& profileToken) const = 0;
};

#endif