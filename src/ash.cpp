#include "ash.hpp"
#include "lua_controle_script.hpp"

Ash::Ash()
	: users_(), controler_()
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
}

void Ash::luaUpdate(const UserUpdateMessage& msg)
{
	std::cout << "Ash::luaUpdate() : index = " << msg.index << std::endl;
	if(msg.name)	std::cout << "\tname    = " << *(msg.name) << std::endl;
	if(msg.correct)	std::cout << "\tcorrect = " << *(msg.correct) << std::endl;
	if(msg.wrong)	std::cout << "\twrong   = " << *(msg.wrong) << std::endl;
	if(msg.score)	std::cout << "\tscore   = " << *(msg.score) << std::endl;
	std::cout << "\t"
	for(int id : msg.info)	std::cout << id << " ";
	std::cout << std::endl;
}

