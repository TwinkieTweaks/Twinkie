#pragma once

namespace TM
{
	template <typename T>
	struct CFastBuffer
	{
		size_t Size = 0;
		T* Ptr = nullptr;
		size_t Capacity = 0;

		T* operator[](size_t Idx)
		{
			return Ptr + Idx; // FUCK YOU BILL GATES
		}
	};

	template <typename T>
	struct CFastArray
	{
		size_t Size = 0;
		T* Ptr = nullptr;

		T operator[](size_t Idx)
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

		GmVec3 operator*(GmVec3 Vec)
		{
			return {x * Vec.x, y * Vec.y, x * Vec.z};
		}

		GmVec3 operator*(float F)
		{
			return { x * F, y * F, x * F };
		}

		GmVec3 operator-(GmVec3 Vec)
		{
			return { x - Vec.x, y - Vec.y, x - Vec.z };
		}

		GmVec3 operator+(GmVec3 Vec)
		{
			return { x + Vec.x, y + Vec.y, x + Vec.z };
		}
	};

	struct GmNat3
	{
		int x;
		int y;
		int z;
	};

	struct GmVec4
	{
		float x;
		float y;
		float z;
		float w;

		float& operator[](int Idx)
		{
			switch (Idx)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			default: return x;
			}
		}
	};

	// might also be called GmFrustum
	struct GmMat4
	{
		GmVec4 x;
		GmVec4 y;
		GmVec4 z;
		GmVec4 t;

		GmMat4()
		{
			x = { 1.f, 0.f, 0.f, 0.f };
			y = { 0.f, 1.f, 0.f, 0.f };
			z = { 0.f, 0.f, 1.f, 0.f };
			t = { 0.f, 0.f, 0.f, 1.f };
		}

		GmMat4(GmVec4 x, GmVec4 y, GmVec4 z, GmVec4 t) : x(x), y(y), z(z), t(t) {}

		GmVec4& operator[](int Idx)
		{
			switch (Idx)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return t;
			default: return x;
			}
		}

		GmVec4 operator*(GmVec4 Vec)
		{
			GmVec4 Result = {};
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					Result[i] += this->operator[](i)[j] * Vec[j];
				}
			}
			return Result;
		}

		GmMat4 operator*(GmMat4 Mat)
		{
			GmMat4 Result = {};
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					for (int k = 0; k < 4; ++k)
					{
						Result[i][j] += this->operator[](i)[k] * Mat[k][j];
					}
				}
			}
			return Result;
		}
	};

	struct GmIso4
	{
		GmVec3 x;
		GmVec3 y;
		GmVec3 z;
		GmVec3 t;

		operator GmMat4()
		{
			GmMat4 mat;
			mat.x = { x.x, y.x, z.x, t.x };
			mat.y = { x.y, y.y, z.y, t.y };
			mat.z = { x.z, y.z, z.z, t.z };
			mat.t = { 0.f, 0.f, 0.f, 1.f };
			return mat;
		}

		GmVec3 operator[](int Idx)
		{
			GmVec3 Arr[] = { x, y, z, t };
			return Arr[Idx];
		}
	};

	struct GmVec2
	{
		float x;
		float y;
	};

	struct CMwId
	{
		unsigned int Value;
	};

	enum RaceState
	{
		BeforeStart = 0,
		Running = 1,
		Finished = 2
	};

	enum AccountType
	{
		Disconnected = 0,
		Nations = 1,
		United = 2
	};

	enum MaterialId
	{
		Concrete = 0,
		Pavement = 1,
		Grass = 2,
		Ice = 3,
		Metal = 4,
		Sand = 5,
		Dirt = 6,
		Turbo = 7,
		DirtRoad = 8,
		Rubber = 9,
		SlidingRubber = 10,
		Test = 11,
		Rock = 12,
		Water = 13,
		Wood = 14,
		Danger = 15,
		Asphalt = 16,
		WetDirtRoad = 17,
		WetAsphalt = 18,
		WetPavement = 19,
		WetGrass = 20,
		Snow = 21,
		ResonantMetal = 22,
		GolfBall = 23,
		GolfWall = 24,
		GolfGround = 25,
		Turbo2 = 26,
		Bumper = 27,
		NotCollidable = 28,
		FreeWheeling = 29,
		TurboRoulette = 30
	};
}