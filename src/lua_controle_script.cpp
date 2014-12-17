#include "lua_controle_script.hpp"
#include "lua_exception.hpp"
#include "ash.hpp"
#include "controle_window.hpp"
#include <boost/serialization/map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <cassert>
#include <map>


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

	window_.reset(new ControleWindow(this));

	// apiを設定
	static const luaL_Reg ash[] = {
		{ "get_user", &LuaControleScript::luaGetUser },
		{ "set_user", &LuaControleScript::luaSetUser },
		{ "set_user_button_state", &LuaControleScript::luaSetUserButtonState },
		{ "set_system_button_state", &LuaControleScript::luaSetSystemButtonState },
		{ "save", &LuaControleScript::luaSave },
		{ NULL, NULL }
	};
	static const luaL_Reg ash_config[] = {
		{ "create_user_button", &LuaControleScript::luaCreateUserButton },
		{ "create_system_button", &LuaControleScript::luaCreateSystemButton },
		{ "add_tracking_var", &LuaControleScript::luaAddTrackingVar },
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

	window_->setAnswer(answer_);
	window_->Create();
}

void LuaControleScript::getSaveData(std::ostream& os)
{
	auto L = lua_.get();

	std::map<std::string, int> data;	// intに決めうち
	// グローバル変数の入ったテーブルを持ってくる
	lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
	lua_pushnil(L);
	while(lua_next(L, -2)){
		// -2: key, -1: value
		// trackingVars_の中にある変数名だったら
		auto it = std::find(trackingVars_.begin(), trackingVars_.end(), std::string(luaL_checkstring(L, -2)));
		if(it != trackingVars_.end()){
			data.insert(std::make_pair(*it, luaL_checkint(L, -1)));
		}

		lua_pop(L, 1);
	}

	boost::archive::text_oarchive oa(os);
	oa << data;
}

void LuaControleScript::restoreSaveData(std::istream& is)
{
	auto L = lua_.get();

	std::map<std::string, int> data;
	boost::archive::text_iarchive ia(is);
	ia >> data;

	for(auto it = data.begin();it != data.end();it++){
		lua_pushnumber(L, it->second);
		lua_setglobal(L, it->first.c_str());
	}
}

void LuaControleScript::onUserButton(int index, int id)
{
	auto L = lua_.get();

	// 終わっていれば押されても何もしない
	if(ash_.hasFinished())	return;

	// 対象外であれば呼ばない
	if(ash_.getUser(index - 1).status != User::STATUS::FIGHTER)
		return;

	// 関数を積む
	lua_getglobal(L, "on_user_button");
	// 引数を積む
	lua_pushnumber(L, index);
	lua_pushnumber(L, id);
	// 呼び出す
	// lua_pcall(L, 引数, 戻り値, ?)
	if(lua_pcall(L, 2, 0, 0))	throw LuaCantCallFuncError(luaL_checkstring(L, -1));
}

void LuaControleScript::onSystemButton(int id)
{
	auto L = lua_.get();
	
	// 終わっていれば押されても何もしない
	if(ash_.hasFinished())	return;

	// 関数を積む
	lua_getglobal(L, "on_system_button");
	// 引数を積む
	lua_pushnumber(L, id);
	// 呼び出す
	// lua_pcall(L, 引数, 戻り値, ?)
	if(lua_pcall(L, 1, 0, 0))	throw LuaCantCallFuncError(luaL_checkstring(L, -1));
}

void LuaControleScript::onName(int index, const std::string& name)
{
	UserUpdateMessage msg;
	msg.index = index - 1;
	msg.name = name;

	thisPtr_->ash_.luaUpdate(msg);
}

bool LuaControleScript::checkboolean(lua_State *L, int index)
{
	assert(lua_isboolean(L, index));
	return lua_toboolean(L, index);
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

int LuaControleScript::luaSetUserButtonState(lua_State *L)
{
	thisPtr_->window_->setUserButtonState(luaL_checkint(L, 1), luaL_checkint(L, 2), checkboolean(L, 3));
}

int LuaControleScript::luaSetSystemButtonState(lua_State *L)
{
	thisPtr_->window_->setSystemButtonState(luaL_checkint(L, 1), checkboolean(L, 2));
}

int LuaControleScript::luaAddTrackingVar(lua_State *L)
{
	thisPtr_->trackingVars_.push_back(luaL_checkstring(L, 1));
}

int LuaControleScript::luaSave(lua_State *L)
{
	thisPtr_->ash_.save();
}

