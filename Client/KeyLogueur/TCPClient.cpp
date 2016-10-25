#include "TCPClient.h"

TCPClient::TCPClient(bool doNotSetup) :
	m_socketFd(0)
{
	if (doNotSetup == false)
	{
		#ifdef __unix__
			//Add any unix code here...
		#elif defined(_WIN32) || defined(_WIN64)
			if (WSAStartup(MAKEWORD(2, 0), &m_WSAData) != 0)
			{
				std::cout << "Failed. Error Code : " << WSAGetLastError() << std::endl;
			}
			else
				std::cout << "Client initiated" << std::endl;
		#endif
	}
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
	}
}

void TCPClient::setSocket(int socket)
{
	m_socketFd = socket;
}

void TCPClient::sendData(const void *data, unsigned int len)
{
	if (send(m_socketFd, reinterpret_cast<const char *>(data), len, 0) != len)
	{
		std::cout << "Cannot send" << std::endl;
	}
}

int TCPClient::receiveData(void *buffer, unsigned int len)
{
	int valReaded = 0;

	if ((valReaded = recv(m_socketFd, reinterpret_cast<char *>(buffer), len, 0)) == 0)
	{
		std::cout << "Receive error, client / host disconnected ?" << std::endl;
	}

	return valReaded;
}

void TCPClient::disconnectFromHost()
{
	#ifdef __unix__
		close(m_socketFd);
	#elif defined(_WIN32) || defined(_WIN64) 
		closesocket(m_socketFd);
	#endif
}

TCPClient& TCPClient::operator<<(const std::string &in)
{
	this->sendData(reinterpret_cast<const void *>(in.c_str()), in.size());
	return (*this);
}

TCPClient& TCPClient::operator>>(std::string &out)
{
	char buffer[1024];
	int readed = this->receiveData(buffer, 1024);
	out.assign(reinterpret_cast<char *>(buffer), readed);
	return (*this);
}

int TCPClient::getSocketDescriptor()
{
	return m_socketFd;
}