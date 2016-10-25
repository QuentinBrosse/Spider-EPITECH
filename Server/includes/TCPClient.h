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
#elif defined(_WIN32) || defined(WIN32) 
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define OS_Windows
#endif

class TCPClient
{
public:
	TCPClient(bool = false);
	~TCPClient();
	void connectToHost(const std::string&, const std::string &);
	void setSocket(int);
	void sendData(const void *, unsigned int);
	int receiveData(void *, unsigned int);
	void disconnectFromHost();
	TCPClient& operator<<(const std::string &);
	TCPClient& operator>>(std::string &);
	int getSocketDescriptor();

private:
	#ifdef __unix__
	#elif defined(_WIN32) || defined(_WIN64) 
		WSADATA	m_WSAData;
	#endif
	int m_socketFd;
	struct sockaddr_in m_hostInfo;
};

#endif /* !_TCPCLIENT_H_ */