#ifndef ___LUA_CONTROLE_SCRIPT_HPP___
#define ___LUA_CONTROLE_SCRIPT_HPP___

#include "controle_script.hpp"
#include <memory>
#include <string>

class LuaControleScript : public ControleScript
{
private:
	std::unique_ptr<lua_State> lua_;
	std::string filename_;
	Ash& ash_;
	static LuaControleScript *thisPtr_;

public:
	LuaControleScript(Ash& ash, const std::string& filename);
	
	void initialize();
};

#endif

