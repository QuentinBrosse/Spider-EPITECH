#ifndef _SSL_HPP_
# define _SSL_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <utility>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "TCPClient.hpp"

class	SSLNetwork : public TCPClient
{
private:
  std::string	key;
  std::string   IV;
public:
  SSLNetwork(bool = false);
  ~SSLNetwork();
  virtual void sendData(const void *, unsigned int);
  virtual int receiveData(void *, unsigned int);
  std::string	getKey() const;
  void	setKey(const std::string &);
  std::string	getIV() const;
  void	setIV(const std::string &);
  std::pair<unsigned char *, int>	encrypt(const unsigned char *, unsigned int);
  std::pair<unsigned char *, int>	decrypt(const unsigned char *, unsigned int);
};

#endif /*!_SSL_HPP_*/
