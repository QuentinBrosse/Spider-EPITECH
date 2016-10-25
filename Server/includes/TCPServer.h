#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include <vector>

#ifdef __unix__
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#elif defined(_WIN32) || defined(_WIN64) 
#include <errno.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define OS_Windows
#endif

#include "TCPClient.h"

class TCPServer
{
public:
	TCPServer();
	~TCPServer();
	void listenClients(const std::string &, const std::string&, unsigned int);
	bool isListening() const;
	int getMasterSocket();
	void waitForClientsActivity();
	int incomingConnection();
	TCPClient* getIncomingCLient();
	void disconectClient(TCPClient *client);
	bool checkSocket(TCPClient *client);

private:
	#ifdef __unix__
	#elif defined(_WIN32) || defined(_WIN64) 
		WSADATA	m_WSAData;
	#endif
	fd_set	m_readFds;
	struct sockaddr_in m_listenInfo;
	bool m_isListening;
	char m_ipAddr[INET_ADDRSTRLEN];
	int m_masterSocket;
	int m_maxSocket;
	int	m_infoLen;
	int m_newSocket;
	int *m_clientSockets;
	int m_maxClients;
	int m_activity;
	int m_SeedDescription;
};


#endif /* !_TCPSERVER_H_ */