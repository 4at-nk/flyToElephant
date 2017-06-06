#include <boost/test/minimal.hpp>
#include <boost/locale.hpp>
#include "flyToElephant.h"

int test_main(int argc, char* argv[])
{
	BOOST_CHECK(boost::locale::conv::to_utf<char>(
		elephantFromFly("test/word1_1.txt", "test/dic1.txt"), "cp1251") == 		
			"МУХА\n"
			"МУРА\n"
			"ТУРА\n"
			"ТАРА\n"
			"КАРА\n"
			"КАРЕ\n"
			"КАФЕ\n"
			"КАФР\n"
			"КАЮР\n"
			"КАЮК\n"
			"КРЮК\n"
			"УРЮК\n"
			"УРОК\n"
			"СРОК\n"
			"СТОК\n"
			"СТОН\n"
			"СЛОН");
	BOOST_CHECK(boost::locale::conv::to_utf<char>(
		elephantFromFly("test/word1_2.txt", "test/dic1.txt"), "cp1251") == 
			"МИГ\n"
			"МАГ\n"
			"МАЙ\n"
			"ЧАЙ\n"
			"ЧАД\n"
			"ГАД\n"
			"ГОД\n"
			"БОД\n"
			"БОА\n"
			"БРА\n"
			"ЭРА");	
	BOOST_CHECK(boost::locale::conv::to_utf<char>(
		elephantFromFly("test/word1_3.txt", "test/dic1.txt"), "cp1251") == 
			"ГОД\n"
			"ГИД\n"
			"ВИД\n"
			"ВИС\n"
			"ВЕС\n"
			"ВЕК");		
	BOOST_CHECK(boost::locale::conv::to_utf<char>(
		elephantFromFly("test/word1_4.txt", "test/dic1.txt"), "cp1251") == 
			"КОЗА\n"
			"ПОЗА\n"
			"ПОЛА\n"
			"ПОЛК\n"
			"ВОЛК");	
	BOOST_CHECK(boost::locale::conv::to_utf<char>(
		elephantFromFly("test/word2_1.txt", "test/dic2.txt"), "cp1251") == 
			"КОТ\n"
			"ТОТ\n"
			"ТОН");

	return 0;
}
