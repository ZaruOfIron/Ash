#ifndef ___ASH_HPP___
#define ___ASH_HPP___

#include "user.hpp"
#include "controle_script.hpp"
#include <memory>
#include <vector>

class View;

class Ash
{
private:
	std::vector<User> users_;
	std::unique_ptr<ControleScript> controler_;
	std::unique_ptr<View> view_;

public:
	Ash();

	void setScript(const std::string& filename);
	const User& getUser(int index);

	void luaInitialize(int answer, int winner, const std::string& title, const std::string& subtitle, int quizId, const User& orgUser);
	void luaUpdate(const UserUpdateMessage& msg);
};


#endif

