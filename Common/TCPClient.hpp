#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

#include <string>
#include <iostream>

#ifdef __unix__         
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <fcntl.h>
#elif defined(_WIN32) || defined(WIN32) 
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <Ws2tcpip.h>
#define OS_Windows
#endif

#include "ITCPClient.hpp"

class TCPClient : public ITCPClient
{
public:
	TCPClient(bool = false);
	virtual ~TCPClient();
	virtual void unblockSocket();
	virtual void blockSocket();
	virtual void connectToHost(const std::string&, const std::string &);
	virtual void setSocket(int);
	virtual void sendData(const void *, unsigned int);
	virtual int receiveData(void *, unsigned int);
	virtual bool isConnected() const;
	virtual void disconnectFromHost();
	virtual int getSocketDescriptor();
	virtual void preInit();

protected:
	#ifdef __unix__
	#elif defined(_WIN32) || defined(_WIN64) 
		WSADATA	m_WSAData;
	#endif
	int m_socketFd;
	bool m_isConnected;
	struct sockaddr_in m_hostInfo;
};

#endif /* !_TCPCLIENT_H_ */
