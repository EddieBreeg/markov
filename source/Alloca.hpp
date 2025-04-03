#pragma once

#ifdef _WIN32
extern "C" void* _alloca(size_t);
#define Alloca(T, n)	static_cast<T*>(_alloca((n) * sizeof(T)))
#else
extern "C" void* alloca(size_t);
#define Alloca(T, n)	static_cast<T*>(alloca((n) * sizeof(T)))
#endif