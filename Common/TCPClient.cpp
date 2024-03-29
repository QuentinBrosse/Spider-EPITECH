#include "TCPClient.hpp"

TCPClient::TCPClient(bool doNotSetup) :
	m_socketFd(0), m_isConnected(false)
{
	if (doNotSetup == false)
	{
		this->preInit();
	}
}

void TCPClient::preInit()
{
#ifdef __unix__
	//Add any unix code here...
#elif defined(_WIN32) || defined(_WIN64)
	if (WSAStartup(MAKEWORD(2, 0), &m_WSAData) != 0)
	{
		std::cout << "Failed. Error Code : " << WSAGetLastError() << std::endl;
	}
	else
		std::cout << "Client initilised" << std::endl;
#endif
}

TCPClient::~TCPClient()
{
	#ifdef __unix__
		//Add any unix code here...
	#elif defined(_WIN32) || defined(_WIN64)
		WSACleanup();
	#endif
	std::cout << "Client destroyed" << std::endl;
}

void TCPClient::connectToHost(const std::string&ip, const std::string &port)
{
	if ((m_socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
	{
		std::cout << "Socket Creation Failed" << std::endl;
	}

	inet_pton(AF_INET, ip.c_str(), &m_hostInfo.sin_addr.s_addr);
	m_hostInfo.sin_family = AF_INET;
	m_hostInfo.sin_port = htons(std::stoi(port));
	
	if (connect(m_socketFd, reinterpret_cast<struct sockaddr *>(&m_hostInfo), sizeof(m_hostInfo)) != 0)
	{
		std::cout << "Cannot connect " << std::endl;
		#ifdef __unix__
				close(m_socketFd);
		#elif defined(_WIN32) || defined(_WIN64) 
				closesocket(m_socketFd);
		#endif
				m_isConnected = false;
	}
	else
		m_isConnected = true;
}

bool TCPClient::isConnected() const
{
	return m_isConnected;
}

void TCPClient::setSocket(int socket)
{
	m_socketFd = socket;
	m_isConnected = true;
}

void TCPClient::sendData(const void *data, unsigned int len)
{
	if (send(m_socketFd, reinterpret_cast<const char *>(data), len, 0) != len)
	{
		std::cout << "Send error" << std::endl;
		m_isConnected = false;
	}
}

int TCPClient::receiveData(void *buffer, unsigned int len)
{
	int valReaded = 0;

	if ((valReaded = recv(m_socketFd, reinterpret_cast<char *>(buffer), len, 0)) == 0)
	{
		m_isConnected = false;
	}

	return valReaded;
}

void TCPClient::unblockSocket()
{
	u_long iMode = 1;

	#ifdef __unix__
		int flags = fcntl(m_socketFd, F_GETFL, 0);
		flags = (flags&~O_NONBLOCK);
		fcntl(m_socketFd, F_SETFL, flags);
	#elif defined(_WIN32) || defined(_WIN64) 
		ioctlsocket(m_socketFd, FIONBIO, &iMode);
	#endif
}

void TCPClient::blockSocket()
{
	u_long iMode = 0;
	#ifdef __unix__
		int flags = fcntl(m_socketFd, F_GETFL, 0);
		flags = (flags&~O_NONBLOCK);
		fcntl(m_socketFd, F_SETFL, flags);
	#elif defined(_WIN32) || defined(_WIN64) 
		ioctlsocket(m_socketFd, FIONBIO, &iMode);
	#endif
}

void TCPClient::disconnectFromHost()
{
	#ifdef __unix__
		close(m_socketFd);
	#elif defined(_WIN32) || defined(_WIN64) 
		closesocket(m_socketFd);
	#endif
}

int TCPClient::getSocketDescriptor()
{
	return m_socketFd;
}
