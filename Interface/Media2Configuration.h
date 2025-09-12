#ifndef INTERFACE_MEDIA2_CONFIGURATION_H
#define INTERFACE_MEDIA2_CONFIGURATION_H

#include "MediaConfiguration.h"

class VideoEncoder2Configuration : public ConfigurationEntity
{
protected:
    int m_ch;
    int m_stream;

public:
    VideoEncoder2Configuration(int ch, int stream)
        : m_ch(ch)
        , m_stream(stream)
    {}

public:
    virtual int getChannel() const { return m_ch; }
    virtual int getStream() const { return m_stream; }
    virtual tt__VideoEncoder2Configuration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__VideoEncoder2Configuration *configuration) = 0;
};

class AudioEncoder2Configuration : public ConfigurationEntity
{
protected:
    int m_ch;

public:
    AudioEncoder2Configuration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tt__AudioEncoder2Configuration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tt__AudioEncoder2Configuration *configuration) = 0;
};

class Receiver2Configuration
{
protected:
    int m_ch;
    std::string m_token;

public:
    Receiver2Configuration(int ch)
        : m_ch(ch)
    {}

public:
    virtual tr2__ReceiverConfiguration* getConfiguration(soap *soap) const = 0;
    virtual bool setConfiguration(const tr2__ReceiverConfiguration *configuration) = 0;
};

class Media2Profile : public MediaProfile
{
protected:
    std::shared_ptr<VideoEncoder2Configuration> m_videoEncoder2;
    std::shared_ptr<AudioEncoder2Configuration> m_audioEncoder2;
    std::shared_ptr<Receiver2Configuration> m_receiver2;

public:
    Media2Profile(int ch, int stream, bool fixed = true)
        : MediaProfile(ch, stream, fixed)
    {}

public:
    virtual std::shared_ptr<VideoEncoder2Configuration>& getVideoEncoder2() { return m_videoEncoder2; }
    virtual std::shared_ptr<AudioEncoder2Configuration>& getAudioEncoder2() { return m_audioEncoder2; }
    virtual std::shared_ptr<Receiver2Configuration>& getReceiver2() { return m_receiver2; }
};

class Media2Configuration
{
protected:
    static std::shared_ptr<Media2Configuration> m_instance;
    std::vector<std::shared_ptr<VideoSourceConfiguration>> m_videoSources;
    std::vector<std::shared_ptr<AudioSourceConfiguration>> m_audioSources;
    std::vector<std::shared_ptr<VideoEncoder2Configuration>> m_videoEncoders;
    std::vector<std::shared_ptr<AudioEncoder2Configuration>> m_audioEncoders;
    std::vector<std::shared_ptr<VideoAnalyticsConfiguration>> m_videoAnalytics;
    std::vector<std::shared_ptr<PTZConfiguration>> m_ptz;
    std::vector<std::shared_ptr<MetadataConfiguration>> m_metadata;
    std::vector<std::shared_ptr<AudioOutputConfiguration>> m_audioOutputs;
    std::vector<std::shared_ptr<AudioDecoderConfiguration>> m_audioDecoders;
    std::vector<std::shared_ptr<Receiver2Configuration>> m_receivers;
    std::vector<std::shared_ptr<Media2Profile>> m_profiles;

public:
    static std::shared_ptr<Media2Configuration> create();
    virtual const std::vector<std::shared_ptr<Media2Profile>>& getProfiles() const { return m_profiles; }
    virtual std::shared_ptr<Media2Profile> getProfile(const std::string& profileToken) const = 0;
};

#endif