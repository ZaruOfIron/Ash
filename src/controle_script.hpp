#ifndef ___CONTROLE_SCRIPT_HPP___
#define ___CONTROLE_SCRIPT_HPP___

#include <boost/optional.hpp>
#include <vector>

struct UserUpdateMessage
{
	int index;
	boost::optional<std::string> name;
	boost::optional<int> correct, wrong, score;
	std::vector<int> info;
};

class ControleScript
{
public:
	ControleScript(){}
	virtual ~ControleScript(){}

	virtual void initialize() = 0;
	virtual void run() = 0;

};


#endif

