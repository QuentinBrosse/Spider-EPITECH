#ifndef _SSLTCPCLIENT_HPP_
# define _SSLTCPCLIENT_HPP_

#include <string>
#include <utility>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "SSLCipher.hpp"
#include "TCPClient.hpp"

class	SSLTCPClient : public TCPClient, public SSLCipher
{
public:
	SSLTCPClient(bool = false);
	virtual ~SSLTCPClient();
	virtual void sendData(const void *, unsigned int);
	virtual int receiveData(void *, unsigned int);
};

#endif /*!_SSLTCPCLIENT_HPP_*/
