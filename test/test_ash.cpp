#include <boost/test/unit_test.hpp>
#include "ash.hpp"

BOOST_AUTO_TEST_CASE( no_script_ash )
{
	Ash ash;
	BOOST_CHECK_THROW(ash.getUser(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE( ash )
{
	Ash ash;
	ash.setScript("script/ash_script.lua");
	const auto& user = ash.getUser(0);
	BOOST_CHECK_EQUAL(user.correct, 1);
	BOOST_CHECK_EQUAL(user.wrong, 2);
	BOOST_CHECK_EQUAL(user.score, 3);
}

