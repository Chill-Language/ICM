// Name:	     prints.h
// Date:	     06/14/2016
// Version:      2.0.0.0
// Description:  Basic on 'to_string', using 'printf'.

#ifndef _SYSTEM_PRINTS_H_
#define _SYSTEM_PRINTS_H_
#include "macro.h"
#include <string>
#include <initializer_list>

SYSTEM BEGIN
namespace Convert
{
	// To String
	//   String & const char*
	inline const std::string& to_string(const std::string &str)
	{
		return str;
	}
	inline std::string to_string(const char *s)
	{
		return std::string(s);
	}
	//   Value to Template
	template <typename T>
	inline std::string to_string(T value)
	{
		return std::to_string(value);
	}
	template <>
	inline std::string to_string(std::nullptr_t null)
	{
		return std::string("Null");
	}
	template <>
	inline std::string to_string(bool b)
	{
		return std::string((b == true) ? "T" : "F");
	}
	template <>
	inline std::string to_string(char c)
	{
		std::string str;
		str.push_back(c);
		return str;
	}
	template <>
	inline std::string to_string(const void *p)
	{
		size_t size = sizeof(void*) * 2 + 3;
		char *str = new char[size];
		snprintf(str, size, "0x%p", p);
		return str;
	}
}

namespace Output
{
	// Print
	template <typename T>
	inline void print(const T &obj)
	{
		printf("%s", Convert::to_string(obj).c_str());
	}
	inline void print(const char *str)
	{
		printf("%s", str);
	}
	
	// Pintln
	inline void println()
	{
		printf("\n");
	}
	template <typename T>
	inline void println(const T &obj)
	{
		print<T>(obj);
		println();
	}
	
	// PutError
	inline void putError(const char *msg)
	{
		println(msg);
		exit(-1);
	}
}
template <typename T>
inline void print(const T &obj)
{
	Output::print(to_string(obj));
}
template <>
inline void print(const std::string &obj)
{
	Output::print(obj);
}
inline void print(const char *obj)
{
	Output::print(obj);
}
inline void println()
{
	Output::println();
}
template <typename T>
inline void println(const T &obj)
{
	Common::print(obj);
	println();
}

END

#endif
