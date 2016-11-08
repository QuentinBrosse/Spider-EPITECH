#ifndef _ISSL_HPP_
# define _ISSL_HPP_

#include <string>
#include <utility>

class	ISSL
{
public:
	virtual ~ISSL() { };
  virtual int compute_cyphered_size(const unsigned char *data, unsigned int) = 0;
  virtual std::string	getKey() const = 0;
  virtual void	setKey(const std::string &) = 0;
  virtual std::string	getIV() const = 0;
  virtual void	setIV(const std::string &) = 0;
  virtual std::pair<unsigned char *, int>	encrypt(const unsigned char *, unsigned int) = 0;
  virtual std::pair<unsigned char *, int>	decrypt(const unsigned char *, unsigned int) = 0;
};

#endif /*!_ISSL_HPP_*/
