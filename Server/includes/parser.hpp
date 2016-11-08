#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <vector>
#include <fstream>

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
private:
char m_cmdBuffer[255];
TCPServer &m_server;
std::vector<SSLTCPClient *> &m_clientList;
std::ofstream m_output;
};

#endif /* !_PARSER_HPP_ */
