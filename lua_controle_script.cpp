#include "lua_controle_script.hpp"
#include <cassert>

LuaControleScript::LuaControleScript(Ash& ash, const std::string& filename)
	: lua_(luaL_newstate(), lua_close), filenaem_(filename), ash_(ash)
{
	// nullでなければ、他にインスタンスが存在する
	assert(thisPtr_ == nullptr);
	thisPtr_ = this;

	// Lua Scriptを初期化
	auto L = lua_.get();
	luaL_openlibs(L);
}

void LuaControleScript::initialize()
{
	auto L = lua_.get();

	window_.reset(new ControleWindow(this));

	// apiを設定
	static const luaL_Reg ash[] = {
		{ "user", &LuaControleScript::luaUser },
		{ NULL, NULL }
	};
	luaL_newlib(L, ash);
	lua_setglobal(L, "ash");

	// initialize()を呼び出す
	// 関数を積む
	lua_getglobal(L, "initialize");
	// 呼び出す
	// lua_pcall(L, 引数, 戻り値, ?)
	if(lua_pcall(L, 0, 0, 0))	throw std::runtime_error(luaL_checkstring(L, -1));
	// 戻り値を取得
}

void LuaControleScript::onCommand(int index, int id)
{
	auto L = lua_.get();

	// on_command()を呼び出す
	// 関数を積む
	lua_getglobal(L, "on_command");
	// 引数を積む
	lua_pushnumber(index);
	lua_pushnumber(id);
	// 呼び出す
	// lua_pcall(L, 引数, 戻り値, ?)
	if(lua_pcall(L, 2, 0, 0))	throw std::runtime_error(luaL_checkstring(L, -1));
}

