#include <cstring>
#include "SSLTCPClient.hpp"

SSLTCPClient::SSLTCPClient(bool useExistingSocket) :
	TCPClient(useExistingSocket), SSLCipher()
{

}

SSLTCPClient::~SSLTCPClient()
{
	
}

void SSLTCPClient::sendData(const void *data, unsigned int len)
{
	auto result = this->encrypt(reinterpret_cast<const unsigned char *>(data), len);
	if (send(this->m_socketFd, reinterpret_cast<const char *>(result.first), result.second, 0) != result.second)
	{
		std::cout << "Send error" << std::endl;
		m_isConnected = false;
	}
	delete[] result.first;
}

int SSLTCPClient::receiveData(void *data, unsigned int len)
{
	int valReaded = 0;

	len = this->compute_cyphered_size(reinterpret_cast<unsigned char *>(data), len);
	if ((valReaded = recv(m_socketFd, reinterpret_cast<char *>(data), len, 0)) == 0)
	{
		m_isConnected = false;
	}

	auto result = this->decrypt(reinterpret_cast<const unsigned char *>(data), len);
	std::memcpy(data, result.first, result.second);
	if (valReaded > 0)
		valReaded = result.second;
	delete[] result.first;
	return valReaded;
}
