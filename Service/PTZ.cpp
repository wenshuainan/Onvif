#include "PTZ.h"
#include "httpda.h"

PTZ::PTZ(Server* server, int port, bool tls)
    : Service(server, port, tls)
{
    printf("++++++ PTZ\n");
    this->soap->bind_flags = SO_REUSEADDR;
    soap_register_plugin(this->soap, http_da);
    m_bStop = false;
    m_thread = std::unique_ptr<std::thread>(new std::thread(&PTZ::threadProc, this));
}

PTZ::~PTZ()
{
    printf("------ PTZ\n");
    m_bStop = true;
    printf("wait PTZ thread join\n");
    shutdown(this->soap->master, SHUT_RDWR);
    m_thread->join();
    printf("PTZ thread joined\n");
}

void PTZ::threadProc()
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
        printf("PTZ (ssl_)run sopted\n");
    }
    printf("PTZ threadProc exit\n");
}

const char* PTZ::getPath() const
{
    return "/onvif/ptz_service";
}   

const char* PTZ::getNamespace() const
{
    return "http://www.onvif.org/ver20/ptz/wsdl";
}

const char* PTZ::getVersion() const
{
#ifdef PTZ_SERVICE_SPECIFICATION_VERSION
    return PTZ_SERVICE_SPECIFICATION_VERSION;
#else
    return "";
#endif
}

int PTZ::run(int port, int backlog)
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
int PTZ::ssl_run(int port, int backlog)
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

int PTZ::dispatch()
{
    int err = authenticate(this->soap);
    if (err != SOAP_OK)
    {
        return err;
    }

    return PTZBindingService::dispatch();
}