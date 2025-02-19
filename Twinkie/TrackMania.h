#pragma once

namespace TM
{
	template <typename T>
	struct CFastBuffer
	{
		int Size = 0;
		T* Ptr = nullptr;
		int Capacity = 0;
	};

	template <typename T>
	struct CFastArray
	{
		int Size = 0;
		T* Ptr = nullptr;
	};

	struct GmVec3
	{
		float x;
		float y;
		float z;
	};

	struct GmIso4
	{
		GmVec3 x;
		GmVec3 y;
		GmVec3 z;
		GmVec3 t;
	};

	enum GameType
	{
		United,
		Nations,
		Modded,
		Unk
	};

	enum RaceState
	{
		Playing = 0,
		Finished = 1,
		Paused = 0xf
	};
}