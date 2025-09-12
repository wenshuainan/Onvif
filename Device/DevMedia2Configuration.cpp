#include "DevMedia2Configuration.h"
#include "DevMediaConfiguration.h"

DevVideoEncoder2Configuration::DevVideoEncoder2Configuration(int ch, int stream)
    : VideoEncoder2Configuration(ch, stream)
{
    m_Name = "VideoEncoder2ConfigurationName" + std::to_string(ch) + "_" + std::to_string(stream);
    m_token = "VideoEncoder2ConfigurationToken" + std::to_string(ch) + "_" + std::to_string(stream);
}

tt__VideoEncoder2Configuration* DevVideoEncoder2Configuration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevVideoEncoder2Configuration::setConfiguration(const tt__VideoEncoder2Configuration *configuration)
{
    return false;
}

DevAudioEncoder2Configuration::DevAudioEncoder2Configuration(int ch)
    : AudioEncoder2Configuration(ch)
{
    m_Name = "AudioEncoder2ConfigurationName" + std::to_string(ch);
    m_token = "AudioEncoder2ConfigurationToken" + std::to_string(ch);
}

tt__AudioEncoder2Configuration* DevAudioEncoder2Configuration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevAudioEncoder2Configuration::setConfiguration(const tt__AudioEncoder2Configuration *configuration)
{
    return false;
}

DevReceiver2Configuration::DevReceiver2Configuration(int ch)
    : Receiver2Configuration(ch)
{
    m_token = "Receiver2ConfigurationToken" + std::to_string(ch);
}

tr2__ReceiverConfiguration* DevReceiver2Configuration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevReceiver2Configuration::setConfiguration(const tr2__ReceiverConfiguration *configuration)
{
    return false;
}

DevMedia2Profile::DevMedia2Profile(int ch, int stream, bool fixed)
    : Media2Profile(ch, stream, fixed)
{
    m_Name = "Profile2Name" + std::to_string(ch) + "_" + std::to_string(stream);
    m_token = "Profile2Token" + std::to_string(ch) + "_" + std::to_string(stream);
}

std::shared_ptr<Media2Configuration> Media2Configuration::m_instance;

std::shared_ptr<Media2Configuration> Media2Configuration::create()
{
    if (!m_instance)
    {
        m_instance = std::make_shared<DevMedia2Configuration>();
    }
    return m_instance;
}

DevMedia2Configuration::DevMedia2Configuration()
{
    // 先创建所有Configuration
    // 遍历所有通道创建SourceConfiguration
    for (int ch = 0; ch < 1; ch++)
    {
        m_videoSources.push_back(std::make_shared<DevVideoSourceConfiguration>(ch));
        m_audioSources.push_back(std::make_shared<DevAudioSourceConfiguration>(ch));
        // 遍历通道所有stream创建EncoderConfiguration
        for (int stream = 0; stream < 2; stream++)
        {
            m_videoEncoders.push_back(std::make_shared<DevVideoEncoder2Configuration>(ch, stream));
        }
        m_audioEncoders.push_back(std::make_shared<DevAudioEncoder2Configuration>(ch));
        m_videoAnalytics.push_back(std::make_shared<DevVideoAnalyticsConfiguration>(ch));
        m_ptz.push_back(std::make_shared<DevPTZConfiguration>(ch));
        m_metadata.push_back(std::make_shared<DevMetadataConfiguration>(ch));
        m_receivers.push_back(std::make_shared<DevReceiver2Configuration>(ch));
    }
    // 音频输出和音频解码
    m_audioOutputs.push_back(std::make_shared<DevAudioOutputConfiguration>(0));
    m_audioDecoders.push_back(std::make_shared<DevAudioDecoderConfiguration>(0));
    // 根据Configuration创建Profile
    for (auto& ve : m_videoEncoders)
    {
        int ch = ve->getChannel();
        int stream = ve->getStream();
        m_profiles.push_back(std::make_shared<DevMedia2Profile>(ch, stream));
        m_profiles.back()->getVideoSource() = m_videoSources[ch];
        m_profiles.back()->getVideoEncoder2() = ve;
        m_profiles.back()->getAudioSource() = m_audioSources[ch];
        m_profiles.back()->getAudioEncoder2() = m_audioEncoders[ch];
        m_profiles.back()->getVideoAnalytics() = m_videoAnalytics[ch];
        m_profiles.back()->getPTZ() = m_ptz[ch];
        m_profiles.back()->getMetadata() = m_metadata[ch];
        m_profiles.back()->getAudioOutput() = m_audioOutputs[0];
        m_profiles.back()->getAudioDecoder() = m_audioDecoders[0];
        m_profiles.back()->getReceiver2() = m_receivers[ch];

        m_videoSources[ch]->addCount();
        ve->addCount();
        m_audioSources[ch]->addCount();
        m_audioEncoders[ch]->addCount();
        m_videoAnalytics[ch]->addCount();
        m_ptz[ch]->addCount();
        m_metadata[ch]->addCount();
        m_audioOutputs[0]->addCount();
        m_audioDecoders[0]->addCount();
    }
}

std::shared_ptr<Media2Profile> DevMedia2Configuration::getProfile(const std::string& profileToken) const
{
    for (auto profile : m_profiles)
    {
        if (profile->getToken() == profileToken)
        {
            return profile;
        }
    }
    return nullptr;
}