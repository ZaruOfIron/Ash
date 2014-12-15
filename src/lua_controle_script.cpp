#include "lua_controle_script.hpp"
#include "lua_exception.hpp"
#include "ash.hpp"
#include "controle_window.hpp"
#include <cassert>

LuaControleScript *LuaControleScript::thisPtr_ = nullptr;

LuaControleScript::LuaControleScript(Ash& ash, const std::string& filename)
	: lua_(luaL_newstate(), lua_close), filename_(filename), ash_(ash)
{
	// nullでなければ、他にインスタンスが存在する
	assert(thisPtr_ == nullptr);

	// Lua Scriptを初期化
	auto L = lua_.get();
	luaL_openlibs(L);
	if(luaL_loadfile(L, filename_.c_str()) || lua_pcall(L, 0, 0, 0))
		throw LuaCantLoadFileError(luaL_checkstring(L, -1));

	// コンストラクタ内で例外が飛んだときのために
	// 代入は一番最後
	thisPtr_ = this;
}

LuaControleScript::~LuaControleScript()
{
	thisPtr_ = nullptr;
}

void LuaControleScript::initialize()
{
	auto L = lua_.get();

	window_.reset(new ControleWindowFrame(this));

	// apiを設定
	static const luaL_Reg ash[] = {
		{ "get_user", &LuaControleScript::luaGetUser },
		{ "set_user", &LuaControleScript::luaSetUser },
		{ NULL, NULL }
	};
	static const luaL_Reg ash_config[] = {
		{ "create_user_button", &LuaControleScript::luaCreateUserButton },
		{ "create_system_button", &LuaControleScript::luaCreateSystemButton },
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
	if(lua_pcall(L, 0, 1, 0))	throw LuaCantCallFuncError(luaL_checkstring(L, -1));
	// 戻り値を取得
	lua_getfield(L, 1, "org_user");

	lua_getfield(L, 1, "answer");
	answer_ = luaL_checkint(L, -1);
	lua_getfield(L, 1, "winner");
	int winner = luaL_checkint(L, -1);
	lua_getfield(L, 1, "title");
	std::string title = luaL_checkstring(L, -1);
	lua_getfield(L, 1, "subtitle");
	std::string subtitle = luaL_checkstring(L, -1);
	lua_getfield(L, 1, "quizid");
	int quizId = luaL_checkint(L, -1);

	User orgUser;
	lua_getfield(L, 2, "correct");
	orgUser.correct = luaL_checkint(L, -1);
	lua_getfield(L, 2, "wrong");
	orgUser.wrong = luaL_checkint(L, -1);
	lua_getfield(L, 2, "score");
	orgUser.score = luaL_checkint(L, -1);

	// ash.configをnilにしておく
	lua_getglobal(L, "ash");
	lua_pushnil(L);
	lua_setfield(L, -2, "config");
	lua_setglobal(L, "ash");

	ash_.luaInitialize(answer_, winner, title, subtitle, quizId, orgUser);
}

void LuaControleScript::run()
{
	window_->run(answer_);
}

void LuaControleScript::onCommand(int index, int id)
{
	auto L = lua_.get();

	// 対象外であれば呼ばない
	if(ash_.getUser(index - 1).status != User::STATUS::FIGHTER)
		return;

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

void LuaControleScript::onCommand(int index, const std::string& name)
{
	UserUpdateMessage msg;
	msg.index = index - 1;
	msg.name = name;

	thisPtr_->ash_.luaUpdate(msg);
}

int LuaControleScript::luaGetUser(lua_State *L)
{
	int index = luaL_checkint(L, -1);
	auto& user = thisPtr_->ash_.getUser(index - 1);
	lua_settop(L, 0);	// Clear the stack

	// 対象外のユーザーであればnilを返す
	if(user.status != User::STATUS::FIGHTER){
		lua_pushnil(L);
		return 1;
	}

	lua_newtable(L);
	lua_pushnumber(L, index);
	lua_setfield(L, -2, "index");
	lua_pushnumber(L, user.correct);
	lua_setfield(L, -2, "correct");
	lua_pushnumber(L, user.wrong);
	lua_setfield(L, -2, "wrong");
	lua_pushnumber(L, user.score);
	lua_setfield(L, -2, "score");

	return 1;
}

int LuaControleScript::luaSetUser(lua_State *L)
{
	// 1: index, 2: data, 3: info

	// 引数の数を取得する
	int argCount = lua_gettop(L);

	UserUpdateMessage msg;
	msg.index = luaL_checkint(L, 1) - 1;

	lua_getfield(L, 2, "correct");
	if(!lua_isnil(L, -1))	msg.correct = luaL_checkint(L, -1);
	lua_getfield(L, 2, "wrong");
	if(!lua_isnil(L, -1))	msg.wrong = luaL_checkint(L, -1);
	lua_getfield(L, 2, "score");
	if(!lua_isnil(L, -1))	msg.score = luaL_checkint(L, -1);

	// テーブルの中身を全部引っ張り出す
	if(argCount == 3){	// 第三引数があれば
		lua_pushnil(L);
		while(lua_next(L, 3)){
			msg.info.push_back(luaL_checkint(L, -1));
			lua_pop(L, 1);
		}
	}

	thisPtr_->ash_.luaUpdate(msg);

	return 0;
}

int LuaControleScript::luaCreateUserButton(lua_State *L)
{
	thisPtr_->window_->registerUserButton(ButtonData(luaL_checkint(L, 1), luaL_checkstring(L, 2)));
}

int LuaControleScript::luaCreateSystemButton(lua_State *L)
{
	thisPtr_->window_->registerSystemButton(ButtonData(luaL_checkint(L, 1), luaL_checkstring(L, 2)));
}

