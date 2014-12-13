#include <boost/test/unit_test.hpp>
#include "../ash.hpp"
#include "../lua_controle_script.hpp"
#include "../lua_exception.hpp"

BOOST_AUTO_TEST_CASE( no_script )
{
	Ash ash;
	BOOST_CHECK_THROW(LuaControleScript(ash, ""), LuaCantLoadFileError);
}

BOOST_AUTO_TEST_CASE( empty_script )
{
	Ash ash;
	LuaControleScript controle(ash, "test/empty_script.lua");
	BOOST_CHECK_THROW(controle.initialize(), LuaCantCallFuncError);
	BOOST_CHECK_THROW(controle.onCommand(0, 0), LuaCantCallFuncError);
}


