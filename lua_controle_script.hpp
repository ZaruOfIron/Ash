#ifndef ___LUA_CONTROLE_SCRIPT_HPP___
#define ___LUA_CONTROLE_SCRIPT_HPP___

#include "controle_script.hpp"
#include <memory>
#include <string>

struct lua_State;
class Ash;
class ControleWindow;

class LuaControleScript : public ControleScript
{
private:
	std::unique_ptr<lua_State> lua_;
	std::unique_ptr<ControleWindow> window_;
	std::string filename_;
	Ash& ash_;
	static LuaControleScript *thisPtr_;

public:
	LuaControleScript(Ash& ash, const std::string& filename);
	
	void initialize();
	void onCommand(int index, int id);
};

#endif

