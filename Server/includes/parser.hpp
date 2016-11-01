#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <vector>
#include <fstream>

class TCPClient;
class TCPServer;

class Parser
{
public:
Parser(TCPServer &, std::vector<TCPClient *> &);
  ~Parser();
  void parseCommands();
  std::ofstream &getOutputStream();
private:
char m_cmdBuffer[255];
TCPServer &m_server;
std::vector<TCPClient *> &m_clientList;
std::ofstream m_output;
};

#endif /* !_PARSER_HPP_ */
