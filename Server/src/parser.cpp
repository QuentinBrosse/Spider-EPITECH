#include <unistd.h>
#include <string>
#include <cstring>
#include <sstream>

#include "parser.hpp"
#include "TCPServer.hpp"
#include "Protocol.hpp"
#include "TCPClient.hpp"

Parser::Parser(TCPServer &server, std::vector<TCPClient *> &clients) :
  m_server(server), m_clientList(clients)
{

}

Parser::~Parser()
{

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
	  stream >> cmd >> client_id;

	  std::cout << "Parsed: " << cmd << " " << client_id << std::endl;
	  auto list = m_server.getClientsSocketList();
	  int sock = list[std::stoi(client_id)];
	  for (auto clientIt = m_clientList.begin(); clientIt != m_clientList.end(); clientIt++)
	    {
	      if (sock == (*clientIt)->getSocketDescriptor() && (*clientIt)->isConnected())
		{
		  t_cmd command;
		  command.cmd = commandType::CLOSE;
		  (*clientIt)->sendData(reinterpret_cast<char *>(&command), sizeof(t_cmd));
		  break;
		}
	    }
	}
      if (std::strncmp(cmd.c_str(), "PURGE_LOG", 9) == 0)
	{
	  std::istringstream stream(cmd);
	  std::string cmd;
	  std::string client_id;
	  stream >> cmd >> client_id;

	  std::cout << "Parsed: " << cmd << " " << client_id << std::endl;
	  auto list = m_server.getClientsSocketList();
	  int sock = list[std::stoi(client_id)];
	  for (auto clientIt = m_clientList.begin(); clientIt != m_clientList.end(); clientIt++)
	    {
	      if (sock == (*clientIt)->getSocketDescriptor() && (*clientIt)->isConnected())
		{
		  t_cmd command;
		  command.cmd = commandType::PURGE_LOG;
		  (*clientIt)->sendData(reinterpret_cast<char *>(&command), sizeof(t_cmd));
		  break;
		}
	    }
	}
      if (std::strncmp(cmd.c_str(), "DOWNLOAD_LOG", 12) == 0)
	{
	  std::istringstream stream(cmd);
	  std::string cmd;
	  std::string client_id;
	  stream >> cmd >> client_id;

	  std::cout << "Parsed: " << cmd << " " << client_id << std::endl;
	  auto list = m_server.getClientsSocketList();
	  int sock = list[std::stoi(client_id)];
	  for (auto clientIt = m_clientList.begin(); clientIt != m_clientList.end(); clientIt++)
	    {
	      if (sock == (*clientIt)->getSocketDescriptor() && (*clientIt)->isConnected())
		{
		  t_cmd command;
		  command.cmd = commandType::DOWNLOAD_LOG;
		  (*clientIt)->sendData(reinterpret_cast<char *>(&command), sizeof(t_cmd));
		  break;
		}
	    }
	}

    }
}
