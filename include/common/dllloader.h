// Name:    dllloader.h
// Date:    09/04/2017
// Version: 1.0.0.0

#pragma once
#ifndef _SYSTEM_DLLLOADER_H_
#define _SYSTEM_DLLLOADER_H_
#include "macro.h"
#include <memory>

#define S_WINDOWS 1
#define S_LINUX   2

#if (defined _WIN32)
#	define SYSTEM_PLATFORM S_WINDOWS
#elif (defined __linux__)
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
	using DLLPointer = HINSTANCE;
	using ErrorCode = DWORD;
#	ifdef UNICODE
	using Path = LPCWSTR;
#	else
	using Path = LPCSTR;
#	endif
#elif SYSTEM_PLATFORM == S_LINUX
	using DLLPointer = void*;
	using ErrorCode = const char *;
	using Path = const char*;
#endif

	struct DLLData
	{
		DLLPointer data;
	};

	DLLLoader() : _errcode((ErrorCode)(0)) {}

	explicit DLLLoader(Path name, int flag = 0)
		: data(dllcreate(name, flag, _errcode), DLLLoader::dllclose) {}

	void open(Path name, int flag = 0) {
		DLLData *ptr = dllcreate(name, flag, _errcode);
		this->data = std::shared_ptr<DLLData>(ptr, DLLLoader::dllclose);
	}
	void close() {
		this->data = nullptr;
	}

	template <typename T = void>
	T* get(const char *func_name) const {
		return reinterpret_cast<T*>(DLLLoader::dllgetfunc(*data, func_name));
	}

	bool bad() const {
		return data == nullptr;
	}
	ErrorCode errcode() const {
		return _errcode;
	}

private:

	static DLLData* dllcreate(Path name, int flag, ErrorCode &errcode) {
		bool error = false;

#if SYSTEM_PLATFORM == S_WINDOWS
		auto ptr = LoadLibrary(name);
		if (ptr == nullptr) {
			error = true;
			errcode = GetLastError();
		}
#elif SYSTEM_PLATFORM == S_LINUX
		auto ptr = dlopen(name, flag);
		if (ptr == nullptr || (errcode = dlerror()) != nullptr) {
			error = true;
			errcode = dlerror();
		}
#endif

		return error ? nullptr : new DLLData{ ptr };
	}

	static void dllclose(DLLData *data) {
		if (data == nullptr) return;

#if SYSTEM_PLATFORM == S_WINDOWS
		FreeLibrary(data->data);
#elif SYSTEM_PLATFORM == S_LINUX
		dlclose(data->data);
#endif
	}

	static void* dllgetfunc(DLLData data, const char *func_name) {
#if SYSTEM_PLATFORM == S_WINDOWS
		return (void*)GetProcAddress(data.data, func_name);
#elif SYSTEM_PLATFORM == S_LINUX
		return dlsym(data.data, func_name);
#endif
	}

private:
	std::shared_ptr<DLLData> data;
	ErrorCode _errcode;
};
SYSTEM_END

#endif
