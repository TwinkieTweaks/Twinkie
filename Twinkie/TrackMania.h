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
}