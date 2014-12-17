#pragma once
#ifndef ___VIEW_HPP___
#define ___VIEW_HPP___

#include "user.hpp"
#include <string>

class View
{
public:
	View(){}
	virtual ~View(){}

	virtual void initialize(int answerNumber, int winnerNumber, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser) = 0;
	virtual void sendUserModified(int index, const User& user, int modIndex) = 0;
	virtual void sendInfo(int id) = 0;
	virtual void sendInfo(const std::string& str) = 0;
};

#endif

