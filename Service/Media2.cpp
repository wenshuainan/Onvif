#include "Media2.h"
#include "httpda.h"
#include "Interface/Media2Configuration.h"

Media2::Media2(Server* server, int port, bool tls)
    : Service(server, port, tls)
{
    printf("++++++ Media2\n");
    this->soap->bind_flags = SO_REUSEADDR;
    soap_register_plugin(this->soap, http_da);
    m_bStop = false;
    m_thread = std::unique_ptr<std::thread>(new std::thread(&Media2::threadProc, this));
}

Media2::~Media2()
{
    printf("------ Media2\n");
    m_bStop = true;
    printf("wait Media2 thread join\n");
    shutdown(this->soap->master, SHUT_RDWR);
    m_thread->join();
    printf("Media2 thread joined\n");
}

void Media2::threadProc()
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
        printf("Media2 (ssl_)run sopted\n");
    }
    printf("Media2 threadProc exit\n");
}

const char* Media2::getPath() const
{
    return "/onvif/media2_service";
}

const char* Media2::getNamespace() const
{
    return "http://www.onvif.org/ver20/media/wsdl";
}

const char* Media2::getVersion() const
{
#ifdef MEDIA2_SERVICE_SPECIFICATION_VERSION
    return MEDIA2_SERVICE_SPECIFICATION_VERSION;
#else
    return "";
#endif
}

int Media2::run(int port, int backlog)
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
int Media2::ssl_run(int port, int backlog)
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

int Media2::dispatch()
{
    int err = authenticate(this->soap);
    if (err != SOAP_OK)
    {
        return err;
    }

    return Media2BindingService::dispatch();
}

int Media2::GetProfiles(_tr2__GetProfiles *tr2__GetProfiles, _tr2__GetProfilesResponse &tr2__GetProfilesResponse)
{
    auto media2Configuration = Media2Configuration::create();
    if (!media2Configuration)
    {
        return SOAP_EOM;
    }

    auto profiles = media2Configuration->getProfiles();
    for (auto profile : profiles)
    {
        tr2__MediaProfile *item = soap_new_tr2__MediaProfile(tr2__GetProfilesResponse.soap);
        if (!item)
        {
            return SOAP_EOM;
        }

        item->Configurations = soap_new_tr2__ConfigurationSet(tr2__GetProfilesResponse.soap);
        if (!item->Configurations)
        {
            return SOAP_EOM;
        }

        item->Name = profile->getName();
        item->token = profile->getToken();
        item->fixed = makeBool(tr2__GetProfilesResponse.soap, profile->isFixed());
        item->Configurations->VideoSource = profile->getVideoSource()->getConfiguration(tr2__GetProfilesResponse.soap);
        item->Configurations->AudioSource = profile->getAudioSource()->getConfiguration(tr2__GetProfilesResponse.soap);
        item->Configurations->VideoEncoder = profile->getVideoEncoder2()->getConfiguration(tr2__GetProfilesResponse.soap);
        item->Configurations->AudioEncoder = profile->getAudioEncoder2()->getConfiguration(tr2__GetProfilesResponse.soap);
        item->Configurations->Analytics = profile->getVideoAnalytics()->getConfiguration(tr2__GetProfilesResponse.soap);
        item->Configurations->PTZ = profile->getPTZ()->getConfiguration(tr2__GetProfilesResponse.soap);
        item->Configurations->Metadata = profile->getMetadata()->getConfiguration(tr2__GetProfilesResponse.soap);
        item->Configurations->AudioOutput = profile->getAudioOutput()->getConfiguration(tr2__GetProfilesResponse.soap);
        item->Configurations->AudioDecoder = profile->getAudioDecoder()->getConfiguration(tr2__GetProfilesResponse.soap);
        item->Configurations->Receiver = profile->getReceiver2()->getConfiguration(tr2__GetProfilesResponse.soap);

        tr2__GetProfilesResponse.Profiles.push_back(item);
    }

    return SOAP_OK;
}