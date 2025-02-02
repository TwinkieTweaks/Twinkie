#pragma once

#include "pch.h"

class ProcHandler
{
public:
	ProcHandler(){}

	template <typename T>
	T Read(uintptr_t Addr)
	{
		return *reinterpret_cast<T*>(Addr);
	}

	template <typename T>
	void Write(T Value, uintptr_t Addr)
	{
		*reinterpret_cast<T*>(Addr) = Value;
	}
};