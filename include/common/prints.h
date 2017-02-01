// Name:	     prints.h
// Date:	     06/16/2016
// Version:      2.1.0.0
// Description:  Basic on 'to_string', using 'printf'.

#pragma once
#ifndef _SYSTEM_PRINTS_H_
#define _SYSTEM_PRINTS_H_
#include "macro.h"
#include <string>

SYSTEM BEGIN
namespace Convert
{
	// Format
	template <typename T> struct Format;
	template <> struct Format<int> { static constexpr char text[] = "%d"; };
	template <> struct Format<long> { static constexpr char text[] = "%ld"; };
	template <> struct Format<long long> { static constexpr char text[] = "%lld"; };
	template <> struct Format<unsigned> { static constexpr char text[] = "%u"; };
	template <> struct Format<unsigned long> { static constexpr char text[] = "%lu"; };
	template <> struct Format<unsigned long long> { static constexpr char text[] = "%llu"; };
	template <> struct Format<float> { static constexpr char text[] = "%f"; };
	template <> struct Format<double> { static constexpr char text[] = "%f"; };
	template <> struct Format<long double> { static constexpr char text[] = "%Lf"; };
	template <> struct Format<char> { static constexpr char text[] = "%c"; };

	template <typename T> inline constexpr char* format() { return Format<T>::text; }

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
	template <char B = '\0', char E = '\0', char D = '\0', typename Iter, typename Func>
	std::string to_string(Iter begin, Iter end, Func func) {
		std::string str;
		if (B != '\0')
			str.push_back(B);
		if (begin != end) {
			str.append(func(*begin++));
			while (begin != end) {
				if (D != '\0')
					str.push_back(D);
				if (D != '\n')
					str.append(" ");
				str.append(func(*begin++));
			}
		}
		if (E != '\0')
			str.push_back(E);
		return str;
	}
	template <char B = '\0', char E = '\0', char D = '\0', typename Iter>
	inline std::string to_string(Iter begin, Iter end) {
		using std::to_string;
		using Convert::to_string;

		return to_string<B, E, D>(begin, end, [](const auto &e) { return to_string(e); });
	}
}

namespace Convert
{
	// To Hex
	template <typename T>
	std::string to_hex(T value, const char *fmt)
	{
		const size_t len = sizeof(T) * 2 + 1;
		char data[len];
#if defined(_MSC_VER)
		sprintf_s(data, len, fmt, value);
#else
		sprintf(data, fmt, value);
#endif
		return to_string(data);
	}

	inline std::string to_hex(int8_t i) { return to_hex(i, "%02x"); }
	inline std::string to_hex(uint8_t i) { return to_hex(i, "%02x"); }
	inline std::string to_hex(int16_t i) { return to_hex(i, "%04x"); }
	inline std::string to_hex(uint16_t i) { return to_hex(i, "%04x"); }
	inline std::string to_hex(int32_t i) { return to_hex(i, "%08x"); }
	inline std::string to_hex(uint32_t i) { return to_hex(i, "%08x"); }
	inline std::string to_hex(int64_t i) { return to_hex(i, "%016llx"); }
	inline std::string to_hex(uint64_t i) { return to_hex(i, "%016llx"); }
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
