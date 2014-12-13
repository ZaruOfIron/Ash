#include "lua_controle_script.hpp"
#include "lua_exception.hpp"
#include <cassert>

LuaControleScript *LuaControleScript::thisPtr_ = nullptr;

LuaControleScript::LuaControleScript(Ash& ash, const std::string& filename)
	: lua_(luaL_newstate(), lua_close), filename_(filename), ash_(ash)
{
	// nullでなければ、他にインスタンスが存在する
	assert(thisPtr_ == nullptr);
	thisPtr_ = this;

	// Lua Scriptを初期化
	auto L = lua_.get();
	luaL_openlibs(L);
	if(luaL_loadfile(L, filename_.c_str()) || lua_pcall(L, 0, 0, 0))
		throw LuaCantLoadFileError(luaL_checkstring(L, -1));
}

LuaControleScript::~LuaControleScript()
{
	thisPtr_ = nullptr;
}

void LuaControleScript::initialize()
{
	auto L = lua_.get();

	//window_.reset(new ControleWindow(this));

	// apiを設定
	static const luaL_Reg ash[] = {
		{ "user", &LuaControleScript::luaUser },
		//{ "save", &LuaControleScript::luaSave },
		{ NULL, NULL }
	};
	static const luaL_Reg ash_config[] = {
		//{ "set_quiz_id", &LuaControleScript::luaSetQuizId },
		//{ "set_answer_number", &LuaControleScript::luaSetAnswerNumber },
		//{ "set_title", &LuaControleScript::luaSetTitle },
		//{ "set_subtitle", &LuaControleScript::luaSetSubtitle },
		//{ "create_user_button", &LuaControleScript::luaCreateUserButton },
		//{ "create_system_button", &LuaControleScript::luaCreateSystemButton },
		{ NULL, NULL }
	};

	luaL_newlib(L, ash);
	luaL_newlib(L, ash_config);
	lua_setfield(L, -2, "config");
	lua_setglobal(L, "ash");
	
	// initialize()を呼び出す
	// 関数を積む
	lua_getglobal(L, "initialize");
	// 呼び出す
	// lua_pcall(L, 引数, 戻り値, ?)
	if(lua_pcall(L, 0, 0, 0))	throw LuaCantCallFuncError(luaL_checkstring(L, -1));
	// 戻り値を取得
}

void LuaControleScript::onCommand(int index, int id)
{
	auto L = lua_.get();

	// on_command()を呼び出す
	// 関数を積む
	lua_getglobal(L, "on_command");
	// 引数を積む
	lua_pushnumber(L, index);
	lua_pushnumber(L, id);
	// 呼び出す
	// lua_pcall(L, 引数, 戻り値, ?)
	if(lua_pcall(L, 2, 0, 0))	throw LuaCantCallFuncError(luaL_checkstring(L, -1));
}

int LuaControleScript::luaUser(lua_State *L)
{
	int index = luaL_checkint(L, -1);
	lua_settop(L, 0);	// Clear the stack

	static const luaL_Reg ash_user[] = {
		//{ "correct", &LuaControleScript::luaCorrect },
		//{ "wrong", &LuaControleScript::luaWrong },
		//{ "score", &LuaControleScript::luaScore },
		//{ "add_info", &LuaControleScript::luaAddInfo },
		{ NULL, NULL }
	};

	luaL_newlib(L, ash_user);
	lua_pushnumber(L, index);
	lua_setfield(L, -2, "index");

	return 1;
}

