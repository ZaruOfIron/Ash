#ifndef ___USER_HPP___
#define ___USER_HPP___

#include <string>

struct User
{
	std::string name;
	int correct, wrong, score;
	bool hasWon;

	User()
		: name(), correct(0), wrong(0), score(0), hasWon(false){}
};



#endif

