#include <boost/test/unit_test.hpp>
#include "../lua_controle_script.hpp"

BOOST_AUTO_TEST_CASE( script1 )
{
	Ash ash;
	LuaControleScript controle(ash, "test/script1.lua");
	BOOST_CHECK_THROW(controle.initialize(), std::exception);
	BOOST_CHECK_THROW(controle.onCommand(0, 0), std::exception);
}

