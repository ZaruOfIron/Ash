#include "ash.hpp"
#include "lua_controle_script.hpp"
#include "copydata_view.hpp"

//#include <iostream>

Ash::Ash()
	: users_(), controler_(), view_(new CopyDataView("AUAUA"))
{
}

void Ash::setScript(const std::string& filename)
{
	controler_.reset(new LuaControleScript(*this, filename));
	controler_->initialize();
}

const User& Ash::getUser(int index)
{
	return users_.at(index);
}

void Ash::luaInitialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser)
{
	users_.resize(answer, orgUser);

	view_->initialize(answer, winner, title, subtitle, quizId);

	for(int i = 0;i < answer;i++){
		view_->sendUserModified(i, orgUser, 0x0f);
	}
}

void Ash::luaUpdate(const UserUpdateMessage& msg)
{
/*	std::cout << "Ash::luaUpdate() : index = " << msg.index << std::endl;
	if(msg.name)	std::cout << "\tname    = " << *(msg.name) << std::endl;
	if(msg.correct)	std::cout << "\tcorrect = " << *(msg.correct) << std::endl;
	if(msg.wrong)	std::cout << "\twrong   = " << *(msg.wrong) << std::endl;
	if(msg.score)	std::cout << "\tscore   = " << *(msg.score) << std::endl;
	std::cout << "\t";
	for(int id : msg.info)	std::cout << id << " ";
	std::cout << std::endl;
	*/

	auto& user = users_.at(msg.index);
	int modIndex = 0;
	if(msg.name){
		user.name = *(msg.name);
		modIndex |= 1 << 0;
	}
	if(msg.correct){
		user.correct = *(msg.correct);
		modIndex |= 1 << 1;
	}
	if(msg.wrong){
		user.wrong = *(msg.wrong);
		modIndex |= 1 << 2;
	}
	if(msg.score){
		user.score = *(msg.score);
		modIndex |= 1 << 3;
	}

	view_->sendUserModified(msg.index, user, modIndex);

	for(int id : msg.info){
		// Ÿ‚¿”²‚¯
		view_->sendInfo(id);
	}
}

