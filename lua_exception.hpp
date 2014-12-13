#ifndef ___LUA_EXCEPTION_HPP___
#define ___LUA_EXCEPTION_HPP___

#include <stdexcept>

class LuaException : public std::runtime_error
{
public:
	LuaException(const std::string& msg)
		: std::runtime_error("lua: " + msg)
	{}
};

class LuaCantLoadFileError : public LuaException
{
public:
	LuaCantLoadFileError(const std::string& msg)
		: LuaException(msg)
	{}
};

class LuaCantCallFuncError : public LuaException
{
public:
	LuaCantCallFuncError(const std::string& msg)
		: LuaException(msg)
	{}
};

#endif

