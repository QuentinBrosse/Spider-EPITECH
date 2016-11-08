#include "TCPServer.hpp"

TCPServer::TCPServer() :
  m_isListening(false)
{
#ifdef __unix__
  //Add any unix code here...
#elif defined(_WIN32) || defined(_WIN64)
  if (WSAStartup(MAKEWORD(2, 0), &m_WSAData) != 0)
    {
      std::cout << "WSA initialisation failed : " << WSAGetLastError() << std::endl;
    }
#endif
}

TCPServer::~TCPServer()
{
	#ifdef __unix__
		close(m_masterSocket);
	#elif defined(_WIN32) || defined(_WIN64) 
		closesocket(m_masterSocket);
	#endif
	#ifdef __unix__
		//Add any unix code here...
	#elif defined(_WIN32) || defined(_WIN64)
		WSACleanup();
	#endif
}

void TCPServer::listenClients(const std::string &ip, const std::string &port, unsigned int maxClients)
{
	int opt = true;
	m_clientSockets = new int[maxClients];
	m_maxClients = maxClients;

	for (int m_i = 0; m_i < m_maxClients; m_i++)
	{
		m_clientSockets[m_i] = 0;
	}
	//create a master socket and set it to allow multiple connections
	if ((m_masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
	{
		std::cout << "master socket creation failed" << std::endl;
	}

	if (setsockopt(m_masterSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&opt), sizeof(opt)) < 0)
	{
		std::cout << "master socket configuration failed" << std::endl;
	}

	m_listenInfo.sin_family = AF_INET;
	m_listenInfo.sin_addr.s_addr = INADDR_ANY;
	m_listenInfo.sin_port = htons(std::stoi(port));
	m_infoLen = sizeof(m_listenInfo);

	if (bind(m_masterSocket, reinterpret_cast<struct sockaddr *>(&m_listenInfo), sizeof(m_listenInfo)) < 0)
	{
		std::cout << "bind failed" << std::endl;
	}

	if (listen(m_masterSocket, maxClients) < 0)
	{
		std::cout << "Cannot listen" << std::endl;
	}
	else
		m_isListening = true;
}

bool TCPServer::isListening() const
{
	return m_isListening;
}

int TCPServer::getMasterSocket()
{
	return m_masterSocket;
}

void TCPServer::fdSetChk(int &m_SeedDescription, fd_set& m_readFds)
{
  if (m_SeedDescription > 0)
    FD_SET(m_SeedDescription, &m_readFds);
}

void TCPServer::waitForClientsActivity()
{
	FD_ZERO(&m_readFds);
	FD_SET(m_masterSocket, &m_readFds);
	m_maxSocket = m_masterSocket;
	for (int m_i = 0; m_i < m_maxClients; m_i++)
	{
		m_SeedDescription = m_clientSockets[m_i];
		fdSetChk(m_SeedDescription, m_readFds);
		if (m_SeedDescription > m_maxSocket)
			m_maxSocket = m_SeedDescription;
	}
	struct timeval tv;
	tv.tv_usec = 50;
	tv.tv_sec = 0;
	m_activity = select(m_maxSocket + 1, &m_readFds, NULL, NULL, &tv);
	if ((m_activity < 0) && (errno != EINTR))
	{
		std::cout << "select error" << std::endl;
	}
}

int TCPServer::incomingConnection()
{
	return FD_ISSET(m_masterSocket, &m_readFds);
}

SSLNetwork* TCPServer::getIncomingCLient()
{
	if ((m_newSocket = accept(m_masterSocket, reinterpret_cast<struct sockaddr *>(&m_listenInfo), reinterpret_cast<socklen_t *>(&m_infoLen))) < 0)
	{
		std::cout << "Cannot accept" << std::endl;
	}

	std::cout << "Nouvelle connection:" << std::endl << "\tFD: " << m_newSocket << std::endl << "\tIP:" << inet_ntop(AF_INET, &(m_listenInfo.sin_addr), m_ipAddr, sizeof(m_ipAddr)) << std::endl << "\tPORT:" << ntohs(m_listenInfo.sin_port) << std::endl;

	for (int m_i = 0; m_i < m_maxClients; m_i++)
	{
		if (m_clientSockets[m_i] == 0)
		{
			m_clientSockets[m_i] = m_newSocket;
			break;
		}
	}

	SSLNetwork* client = new SSLNetwork(true);
	client->setSocket(m_newSocket);
	return client;
}

void TCPServer::disconectClient(SSLNetwork *client)
{
	for (int m_i = 0; m_i < m_maxClients; m_i++)
	{
		if (client->getSocketDescriptor() == m_clientSockets[m_i])
		{
			m_SeedDescription = m_clientSockets[m_i];
			#ifdef __unix__
				close(m_SeedDescription);
			#elif defined(_WIN32) || defined(_WIN64) 
				closesocket(m_SeedDescription);
			#endif
			m_clientSockets[m_i] = 0;
		}
	}
}

bool TCPServer::checkSocket(SSLNetwork *client)
{
	return FD_ISSET(client->getSocketDescriptor(), &m_readFds);
}

const std::vector<int> TCPServer::getClientsSocketList()
{
	std::vector<int> list;
	for (int m_i = 0; m_i < m_maxClients; m_i++)
	  {
		  list.push_back(m_clientSockets[m_i]);
	  }
  return (list);
}
 
