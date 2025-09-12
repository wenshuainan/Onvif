#ifndef DEVICE_MEDIA2_CONFIGURATION_H
#define DEVICE_MEDIA2_CONFIGURATION_H

#include "Interface/Media2Configuration.h"

class DevVideoEncoder2Configuration : public VideoEncoder2Configuration
{
public:
    DevVideoEncoder2Configuration(int ch, int stream);

public:
    virtual tt__VideoEncoder2Configuration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__VideoEncoder2Configuration *configuration);
};

class DevAudioEncoder2Configuration : public AudioEncoder2Configuration
{
public:
    DevAudioEncoder2Configuration(int ch);

public:
    virtual tt__AudioEncoder2Configuration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tt__AudioEncoder2Configuration *configuration);
};

class DevReceiver2Configuration : public Receiver2Configuration
{
public:
    DevReceiver2Configuration(int ch);

public:
    virtual tr2__ReceiverConfiguration* getConfiguration(soap *soap) const;
    virtual bool setConfiguration(const tr2__ReceiverConfiguration *configuration);
};

class DevMedia2Profile : public Media2Profile
{
public:
    DevMedia2Profile(int ch, int stream, bool fixed = true);
};

class DevMedia2Configuration : public Media2Configuration
{
public:
    DevMedia2Configuration();

public:
    virtual std::shared_ptr<Media2Profile> getProfile(const std::string& profileToken) const;
};

#endif