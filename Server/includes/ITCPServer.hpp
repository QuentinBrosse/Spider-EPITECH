#ifndef _ITCPSERVER_H_
#define _ITCPSERVER_H_

#include <string>
#include <vector>
#include "SSLTCPClient.hpp"

class ITCPServer
{
public:
  virtual ~ITCPServer() { };
  virtual void listenClients(const std::string &, const std::string&, unsigned int) = 0;
  virtual bool isListening() const = 0;
  virtual int getMasterSocket() = 0;
  virtual void waitForClientsActivity() = 0;
  virtual int incomingConnection() = 0;
  virtual void fdSetChk(int &m_SeedDescription, fd_set& m_readFds) = 0;
  virtual SSLTCPClient* getIncomingCLient() = 0;
  virtual void disconectClient(SSLTCPClient *client) = 0;
  virtual bool checkSocket(SSLTCPClient *client) = 0;
  virtual const std::vector<int> getClientsSocketList() = 0;
};


#endif /* !_ITCPSERVER_H_ */
