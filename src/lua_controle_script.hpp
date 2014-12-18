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
	std::unique_ptr<ControleWindow> window_;
	std::string filename_;
	Ash& ash_;
	static LuaControleScript *thisPtr_;
	int answer_;

public:
	LuaControleScript(Ash& ash, const std::string& filename);
	~LuaControleScript();
	
	// Call from Ash
	void initialize();
	void getSaveData(std::ostream& os);
	void restoreSaveData(std::istream& is);

	// Call from ControleWindow
	void onUserButton(int index, int id);
	void onSystemButton(int id);
	void onName(int index, const std::string& name);

private:
	static bool checkboolean(lua_State *L, int index);

	static int luaGetUser(lua_State *L);
	static int luaSetUser(lua_State *L);
	static int luaCreateUserButton(lua_State *L);
	static int luaCreateSystemButton(lua_State *L);
	static int luaSave(lua_State *L);
};

#endif

