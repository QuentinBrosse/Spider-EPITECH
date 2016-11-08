#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <vector>
#include <fstream>

#include "Protocol.hpp"

class TCPServer;
class SSLTCPClient;

class Parser
{
public:
Parser(TCPServer &, std::vector<SSLTCPClient *> &);
  ~Parser();
  void parseCommands();
  std::ofstream &getOutputStream();
  void blockRead();
  void unblockRead();
  void parseNetwork(char *);
  void networkDownloadLog(t_cmd *);
  void networkDownloadLogEnd(t_cmd *);
  void networkDisplayLog(t_cmd *);
  void networkDisplayLogEnd(t_cmd *);
  void commandList(std::string &);
  void commandExit(std::string &);
  void comandClose(std::string &);
  void commandPurge(std::string &);
  void commandDownload(std::string &);
  void commandDisplay(std::string &);
  void checkCommandBuffer(int);

private:
char m_cmdBuffer[255];
TCPServer &m_server;
std::vector<SSLTCPClient *> &m_clientList;
std::ofstream m_output;
};

#endif /* !_PARSER_HPP_ */
