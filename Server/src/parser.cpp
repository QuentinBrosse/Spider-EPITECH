#include <unistd.h>
#include <string>
#include <cstring>
#include <sstream>
#include <exception>

#include "parser.hpp"
#include "TCPServer.hpp"
#include "Protocol.hpp"
#include "SSLTCPClient.hpp"

Parser::Parser(TCPServer &server, std::vector<SSLTCPClient *> &clients) :
  m_server(server), m_clientList(clients)
{

}

Parser::~Parser()
{

}

void Parser::blockRead()
{
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) & ~O_NONBLOCK);
}

void Parser::unblockRead()
{
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
}

std::ofstream &Parser::getOutputStream()
{
  return m_output;
}

void Parser::parseNetwork(char *buffer)
{
  t_cmd *command = reinterpret_cast<t_cmd *>(buffer);
  if (command->cmd == commandType::DOWNLOAD_LOG)
    {
      char data[buffer_size + 1];
      std::memcpy(data, command->buffer, buffer_size);
      data[command->data_len] = '\0';
      this->getOutputStream() << data;
    }
  if (command->cmd == commandType::DOWNLOAD_LOG_END)
    {
      this->getOutputStream() << std::endl;
      std::cout << "LOG DOWNLOAD COMPLETE" << std::endl;
    }
  if (command->cmd == commandType::DISPLAY_LOG)
    {
      char data[buffer_size + 1];
      std::memcpy(data, command->buffer, buffer_size);
      data[command->data_len] = '\0';
      std::cout << data;
    }
  if (command->cmd == commandType::DISPLAY_LOG_END)
    {
      std::cout << std::endl;
      std::cout << "LOG DOWNLOAD COMPLETED" << std::endl;
    }
}

void Parser::parseCommands()
{
  int size = read(0,&m_cmdBuffer,254);
  if (size > 0)
    {
      if (m_cmdBuffer[size - 1] == '\n')
	size -= 1;
      m_cmdBuffer[size] = '\0';
      std::string cmd(m_cmdBuffer);
      if (std::strncmp(cmd.c_str(), "LIST", 4) == 0)
	{
	  unsigned int pos = 0;
	  std::cout << "CLIENTS AVAILABLE: " << std::endl;
	  {
	    auto list = m_server.getClientsSocketList();
	    for (auto it = list.begin(); it != list.end() ; it ++)
	      {
		if ((*it) != 0)
		  std::cout << "Client with id " << pos << " available" << std::endl;
		pos ++;
	      }
	  }
        }
      if (std::strncmp(cmd.c_str(), "EXIT", 4) == 0)
	{
	  exit(0);
	}
      if (std::strncmp(cmd.c_str(), "CLOSE", 5) == 0)
	{
	  std::istringstream stream(cmd);
	  std::string cmd;
	  std::string client_id;
	  int sock;

	  stream >> cmd >> client_id;
	  auto list = m_server.getClientsSocketList();
	  try {
	    sock = list[std::stoi(client_id)];
	  }
	  catch (std::exception &e) {
	    std::cout << "Invalid syntaxe for command CLOSE [ID]" << std::endl;
	    return ;
	  }
	  for (auto clientIt = m_clientList.begin(); clientIt != m_clientList.end(); clientIt++)
	    {
	      if (sock == (*clientIt)->getSocketDescriptor() && (*clientIt)->isConnected())
		{
		  t_cmd command;
		  command.cmd = commandType::CLOSE;
		  (*clientIt)->sendData(reinterpret_cast<char *>(&command), sizeof(t_cmd));
		  std::cout << "Command sent" << std::endl;
		  return ;
		}
	    }
	  std::cout << "Client with id " << client_id << " not found" << std::endl;
	}
      if (std::strncmp(cmd.c_str(), "PURGE_LOG", 9) == 0)
	{
	  std::istringstream stream(cmd);
	  std::string cmd;
	  std::string client_id;
	  int sock;

	  stream >> cmd >> client_id;
	  auto list = m_server.getClientsSocketList();
	  try {
	    sock = list[std::stoi(client_id)];
	  }
	  catch(std::exception &e) {
	    std::cout << "Invalid syntaxze for PURGE_LOG [ID]" << std::endl;
	    return ;
	  }
	  for (auto clientIt = m_clientList.begin(); clientIt != m_clientList.end(); clientIt++)
	    {
	      if (sock == (*clientIt)->getSocketDescriptor() && (*clientIt)->isConnected())
		{
		  t_cmd command;
		  command.cmd = commandType::PURGE_LOG;
		  (*clientIt)->sendData(reinterpret_cast<char *>(&command), sizeof(t_cmd));
		  return ;
		}
	    }
	  std::cout << "Client with id " << client_id << " not found" << std::endl;
	}

      if (std::strncmp(cmd.c_str(), "DOWNLOAD_LOG", 12) == 0)
	{
	  std::istringstream stream(cmd);
	  std::string cmd;
	  std::string name;
	  std::string client_id;
	  int sock;

	  stream >> cmd >> client_id;
	  auto list = m_server.getClientsSocketList();
	  try {
	    sock = list[std::stoi(client_id)];
	  }
	  catch (std::exception &e) {
	    std::cout << "Invalid syntaxe for DOWNLOAD_LOG [ID]" << std::endl;
	    return;
	  }
	  this->blockRead();
	  std::cout << "Entrez le nom sous lequel le log sera enregistrer:" << std::endl;
	  std::cin >> name;
	  this->unblockRead();
	  if (name.size() == 0)
	    {
	      std::cout << "Aucun nom n'a ete communiquer, le log sera enregistrer sous le nom default.txt" << std::endl;
	      name = "default.txt";
	    }

	  m_output.open(name);

	  for (auto clientIt = m_clientList.begin(); clientIt != m_clientList.end(); clientIt++)
	    {
	      if (sock == (*clientIt)->getSocketDescriptor() && (*clientIt)->isConnected())
		{
		  t_cmd command;
		  command.cmd = commandType::DOWNLOAD_LOG;
		  (*clientIt)->sendData(reinterpret_cast<char *>(&command), sizeof(t_cmd));
		  return ;
		}
	    }
	  std::cout << "Client with id " << client_id << " not found" << std::endl;
	}

      if (std::strncmp(cmd.c_str(), "DISPLAY_LOG", 11) == 0)
	{
	  std::istringstream stream(cmd);
	  std::string cmd;
	  std::string client_id;
	  int sock;

	  stream >> cmd >> client_id;
	  auto list = m_server.getClientsSocketList();
	  try {
	    sock = list[std::stoi(client_id)];
	  }
	  catch (std::exception &e) {
	    std::cout << "Invalid syntaxe DISPLAY_LOG [ID]" << std::endl;
	    return ;
	  }
	  for (auto clientIt = m_clientList.begin(); clientIt != m_clientList.end(); clientIt++)
	    {
	      if (sock == (*clientIt)->getSocketDescriptor() && (*clientIt)->isConnected())
		{
		  t_cmd command;
		  command.cmd = commandType::DISPLAY_LOG;
		  (*clientIt)->sendData(reinterpret_cast<char *>(&command), sizeof(t_cmd));
		  return ;
		}
	    }
	  std::cout << "Client with id " << client_id << " not found" << std::endl;
	}

    }
}
