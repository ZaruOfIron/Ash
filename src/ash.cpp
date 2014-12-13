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
	users_.resize(userNumber, orgUser);
}


