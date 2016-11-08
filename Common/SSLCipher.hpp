#ifndef _SSL_HPP_
#define _SSL_HPP_

#include <string>
#include <utility>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <cstring>

#include "ISSL.hpp"

class SSLCipher : public ISSL
{
public:
  SSLCipher();
  virtual ~SSLCipher();
  virtual int compute_cyphered_size(const unsigned char *data, unsigned int);
  virtual std::string	getKey() const;
  virtual void	setKey(const std::string &);
  virtual std::string	getIV() const;
  virtual void	setIV(const std::string &);
  virtual std::pair<unsigned char *, int>	encrypt(const unsigned char *, unsigned int);
  virtual std::pair<unsigned char *, int>	decrypt(const unsigned char *, unsigned int);

protected:
	std::string	m_key;
	std::string m_IV;
};

#endif /*!_SSL_HPP_*/
