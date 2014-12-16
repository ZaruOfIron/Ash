#ifndef ___LUA_CONTROLE_SCRIPT_HPP___
#define ___LUA_CONTROLE_SCRIPT_HPP___

#include "controle_script.hpp"
#include <lua.hpp>
#include <memory>
#include <string>
#include <vector>

class Ash;
class ControleWindowFrame;

class LuaControleScript : public ControleScript
{
private:
	std::unique_ptr<lua_State, decltype(&lua_close)> lua_;
	std::unique_ptr<ControleWindowFrame> window_;
	std::string filename_;
	Ash& ash_;
	static LuaControleScript *thisPtr_;
	int answer_;
	std::vector<std::string> trackingVars_;

	static bool checkboolean(lua_State *L, int index);

	static int luaGetUser(lua_State *L);
	static int luaSetUser(lua_State *L);
	static int luaCreateUserButton(lua_State *L);
	static int luaCreateSystemButton(lua_State *L);
	static int luaSetUserButtonState(lua_State *L);
	static int luaSetSystemButtonState(lua_State *L);
	static int luaAddTrackingVar(lua_State *L);

public:
	LuaControleScript(Ash& ash, const std::string& filename);
	~LuaControleScript();
	
	void initialize();
	void run();
	void getSaveData(std::ostream& os);
	void restoreSaveData(std::istream& is);

	void onUserButton(int index, int id);
	void onSystemButton(int id);
	void onName(int index, const std::string& name);
};

#endif

