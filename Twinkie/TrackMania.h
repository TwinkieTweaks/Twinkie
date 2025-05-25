#pragma once

namespace TM
{
	template <typename T>
	struct CFastBuffer
	{
		int Size = 0;
		T* Ptr = nullptr;
		int Capacity = 0;

		T* operator[](int Idx)
		{
			return Ptr + ((Idx * sizeof(T)) / 4); // ??????????????
		}
	};

	template <typename T>
	struct CFastArray
	{
		int Size = 0;
		T* Ptr = nullptr;

		T operator[](int Idx)
		{
			return Ptr[Idx];
		}
	};

	struct CFastString
	{
		int Size;
		char* Cstr;
	};

	struct CFastStringInt
	{
		int Size;
		wchar_t* Cstr;
	};

	struct GmVec3
	{
		float x;
		float y;
		float z;
	};

	struct GmNat3
	{
		int x;
		int y;
		int z;
	};

	struct GmIso4
	{
		GmVec3 x;
		GmVec3 y;
		GmVec3 z;
		GmVec3 t;
	};

	struct CMwId
	{
		unsigned int Value;
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
		BeforeStart = 0,
		Running = 1,
		Finished = 2
	};
}