#pragma once
#ifndef ___COPYDATA_VIEW_HPP___
#define ___COPYDATA_VIEW_HPP___

#include "view.hpp"
#include <wincore.h>

class CopyDataView : public View
{
private:
	HWND hTarget_;

	void sendData(int id, const std::string& str);

public:
	CopyDataView(const std::string& targetTitle);
	~CopyDataView(){}

	bool isTargetValid();
	void initialize(int answerNumber, int winnerNumber, const std::string& title, const std::string& subtitle, int quizId);
	void sendUserModified(int index, const User& user, int modIndex);
	void sendInfo(int id);
	void sendInfo(const std::string& str);
};



#endif

