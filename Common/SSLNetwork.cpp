#include "SSLNetwork.hpp"

SSLNetwork::SSLNetwork(bool useExistingSocket) :
  TCPClient(useExistingSocket)
{
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);

	this->key = "01234567890123456789012345678901";
	this->IV = "01234567890123456";
}

SSLNetwork::~SSLNetwork()
{
	EVP_cleanup();
	ERR_free_strings();
}

std::string	SSLNetwork::getKey() const
{
  return this->key;
}

void	SSLNetwork::setKey(const std::string &_key)
{
  this->key = _key;
}

std::string	SSLNetwork::getIV() const
{
  return this->IV;
}

void	SSLNetwork::setIV(const std::string &_IV)
{
  this->IV = _IV;
}

std::pair<unsigned char *, int>	SSLNetwork::encrypt(const unsigned char *data, unsigned int data_len)
{
	EVP_CIPHER_CTX *ctx;
	unsigned char *ciphertext = new unsigned char[data_len*2];

	int len = 0;
	int ciphertext_len = 0;
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char *>(this->key.c_str()), reinterpret_cast<const unsigned char *>(this->IV.c_str()));
	EVP_EncryptUpdate(ctx, ciphertext, &len, data, data_len);
	ciphertext_len = len;
	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
	ciphertext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	return std::make_pair(ciphertext, ciphertext_len);
}

int SSLNetwork::compute_cyphered_size(const unsigned char *data, unsigned int data_len)
{
	EVP_CIPHER_CTX *ctx;
	unsigned char *ciphertext = new unsigned char[data_len * 2];

	int len = 0;
	int ciphertext_len = 0;
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char *>(this->key.c_str()), reinterpret_cast<const unsigned char *>(this->IV.c_str()));
	EVP_EncryptUpdate(ctx, ciphertext, &len, data, data_len);
	ciphertext_len = len;
	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
	ciphertext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	delete[] ciphertext;
	return ciphertext_len;
}

std::pair<unsigned char *, int>	SSLNetwork::decrypt(const unsigned char *data, unsigned int data_len)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;
	unsigned char *plaintext = new unsigned char[data_len];

	/* Create and initialise the context */
	ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char *>(this->key.c_str()), reinterpret_cast<const unsigned char *>(this->IV.c_str()));
	EVP_DecryptUpdate(ctx, plaintext, &len, data, data_len);
	plaintext_len = len;
	EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
	plaintext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	return std::make_pair(plaintext, plaintext_len);
}

void SSLNetwork::sendData(const void *data, unsigned int len)
{
	auto result = this->encrypt(reinterpret_cast<const unsigned char *>(data), len);
	if (send(this->m_socketFd, reinterpret_cast<const char *>(result.first), result.second, 0) != result.second)
	{
		std::cout << "Send error" << std::endl;
		m_isConnected = false;
	}
	delete[] result.first;
}

int SSLNetwork::receiveData(void *data, unsigned int len)
{
	int valReaded = 0;

	len = this->compute_cyphered_size(reinterpret_cast<unsigned char *>(data), len);
	if ((valReaded = recv(m_socketFd, reinterpret_cast<char *>(data), len, 0)) == 0)
	{
		m_isConnected = false;
	}

	auto result = this->decrypt(reinterpret_cast<const unsigned char *>(data), len);
	std::memcpy(data, result.first, result.second);
	valReaded = result.second;
	delete[] result.first;
	return valReaded;
}
