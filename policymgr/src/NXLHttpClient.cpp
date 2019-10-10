#include "NXLHttpClient.h"
#include "root_certificates.hpp"
#include <iostream>



NXLHttpClient::NXLHttpClient(const char* szServer, const char* szPort) :
m_SSLCtx{ ssl::context::sslv23_client }
{
	m_strServer = szServer;
	std::transform(m_strServer.begin(), m_strServer.end(), m_strServer.begin(), tolower);

	m_strPort = szPort;

	m_pHttpsStream = NULL;
	m_pHttpSocket = NULL;

	//get host
	m_strHost = m_strServer.substr(IsHttps() ? strlen("https://") : strlen("http://"));

	m_bConnected = false;
}

NXLHttpClient::~NXLHttpClient()
{
	Clear();
}

void NXLHttpClient::Reset()
{
	Clear();
	m_bConnected = false;
}

void NXLHttpClient::Clear()
{
	if (m_pHttpsStream)
	{
		m_pHttpsStream->next_layer().close();
		delete m_pHttpsStream;
		m_pHttpsStream = NULL;
	}

	if (m_pHttpSocket)
	{
		m_pHttpSocket->close();
		delete m_pHttpSocket;
		m_pHttpSocket = NULL;
	}
}

bool NXLHttpClient::IsHttps()
{

  size_t nFind =m_strServer.find("https://");

  return (nFind == 0);

}

bool NXLHttpClient::ConnectToServer()
{
	if (IsHttps())
	{
		return ConnectToHttpsServer();
	}
	else
	{
		return ConnectToHttpServer();
	}
	return false;
}

bool NXLHttpClient::ConnectToHttpServer()
{
	tcp::resolver resolver{ m_ioService };

	// Look up the domain name
	try
	{
		auto const lookup = resolver.resolve({ m_strHost.c_str(), m_strPort.c_str() });

		// Make the connection on the IP address we get from a lookup
		m_pHttpSocket = new tcp::socket(m_ioService);
		boost::asio::connect(*m_pHttpSocket, lookup);


		return true;
	}
	catch (...)
	{
	}
	
	return false;
}

bool NXLHttpClient::ConnectToHttpsServer()
{
	tcp::resolver resolver{ m_ioService };

	m_pHttpsStream = new ssl::stream<tcp::socket>{ m_ioService, m_SSLCtx};
	//m_pHttpsStream->set_verify_mode(SSL_VERIFY_PEER); //we didn't verify the certificate

	// Look up the domain name
	auto const lookup = resolver.resolve({ m_strHost.c_str(), m_strPort.c_str()});

	// Make the connection on the IP address we get from a lookup
	boost::asio::connect(m_pHttpsStream->next_layer(), lookup);

	// Perform the SSL handshake
	m_pHttpsStream->handshake(ssl::stream_base::client);

	return true;
}

bool NXLHttpClient::Request(http::request<http::string_body>& refReq,
	boost::beast::flat_buffer& refBuf,
	http::response<http::string_body>& refResp
	)
{
	//connect to server
	if (!m_bConnected)
	{
		m_bConnected = ConnectToServer();
	}

	//get http header
	refReq.set(http::field::host, m_strHost.c_str());

	//send and get response
	if (IsHttps())
	{
		/* resolve Bug 56290 */
		try {
			http::write(*m_pHttpsStream, refReq);
        	http::read(*m_pHttpsStream, refBuf, refResp);
    	} 
    	catch (const boost::system::system_error &e) {
			Clear();
			m_bConnected = ConnectToServer();
			http::write(*m_pHttpsStream, refReq);
			http::read(*m_pHttpsStream, refBuf, refResp);
    	}
	}
	else
	{
		try {
			http::write(*m_pHttpSocket, refReq);
			http::read(*m_pHttpSocket, refBuf, refResp);
    	} 
    	catch (boost::system::system_error &e) {
			Clear();
			m_bConnected = ConnectToServer();
			http::write(*m_pHttpSocket, refReq);
			http::read(*m_pHttpSocket, refBuf, refResp);
    	}
		
	}

	return true;
}