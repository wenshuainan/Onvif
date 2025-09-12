#include "DevMediaConfiguration.h"

DevVideoSourceConfiguration::DevVideoSourceConfiguration(int ch)
    : VideoSourceConfiguration(ch)
{
    m_Name = "VideoSourceConfigurationName" + std::to_string(ch);
    m_token = "VideoSourceConfigurationToken" + std::to_string(ch);
    m_SourceToken = "VideoSourceToken" + std::to_string(ch);
}

tt__VideoSourceConfiguration* DevVideoSourceConfiguration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevVideoSourceConfiguration::setConfiguration(const tt__VideoSourceConfiguration *configuration)
{
    return false;
}

DevAudioSourceConfiguration::DevAudioSourceConfiguration(int ch)
    : AudioSourceConfiguration(ch)
{
    m_Name = "AudioSourceConfigurationName" + std::to_string(ch);
    m_token = "AudioSourceConfigurationToken" + std::to_string(ch);
    m_SourceToken = "AudioSourceToken" + std::to_string(ch);
}

tt__AudioSourceConfiguration* DevAudioSourceConfiguration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevAudioSourceConfiguration::setConfiguration(const tt__AudioSourceConfiguration *configuration)
{
    return false;
}

DevVideoEncoderConfiguration::DevVideoEncoderConfiguration(int ch, int stream)
    : VideoEncoderConfiguration(ch, stream)
{
    m_Name = "VideoEncoderConfigurationName" + std::to_string(ch) + "_" + std::to_string(stream);
    m_token = "VideoEncoderConfigurationToken" + std::to_string(ch) + "_" + std::to_string(stream);
}

tt__VideoEncoderConfiguration* DevVideoEncoderConfiguration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevVideoEncoderConfiguration::setConfiguration(const tt__VideoEncoderConfiguration *configuration)
{
    return false;
}

DevAudioEncoderConfiguration::DevAudioEncoderConfiguration(int ch)
    : AudioEncoderConfiguration(ch)
{
    m_Name = "AudioEncoderConfigurationName" + std::to_string(ch);
    m_token = "AudioEncoderConfigurationToken" + std::to_string(ch);
}

tt__AudioEncoderConfiguration* DevAudioEncoderConfiguration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevAudioEncoderConfiguration::setConfiguration(const tt__AudioEncoderConfiguration *configuration)
{
    return false;
}

DevVideoAnalyticsConfiguration::DevVideoAnalyticsConfiguration(int ch)
    : VideoAnalyticsConfiguration(ch)
{
    m_Name = "VideoAnalyticsConfigurationName" + std::to_string(ch);
    m_token = "VideoAnalyticsConfigurationToken" + std::to_string(ch);
}

tt__VideoAnalyticsConfiguration* DevVideoAnalyticsConfiguration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevVideoAnalyticsConfiguration::setConfiguration(const tt__VideoAnalyticsConfiguration *configuration)
{
    return false;
}

DevPTZConfiguration::DevPTZConfiguration(int ch)
    : PTZConfiguration(ch)
{
    m_Name = "PTZConfigurationName" + std::to_string(ch);
    m_token = "PTZConfigurationToken" + std::to_string(ch);
    m_NodeToken = "PTZNodeToken" + std::to_string(ch);
}

tt__PTZConfiguration* DevPTZConfiguration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevPTZConfiguration::setConfiguration(const tt__PTZConfiguration *configuration)
{
    return false;
}

DevMetadataConfiguration::DevMetadataConfiguration(int ch)
    : MetadataConfiguration(ch)
{
    m_Name = "MetadataConfigurationName" + std::to_string(ch);
    m_token = "MetadataConfigurationToken" + std::to_string(ch);
}

tt__MetadataConfiguration* DevMetadataConfiguration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevMetadataConfiguration::setConfiguration(const tt__MetadataConfiguration *configuration)
{
    return false;
}

DevAudioOutputConfiguration::DevAudioOutputConfiguration(int ch)
    : AudioOutputConfiguration(ch)
{
    m_Name = "AudioOutputConfigurationName" + std::to_string(ch);
    m_token = "AudioOutputConfigurationToken" + std::to_string(ch);
}

tt__AudioOutputConfiguration* DevAudioOutputConfiguration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevAudioOutputConfiguration::setConfiguration(const tt__AudioOutputConfiguration *configuration)
{
    return false;
}

DevAudioDecoderConfiguration::DevAudioDecoderConfiguration(int ch)
    : AudioDecoderConfiguration(ch)
{
    m_Name = "AudioDecoderConfigurationName" + std::to_string(ch);
    m_token = "AudioDecoderConfigurationToken" + std::to_string(ch);
}

tt__AudioDecoderConfiguration* DevAudioDecoderConfiguration::getConfiguration(soap *soap) const
{
    return nullptr;
}

bool DevAudioDecoderConfiguration::setConfiguration(const tt__AudioDecoderConfiguration *configuration)
{
    return false;
}

DevMediaProfile::DevMediaProfile(int ch, int stream, bool fixed)
    : MediaProfile(ch, stream, fixed)
{
    m_Name = "ProfileName" + std::to_string(ch) + "_" + std::to_string(stream);
    m_token = "ProfileToken" + std::to_string(ch) + "_" + std::to_string(stream);
}

std::shared_ptr<MediaConfiguration> MediaConfiguration::m_instance;

std::shared_ptr<MediaConfiguration> MediaConfiguration::create()
{
    if (!m_instance)
    {
        m_instance = std::make_shared<DevMediaConfiguration>();
    }
    return m_instance;
}

DevMediaConfiguration::DevMediaConfiguration()
{
    /* 先创建所有Configuration */
    /* 遍历所有通道创建SourceConfiguration */
    for (int ch = 0; ch < 1; ch++)
    {
        m_videoSources.push_back(std::make_shared<DevVideoSourceConfiguration>(ch));
        m_audioSources.push_back(std::make_shared<DevAudioSourceConfiguration>(ch));
        /* 遍历每个通道所有stream创建EncoderConfiguration */
        for (int stream = 0; stream < 2; stream++)
        {
            m_videoEncoders.push_back(std::make_shared<DevVideoEncoderConfiguration>(ch, stream));
        }
        m_audioEncoders.push_back(std::make_shared<DevAudioEncoderConfiguration>(ch));
        m_videoAnalytics.push_back(std::make_shared<DevVideoAnalyticsConfiguration>(ch));
        m_ptz.push_back(std::make_shared<DevPTZConfiguration>(ch));
        m_metadata.push_back(std::make_shared<DevMetadataConfiguration>(ch));
    }
    /* 音频输出和音频解码Configuration */
    m_audioOutputs.push_back(std::make_shared<DevAudioOutputConfiguration>(0));
    m_audioDecoders.push_back(std::make_shared<DevAudioDecoderConfiguration>(0));
    /* 根据Configuration创建Profile */
    for (auto& ve : m_videoEncoders)
    {
        int ch = ve->getChannel();
        int stream = ve->getStream();
        m_profiles.push_back(std::make_shared<DevMediaProfile>(ch, stream));
        m_profiles.back()->getVideoSource() = m_videoSources[ch];
        m_profiles.back()->getVideoEncoder() = ve;
        m_profiles.back()->getAudioSource() = m_audioSources[ch];
        m_profiles.back()->getAudioEncoder() = m_audioEncoders[ch];
        m_profiles.back()->getVideoAnalytics() = m_videoAnalytics[ch];
        m_profiles.back()->getPTZ() = m_ptz[ch];
        m_profiles.back()->getMetadata() = m_metadata[ch];
        m_profiles.back()->getAudioOutput() = m_audioOutputs[0];
        m_profiles.back()->getAudioDecoder() = m_audioDecoders[0];

        /* 被Profile引用的Configuration，增加计数 */
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

std::shared_ptr<MediaProfile> DevMediaConfiguration::getProfile(const std::string& profileToken) const
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
