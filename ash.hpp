#ifndef ___ASH_HPP___
#define ___ASH_HPP___

#include "user.hpp"
#include "controle_script.hpp"
#include <memory>
#include <vector>

class Ash
{
private:
	std::vector<User> users_;
	std::unique_ptr<ControleScript> controler_;

public:
	Ash();


};


#endif

