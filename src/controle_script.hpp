#ifndef ___CONTROLE_SCRIPT_HPP___
#define ___CONTROLE_SCRIPT_HPP___

#include <boost/optional.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/optional.hpp>
#include <vector>
#include <string>

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
	virtual void getSaveData(std::ostream& os) = 0;
	virtual void restoreSaveData(std::istream& is) = 0;
};


#endif

