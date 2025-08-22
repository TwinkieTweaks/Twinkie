#pragma once
#include "LuaEngine/include/lua.hpp"

#define TwinkieVersion "v2.0.0-luaengine-rcfour"

class Versioning
{
public:
	const char* const TwinkieVer = TwinkieVersion;
	const char* const LuaVer = LUA_VERSION;

	Versioning() {}
};