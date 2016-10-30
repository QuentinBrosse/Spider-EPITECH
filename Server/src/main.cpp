#include <iostream>
#include <string>

#include "TCPServer.h"
#include "TCPClient.hpp"
#include "Protocol.hpp"

int main(int argc, char **argv)
{
	TCPServer server;
	std::vector<TCPClient*> clientsList;
	char buffer[sizeof(t_proto)];

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
				if ((read = ((*clientIt)->receiveData(buffer, sizeof(t_proto)))) == 0)
				{
					std::cout << "Client disconected" << std::endl;
					server.disconectClient(*clientIt);
				}
				else
				{
				  if (read == sizeof(t_proto))
				    {
				      t_proto *data = reinterpret_cast<t_proto *>(buffer);
				      if (data->source == eventSource::KEYBOARD)
					{
					  std::cout << "Client key ";
					  if (m_keycode2name.find(data->button_code) != m_keycode2name.end())
					    std::cout << m_keycode2name.at(data->button_code);
					  else
					    std::cout << "unknown";
					  std::cout << (data->key_status == m_keycodeStatus::DOWN ? " pressed" : " released");
					  std::cout << " on client" << std::endl;
					}
				    }
				}
			}
		}
	}
	return (0);
}
