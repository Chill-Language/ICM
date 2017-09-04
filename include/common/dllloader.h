// Name:    dllloader.h
// Date:    09/04/2017
// Version: 1.0.0.0

#pragma once
#ifndef _SYSTEM_DLLLOADER_H_
#define _SYSTEM_DLLLOADER_H_
#include "macro.h"
#include <cassert>
#include <memory>

#define S_WINDOWS 1
#define S_LINUX   2

#ifdef _WIN32
#	define SYSTEM_PLATFORM S_WINDOWS
#else
#	define SYSTEM_PLATFORM S_LINUX
#endif

#if SYSTEM_PLATFORM == S_WINDOWS
#	include <Windows.h>
#elif SYSTEM_PLATFORM == S_LINUX
#	include <dlfcn.h>
#endif

SYSTEM_BEGIN
class DLLLoader
{
public:

#if SYSTEM_PLATFORM == S_WINDOWS
	using DLLData = HINSTANCE;
#elif SYSTEM_PLATFORM == S_LINUX
	using DLLData = void*;
#endif

#if SYSTEM_PLATFORM == S_WINDOWS
#	ifdef UNICODE
	using Path = LPCWSTR;
#	else
	using Path = LPCSTR;
#	endif
#elif SYSTEM_PLATFORM == S_LINUX
	using Path = const char*;
#endif

	explicit DLLLoader(Path name, int flag = 0)
		: data(create(name, flag), close) {}

	template <typename T>
	T* getfunc(const char *func_name) const {
		return reinterpret_cast<T*>(DLLLoader::getfunc(*data, func_name));
	}

	bool bad() const {
		return *data == nullptr;
	}


private:

	static void* getfunc(DLLData data, const char *func_name) {
		assert(data != nullptr);
#if SYSTEM_PLATFORM == S_WINDOWS
		return (void*)GetProcAddress(data, func_name);
#elif SYSTEM_PLATFORM == S_LINUX
		return dlsym(data, func_name);
#endif
	}

	static void close(DLLData *data) {
#if SYSTEM_PLATFORM == S_WINDOWS
		FreeLibrary(*data);
#elif SYSTEM_PLATFORM == S_LINUX
		dlclose(*data);
#endif
	}
	static DLLData* create(Path name, int flag = 0) {
#if SYSTEM_PLATFORM == S_WINDOWS
		auto ptr = LoadLibrary(name);
#elif SYSTEM_PLATFORM == S_LINUX
		auto ptr = dlopen(name, flag);
#endif

		return new DLLData(ptr);
	}

private:
	std::shared_ptr<DLLData> data;
};
SYSTEM_END

#endif
