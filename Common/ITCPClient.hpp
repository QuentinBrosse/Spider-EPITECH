#ifndef _ITCPCLIENT_H_
#define _ITCPCLIENT_H_

#include <string>

class ITCPClient
{
public:
	virtual ~ITCPClient() { };
	virtual void unblockSocket() = 0;
	virtual void blockSocket() = 0;
	virtual void connectToHost(const std::string&, const std::string &) = 0;
	virtual void setSocket(int) = 0;
	virtual void sendData(const void *, unsigned int) = 0;
	virtual int receiveData(void *, unsigned int) = 0;
	virtual bool isConnected() const = 0;
	virtual void disconnectFromHost() = 0;
	virtual int getSocketDescriptor() = 0;
	virtual void preInit() = 0;
};

#endif /* !_ITCPCLIENT_H_ */
