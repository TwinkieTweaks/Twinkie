#pragma once

#define TwinkieVersion "v2.0.0-luaengine-rcone"
#define LuaVersion "5.4.2"

class Versioning
{
public:
	const char* const TwinkieVer = TwinkieVersion;
	const char* const LuaVer = LuaVersion;

	Versioning() {}
};