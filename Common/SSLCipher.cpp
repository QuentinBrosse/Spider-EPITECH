#include "Protocol.hpp"
#include "SSLCipher.hpp"

SSLCipher::SSLCipher()
{
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);

	this->m_key = ssl_key;
	this->m_IV = ssl_iv;
}

SSLCipher::~SSLCipher()
{
	EVP_cleanup();
	ERR_free_strings();
}

std::string	SSLCipher::getKey() const
{
  return this->m_key;
}

void	SSLCipher::setKey(const std::string &_key)
{
  this->m_key = _key;
}

std::string	SSLCipher::getIV() const
{
  return this->m_IV;
}

void	SSLCipher::setIV(const std::string &_IV)
{
  this->m_IV = _IV;
}

std::pair<unsigned char *, int>	SSLCipher::encrypt(const unsigned char *data, unsigned int data_len)
{
	EVP_CIPHER_CTX *ctx;
	unsigned char *ciphertext = new unsigned char[data_len*2];

	int len = 0;
	int ciphertext_len = 0;
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char *>(this->m_key.c_str()), reinterpret_cast<const unsigned char *>(this->m_IV.c_str()));
	EVP_EncryptUpdate(ctx, ciphertext, &len, data, data_len);
	ciphertext_len = len;
	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
	ciphertext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	return std::make_pair(ciphertext, ciphertext_len);
}

int SSLCipher::compute_cyphered_size(const unsigned char *data, unsigned int data_len)
{
	EVP_CIPHER_CTX *ctx;
	unsigned char *ciphertext = new unsigned char[data_len * 2];

	int len = 0;
	int ciphertext_len = 0;
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char *>(this->m_key.c_str()), reinterpret_cast<const unsigned char *>(this->m_IV.c_str()));
	EVP_EncryptUpdate(ctx, ciphertext, &len, data, data_len);
	ciphertext_len = len;
	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
	ciphertext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	delete[] ciphertext;
	return ciphertext_len;
}

std::pair<unsigned char *, int>	SSLCipher::decrypt(const unsigned char *data, unsigned int data_len)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;
	unsigned char *plaintext = new unsigned char[data_len];

	ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char *>(this->m_key.c_str()), reinterpret_cast<const unsigned char *>(this->m_IV.c_str()));
	EVP_DecryptUpdate(ctx, plaintext, &len, data, data_len);
	plaintext_len = len;
	EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
	plaintext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	return std::make_pair(plaintext, plaintext_len);
}