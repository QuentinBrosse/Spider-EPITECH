#include <iostream>
#include <string>

#include "TCPServer.h"
#include "TCPClient.h"

int main(int argc, char **argv)
{
	TCPServer server;
	std::vector<TCPClient*> clientsList;
	char buffer[1024];

	server.listenClients("127.0.0.1", "4242", 3);
	while (server.isListening())
	{
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
				if ((read = ((*clientIt)->receiveData(buffer, 1024))) == 0)
				{
					std::cout << "Client disconected" << std::endl;
					server.disconectClient(*clientIt);
				}
				else
				{
					buffer[read] = '\0';
					std::cout << buffer << std::endl;
				}
			}
		}
	}
	return (0);
}