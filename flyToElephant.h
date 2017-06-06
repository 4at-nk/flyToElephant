#ifndef flyToElephant
#define flyToElephant

#include <iostream>
#include <string>
#include <forward_list>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include <fstream>

//класс ищет в словаре слова, которые отличаются от first на 1 символ.
//для каждого найденного слова запускается своя ветка поиска (добавляется в findersList),
//где first - найденное слово. 
class Finder
{
	const std::string first;							//начальное слово
	const std::string last;								//конечное слово
	std::forward_list<std::string> dictionary;			//словарь
	const std::string path;								//весь путь который прошла данная ветка
	std::forward_list<Finder>& findersList;				//список всех веток поиска
	bool done;											//флаг об окончании своей работы
	std::string answer;									//окончательный путь, если найден

public:
	Finder(std::string _first, std::string _last, const std::forward_list<std::string> _dictionary, 
		std::string _path, std::forward_list<Finder>& _findersList) :
		first(std::move(_first)),
		last(std::move(_last)),
		dictionary(std::move(_dictionary)),
		path(std::move(_path + '\n' + first)),
		findersList(_findersList),
		done(false) {}

	void find()
	{
		while (true)
		{
			auto f = std::find_if(dictionary.begin(), dictionary.end(), [this](const std::string& word) 
			{
				//предполагается, что в функцию не будут передаваться одинаковые слова и слова разной длины
				int differentLetterCount = 0;
				for (auto i = 0; i < first.length(); ++i)
				{
					if (first[i] != word[i])
						++differentLetterCount;
					if (differentLetterCount > 1)
						return false;
				}
				return true;
			});

			if (f != dictionary.end())
			{
				const std::string word = *f;
				if (word == last)
				{
					answer = boost::trim_copy(path) + '\n' + word;
					return;
				}
				//облегчаем словарь, убирая исходное слово
				dictionary.remove(word);
				findersList.push_front(Finder{ word, last, dictionary, path,
					findersList });
			}
			else
				break;
		}
		done = true;
	}

	bool getDone() const { return done; }
	bool gotAnswer() const { return answer.length(); }
	std::string getAnswer() const { return answer; }
};

std::string elephantFromFly(const std::string& wordsPath, const std::string& dictionaryPath)
{
	std::ifstream finWordPath(wordsPath);
	if (!finWordPath)
	{
		std::cerr << "ошибка, не удалось открыть файл со словами\n";
		return std::string{};
	}
	char bufWord[256];
	finWordPath.getline(bufWord, 256);
	std::string fly = boost::locale::conv::from_utf(bufWord, "cp1251");
	finWordPath.getline(bufWord, 256);
	std::string elephant = boost::locale::conv::from_utf(bufWord, "cp1251");
	finWordPath.close();

	auto ToUpper = [](std::string& s) {
		setlocale(LC_CTYPE, "ru_RU.cp1251");
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);	
		setlocale(LC_ALL, "");
	};	

	//для унификации исходное и конечное слово переводим в верхний регистр (то же со словарём)	
	boost::trim(fly);
	ToUpper(fly);
	boost::trim(elephant);
	ToUpper(elephant);	

	int wordSize = fly.length();
	if (!wordSize)
	{
		std::cerr << "ошибка, некорректное исходное слово\n";
		return std::string{};
	}

	if (!elephant.length())
	{
		std::cerr << "ошибка, некорректное конечное слово\n";
		return std::string{};
	}

	if (wordSize != elephant.length())
	{
		std::cerr << "ошибка, слова разной длины\n";
		return std::string{};
	}

	std::ifstream finDicPath(dictionaryPath);
	if (!finDicPath)
	{
		std::cerr << "ошибка, не удалось открыть файл словаря\n";
		return std::string{};
	}

	if (fly == elephant)
	{
		std::cerr << "ошибка, слова одинаковы\n";
		return std::string{};
	}

	std::forward_list<std::string> dictionary;
	while (finDicPath)
	{
		char buf[256];
		finDicPath.getline(buf, 256);
		std::string word = boost::locale::conv::from_utf(buf, "cp1251");
		boost::trim(word);		
		if (word.length() != wordSize)
			continue;
		ToUpper(word);
		dictionary.push_front(std::move(word));
	}
	finDicPath.close();

	auto gotDictionaryElephant = std::find(dictionary.begin(), dictionary.end(), elephant);
	if (gotDictionaryElephant == dictionary.end())
	{
		std::cerr << "ошибка, в словаре отсутствует искомое слово\n";
		return std::string{};
	}
	//избавляемся от дубликатов в словаре
	dictionary.sort();
	dictionary.unique();
	//облегчаем словарь, убирая исходное слово
	dictionary.remove(fly);

	//будет хранить все ветки поиска
	std::forward_list<Finder> findersList;
	//добавляем первую ветку поиска
	findersList.push_front(Finder{ fly, elephant, dictionary, "", findersList });

	//пока есть потенциально успешные ветки поиска
	while (!findersList.empty())
	{
		//запускаем поиск по имеющимся веткам
		std::for_each(findersList.begin(), findersList.end(), [](Finder& f) {f.find();});
		//удаляем отработавшие ветки
		findersList.remove_if([](const Finder& f) {return f.getDone();});
		//проверяем не дошла ли какая ветка до конца
		auto gotAnswer = std::find_if(findersList.begin(), findersList.end(), [](const Finder& f) 
			{return f.gotAnswer();});
		//если какая-то ветка завершилась успехом, то возвращаем результат
		if (gotAnswer != findersList.end())
			return (*gotAnswer).getAnswer();
	}

	std::cerr << "не удалось построить цепочку\n";
	return std::string{};
}

#endif
