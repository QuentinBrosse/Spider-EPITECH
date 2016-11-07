#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <vector>
#include <fstream>

class SSLNetwork;
class TCPServer;

class Parser
{
public:
Parser(TCPServer &, std::vector<SSLNetwork *> &);
  ~Parser();
  void parseCommands();
  std::ofstream &getOutputStream();
  void blockRead();
  void unblockRead();
private:
char m_cmdBuffer[255];
TCPServer &m_server;
std::vector<SSLNetwork *> &m_clientList;
std::ofstream m_output;
};

#endif /* !_PARSER_HPP_ */
