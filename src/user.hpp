#ifndef ___USER_HPP___
#define ___USER_HPP___

#include <string>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>

struct User
{
	std::string name;
	int correct, wrong, score;
	enum class STATUS {
		FIGHTER, WINNER, LOSER
	} status;

	User()
		: name(), correct(0), wrong(0), score(0), status(STATUS::FIGHTER){}

private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & name & correct & wrong & score & status;
		}
};



#endif

