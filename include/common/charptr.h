// Name:	charptr.h
// Date:    06/15/2016
// Version: 2.0.0.0

#ifndef _SYSTEM_CHARPTR_H_
#define _SYSTEM_CHARPTR_H_
#include "macro.h"
#include "memory.h"
#include <string>
#include <memory>

SYSTEM BEGIN
class charptr
{
public:
	explicit charptr(size_t len) : length(len), data(new char[len + 1]()) {}
	explicit charptr(const char *ptr, size_t len) : length(len), data(Memory::copyOf(ptr, length + 1)) {}
	explicit charptr(const std::string &str) : charptr(str.c_str(), str.length()) {}
	operator char*() { return data.get(); }
	std::string to_string() const { return data.get(); }
	size_t getLength() const { return length; }

private:
	size_t length;
	std::shared_ptr<char> data;
};
END

#endif
