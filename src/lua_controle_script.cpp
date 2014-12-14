#include "lua_controle_script.hpp"
#include "lua_exception.hpp"
#include "ash.hpp"
#include <cassert>

LuaControleScript *LuaControleScript::thisPtr_ = nullptr;

LuaControleScript::LuaControleScript(Ash& ash, const std::string& filename)
	: lua_(luaL_newstate(), lua_close), filename_(filename), ash_(ash)
{
	// null�łȂ���΁A���ɃC���X�^���X�����݂���
	assert(thisPtr_ == nullptr);

	// Lua Script��������
	auto L = lua_.get();
	luaL_openlibs(L);
	if(luaL_loadfile(L, filename_.c_str()) || lua_pcall(L, 0, 0, 0))
		throw LuaCantLoadFileError(luaL_checkstring(L, -1));

	// �R���X�g���N�^���ŗ�O����񂾂Ƃ��̂��߂�
	// ����͈�ԍŌ�
	thisPtr_ = this;
}

LuaControleScript::~LuaControleScript()
{
	thisPtr_ = nullptr;
}

void LuaControleScript::initialize()
{
	auto L = lua_.get();

	//window_.reset(new ControleWindow(this));

	// api��ݒ�
	static const luaL_Reg ash[] = {
		{ "get_user", &LuaControleScript::luaGetUser },
		{ "set_user", &LuaControleScript::luaSetUser },
		{ NULL, NULL }
	};
	static const luaL_Reg ash_config[] = {
	//	{ "create_user_button", &LuaControleScript::luaCreateUserButton },
	//	{ "create_system_button", &LuaControleScript::luaCreateSystemButton },
		{ NULL, NULL }
	};

	luaL_newlib(L, ash);
	luaL_newlib(L, ash_config);
	lua_setfield(L, -2, "config");
	lua_setglobal(L, "ash");
	
	// initialize()���Ăяo��
	// �֐���ς�
	lua_getglobal(L, "initialize");
	// �Ăяo��
	// lua_pcall(L, ����, �߂�l, ?)
	if(lua_pcall(L, 0, 1, 0))	throw LuaCantCallFuncError(luaL_checkstring(L, -1));
	// �߂�l���擾
	lua_getfield(L, 1, "org_user");

	lua_getfield(L, 1, "answer");
	int answer = luaL_checkint(L, -1);
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

	ash_.luaInitialize(answer, winner, title, subtitle, quizId, orgUser);

}

void LuaControleScript::onCommand(int index, int id)
{
	auto L = lua_.get();

	// on_command()���Ăяo��
	// �֐���ς�
	lua_getglobal(L, "on_command");
	// ������ς�
	lua_pushnumber(L, index);
	lua_pushnumber(L, id);
	// �Ăяo��
	// lua_pcall(L, ����, �߂�l, ?)
	if(lua_pcall(L, 2, 0, 0))	throw LuaCantCallFuncError(luaL_checkstring(L, -1));
}

int LuaControleScript::luaGetUser(lua_State *L)
{
	int index = luaL_checkint(L, -1) - 1;
	auto& user = thisPtr_->ash_.getUser(index);
	lua_settop(L, 0);	// Clear the stack

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

	// �����̐����擾����
	int argCount = lua_gettop(L);

	UserUpdateMessage msg;
	msg.index = luaL_checkint(L, 1) - 1;

	lua_getfield(L, 2, "correct");
	if(!lua_isnil(L, -1))	msg.correct = luaL_checkint(L, -1);
	lua_getfield(L, 2, "wrong");
	if(!lua_isnil(L, -1))	msg.wrong = luaL_checkint(L, -1);
	lua_getfield(L, 2, "score");
	if(!lua_isnil(L, -1))	msg.score = luaL_checkint(L, -1);

	// �e�[�u���̒��g��S����������o��
	if(argCount == 3){	// ��O�����������
		lua_pushnil(L);
		while(lua_next(L, 3)){
			msg.info.push_back(luaL_checkint(L, -1));
			lua_pop(L, 1);
		}
	}

	thisPtr_->ash_.luaUpdate(msg);

	return 0;
}

