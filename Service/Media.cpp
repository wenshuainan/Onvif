#include "Media.h"
#include "httpda.h"
#include "Interface/MediaConfiguration.h"

Media::Media(Server* server, int port, bool tls)
    : Service(server, port, tls)
{
    printf("++++++ Media\n");
    this->soap->bind_flags = SO_REUSEADDR;
    soap_register_plugin(this->soap, http_da);
    m_bStop = false;
    m_thread = std::unique_ptr<std::thread>(new std::thread(&Media::threadProc, this));
}

Media::~Media()
{
    printf("------ Media\n");
    m_bStop = true;
    printf("wait Media thread join\n");
    shutdown(this->soap->master, SHUT_RDWR);
    m_thread->join();
    printf("Media thread joined\n");
}

void Media::threadProc()
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
        printf("Media (ssl_)run sopted\n");
    }
    printf("Media threadProc exit\n");
}

const char* Media::getPath() const
{
    return "/onvif/media_service";
}

const char* Media::getNamespace() const
{
    return "http://www.onvif.org/ver10/media/wsdl";
}

const char* Media::getVersion() const
{
#ifdef MEDIA_SERVICE_SPECIFICATION_VERSION
    return MEDIA_SERVICE_SPECIFICATION_VERSION;
#else
    return "";
#endif
}

int Media::run(int port, int backlog)
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
int Media::ssl_run(int port, int backlog)
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

int Media::dispatch()
{
    int err = authenticate(this->soap);
    if (err != SOAP_OK)
    {
        return err;
    }

    return MediaBindingService::dispatch();
}

int Media::GetProfiles(_trt__GetProfiles *trt__GetProfiles, _trt__GetProfilesResponse &trt__GetProfilesResponse)
{
    auto mediaConfiguration = MediaConfiguration::create();
    if (!mediaConfiguration)
    {
        return SOAP_ERR;
    }

    auto profiles = mediaConfiguration->getProfiles();
    for (auto profile : profiles)
    {
        tt__Profile* item = soap_new_tt__Profile(trt__GetProfilesResponse.soap);
        if (!item)
        {
            return SOAP_ERR;
        }
        item->Name = profile->getName();
        item->token = profile->getToken();
        item->fixed = make_bool(trt__GetProfilesResponse.soap, profile->isFixed());
        item->VideoSourceConfiguration = profile->getVideoSource()->getConfiguration(trt__GetProfilesResponse.soap);
        item->AudioSourceConfiguration = profile->getAudioSource()->getConfiguration(trt__GetProfilesResponse.soap);
        item->VideoEncoderConfiguration = profile->getVideoEncoder()->getConfiguration(trt__GetProfilesResponse.soap);
        item->AudioEncoderConfiguration = profile->getAudioEncoder()->getConfiguration(trt__GetProfilesResponse.soap);
        item->VideoAnalyticsConfiguration = profile->getVideoAnalytics()->getConfiguration(trt__GetProfilesResponse.soap);
        item->PTZConfiguration = profile->getPTZ()->getConfiguration(trt__GetProfilesResponse.soap);
        item->MetadataConfiguration = profile->getMetadata()->getConfiguration(trt__GetProfilesResponse.soap);

        trt__GetProfilesResponse.Profiles.push_back(item);
    }

    return SOAP_OK;
}

int Media::GetStreamUri(_trt__GetStreamUri *trt__GetStreamUri, _trt__GetStreamUriResponse &trt__GetStreamUriResponse)
{
    auto mediaConfiguration = MediaConfiguration::create();
    if (!mediaConfiguration)
    {
        return SOAP_EOM;
    }

    auto profile = mediaConfiguration->getProfile(trt__GetStreamUri->ProfileToken);
    if (!profile)
    {
        return SOAP_ERR;
    }

    tt__MediaUri *MediaUri = soap_new_tt__MediaUri(trt__GetStreamUriResponse.soap);
    if (!MediaUri)
    {
        return SOAP_EOM;
    }

    // MediaUri->Uri = profile->getStreamUri();
    MediaUri->InvalidAfterConnect = false;
    MediaUri->InvalidAfterReboot = false;

    trt__GetStreamUriResponse.MediaUri = MediaUri;
    return SOAP_OK;
}