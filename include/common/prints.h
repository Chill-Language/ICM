// Name:	     prints.h
// Date:	     06/16/2016
// Version:      2.1.0.0
// Description:  Basic on 'to_string', using 'printf'.

#ifndef _SYSTEM_PRINTS_H_
#define _SYSTEM_PRINTS_H_
#include "macro.h"
#include <string>

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
	inline std::string to_string(std::nullptr_t null)
	{
		return std::string("Null");
	}
	inline std::string to_string(bool b)
	{
		return std::string((b == true) ? "T" : "F");
	}
	inline std::string to_string(char c)
	{
		std::string str;
		str.push_back(c);
		return str;
	}
	inline std::string to_string(const void *p)
	{
		size_t size = sizeof(void*) * 2 + 3;
		char *str = new char[size];
		snprintf(str, size, "0x%p", p);
		return str;
	}
	template <typename Iter, typename Func>
	std::string to_string(Iter begin, Iter end, Func func) {
		std::string str;
		str.append("[");
		if (begin != end) {
			str.append(func(*begin++));
			while (begin != end) {
				str.append(", ");
				str.append(func(*begin++));
			}
		}
		str.append("]");
		return str;
	}
}

namespace Output
{
	// Print
	template <typename T>
	inline void print(const T &obj)
	{
		using Convert::to_string;
		using std::to_string;
		printf("%s", to_string(obj).c_str());
	}
	inline void print(const char *str)
	{
		printf("%s", str);
	}
	template <typename First, typename... Rest>
	void print(const First& first, const Rest&... rest)
	{
		print(first);
		print(rest...);
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
	template <typename First, typename... Rest>
	void println(const First& first, const Rest&... rest)
	{
		print(first);
		print(rest...);
		println();
	}
	
	// PutError
	inline void putError(const char *msg)
	{
		println(msg);
		exit(-1);
	}
}
END

#endif
