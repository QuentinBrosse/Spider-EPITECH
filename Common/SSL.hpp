#ifndef _SSL_HPP_
# define _SSL_HPP_

#include <string>
#include <iostream>
#include <fstream>

#include "openssl/blowfish.h"
#include "openssl/evp.h"

class	SSL
{
private:
  std::string	pubKey;
  std::string	privKey;
  std::string   IV;
public:
  SSL();
  ~SSL() {}
  std::string	getPubKey() const;
  void	setPubKey(const std::string &);
  std::string	getPrivKey() const;
  void	setPrivKey(const std::string &);
  std::string	getIV() const;
  void	setIV(const std::string &);
  int	encrypt(int, int);
  int	decrypt(int, int);
};

#endif /*!_SSL_HPP_*/
