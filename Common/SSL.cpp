#include "SSL.cpp"

SSL::SSL()
{
  ifstream      filepub("../Key/public.pem", ios::in);
  ifstream      filepriv("../Key/private.pem", ios::in);
  char		caractere;

  if(filepub)
    {
      while (filepub.get(caractere))
	this->pubkey = caractere;
      filepub.close();
    }
  else
    std::cerr << "Cannot open public Key file !" << std::endl;
  if(filepriv)
    {
      while (filepriv.get(caractere))
	this->privkey = caractere;
      filepriv.close();
    }
  else
    std::cerr << "Cannot open private Key file !" << std::endl;
}

std::string	SSL::getPubKey() const
{
  return this->pubKey;
}

void	SSL::setPubKey(const std::string &_pubKey)
{
  this->pubKey = _pubKey;
}

std::string	SSL::getPrivKey() const
{
  return this->privKey;
}

void	SSL::setPrivKey(const std::string &_privKey)
{
  this->privKey = _privKey;
}

std::string	SSL::getIV() const
{
  return this->IV;
}

void	SSL::setIV(const std::string &_IV)
{
  this->IV = _IV;
}
