#ifndef ___LUA_CONTROLE_SCRIPT_HPP___
#define ___LUA_CONTROLE_SCRIPT_HPP___

#include "controle_script.hpp"
#include <lua.hpp>
#include <memory>
#include <string>

class Ash;
class ControleWindow;

class LuaControleScript : public ControleScript
{
private:
	std::unique_ptr<lua_State, decltype(&lua_close)> lua_;
	//std::unique_ptr<ControleWindow> window_;
	std::string filename_;
	Ash& ash_;
	static LuaControleScript *thisPtr_;

	static int luaGetUser(lua_State *L);
	static int luaSetUser(lua_State *L);

public:
	LuaControleScript(Ash& ash, const std::string& filename);
	~LuaControleScript();
	
	void initialize();
	void onCommand(int index, int id);

};

#endif

