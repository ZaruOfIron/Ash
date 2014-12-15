#ifndef ___USER_HPP___
#define ___USER_HPP___

#include <string>

struct User
{
	std::string name;
	int correct, wrong, score;
	enum class STATUS {
		FIGHTER, WINNER, LOSER
	} status;

	User()
		: name(), correct(0), wrong(0), score(0), status(STATUS::FIGHTER){}
};



#endif

