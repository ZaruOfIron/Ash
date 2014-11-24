#include "lua_controle_script.hpp"
#include <cassert>

LuaControleScript::LuaControleScript(Ash& ash, const std::string& filename)
	: lua_(luaL_newstate(), lua_close), filenaem_(filename), ash_(ash)
{
	// null�łȂ���΁A���ɃC���X�^���X�����݂���
	assert(thisPtr_ == nullptr);
	thisPtr_ = this;

	// Lua Script��������
	auto L = lua_.get();
	luaL_openlibs(L);
}

void LuaControleScript::initialize()
{
	auto L = lua_.get();

	window_.reset(new ControleWindow(this));

	// api��ݒ�
	static const luaL_Reg ash[] = {
		{ "user", &LuaControleScript::luaUser },
		{ NULL, NULL }
	};
	luaL_newlib(L, ash);
	lua_setglobal(L, "ash");

	// initialize()���Ăяo��
	// �֐���ς�
	lua_getglobal(L, "initialize");
	// �Ăяo��
	// lua_pcall(L, ����, �߂�l, ?)
	if(lua_pcall(L, 0, 0, 0))	throw std::runtime_error(luaL_checkstring(L, -1));
	// �߂�l���擾
}

void LuaControleScript::onCommand(int index, int id)
{
	auto L = lua_.get();

	// on_command()���Ăяo��
	// �֐���ς�
	lua_getglobal(L, "on_command");
	// ������ς�
	lua_pushnumber(index);
	lua_pushnumber(id);
	// �Ăяo��
	// lua_pcall(L, ����, �߂�l, ?)
	if(lua_pcall(L, 2, 0, 0))	throw std::runtime_error(luaL_checkstring(L, -1));
}

