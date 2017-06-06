#include <iostream>
#include <boost/locale.hpp>
#include "flyToElephant.h"

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cerr << "ошибка, недостаточно аргументов\n";
		return 1;
	}
	
	std::string answer = elephantFromFly(argv[1], argv[2]);
	
	if(answer.length())
		std::cout << boost::locale::conv::to_utf<char>(answer, "cp1251") << '\n';

	return 0;
}
