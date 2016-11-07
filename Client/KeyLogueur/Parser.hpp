#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Protocol.hpp"
#include "KeyLoger.hpp"

class SSLNetwork;

class Parser
{
public:
	Parser(SSLNetwork &);
	~Parser();
	void parseCommands();
private:
	SSLNetwork &m_client;
	char m_buffer[sizeof(t_cmd)];
};

#endif /* !PARSER_HPP_ */