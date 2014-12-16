#include <boost/test/unit_test.hpp>
#include "ash.hpp"
#include "lua_controle_script.hpp"
#include "lua_exception.hpp"

#include <iostream>

BOOST_AUTO_TEST_CASE( no_script )
{
	Ash ash;
	BOOST_CHECK_THROW(LuaControleScript(ash, ""), LuaCantLoadFileError);
}

BOOST_AUTO_TEST_CASE( empty_script )
{
	Ash ash;
	LuaControleScript controle(ash, "script/empty_script.lua");
	BOOST_CHECK_THROW(controle.initialize(), LuaCantCallFuncError);
	BOOST_CHECK_THROW(controle.onCommand(0, 0), LuaCantCallFuncError);
}

BOOST_AUTO_TEST_CASE( simplest_script )
{
	Ash ash;
	LuaControleScript controle(ash, "script/simplest_script.lua");
	BOOST_CHECK_NO_THROW(controle.initialize());
	BOOST_CHECK_NO_THROW(controle.onCommand(1, 1));
}

BOOST_AUTO_TEST_CASE( get_user )
{
	Ash ash;
	LuaControleScript controle(ash, "script/get_user.lua");
	BOOST_CHECK_NO_THROW(controle.initialize());
	BOOST_CHECK_NO_THROW(controle.onCommand(1, 1));
}

BOOST_AUTO_TEST_CASE( set_user )
{
	Ash ash;
	LuaControleScript controle(ash, "script/set_user.lua");
	BOOST_CHECK_NO_THROW(controle.initialize());
	BOOST_CHECK_NO_THROW(controle.onCommand(1, 1));
}

BOOST_AUTO_TEST_CASE( get_set_user )
{
	Ash ash;
	LuaControleScript controle(ash, "script/get_set_user.lua");
	BOOST_CHECK_NO_THROW(controle.initialize());
	BOOST_CHECK_NO_THROW(controle.onCommand(1, 1));
}

BOOST_AUTO_TEST_CASE( create_user_button )
{
	Ash ash;
	LuaControleScript controle(ash, "script/create_user_button.lua");
	BOOST_CHECK_NO_THROW(controle.initialize());
	BOOST_CHECK_NO_THROW(controle.onCommand(1, 1));
	std::cout << "Check from create_user_button" << std::endl;
	//controle.run();
}

BOOST_AUTO_TEST_CASE( create_system_button )
{
	Ash ash;
	LuaControleScript controle(ash, "script/create_system_button.lua");
	BOOST_CHECK_NO_THROW(controle.initialize());
	BOOST_CHECK_NO_THROW(controle.onCommand(1, 1));
	std::cout << "Check from create_system_button" << std::endl;
	//controle.run();
}

BOOST_AUTO_TEST_CASE( set_user_button_state )
{
	Ash ash;
	LuaControleScript controle(ash, "script/set_user_button_state.lua");
	BOOST_CHECK_NO_THROW(controle.initialize());
	std::cout << "Check from set_user_button_state" << std::endl;
	controle.run();
}

BOOST_AUTO_TEST_CASE( set_system_button_state )
{
	Ash ash;
	LuaControleScript controle(ash, "script/set_system_button_state.lua");
	BOOST_CHECK_NO_THROW(controle.initialize());
	std::cout << "Check from set_system_button_state" << std::endl;
	controle.run();
}
