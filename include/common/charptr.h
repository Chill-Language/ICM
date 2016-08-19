// Name:	charptr.h
// Date:    06/15/2016
// Version: 2.1.0.0

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
	explicit charptr(size_t len) : _capacity(len + 1), data(Memory::create<char>(_capacity), Memory::free<char>) {}
	explicit charptr(const char *ptr, size_t len) : _capacity(len + 1), data(Memory::copyOf(ptr, _capacity), Memory::free<char>) {}
	explicit charptr(const std::string &str) : charptr(str.c_str(), str.length()) {}
	operator char*() { return data.get(); }
	std::string to_string() const { return data.get(); }
	charptr* clone() const { return new charptr(data.get(), _capacity); }
	size_t capacity() const { return _capacity; }
	size_t length() const { size_t len = strlen(data.get()); return len < _capacity ? len : _capacity; }

private:
	size_t _capacity;
	std::shared_ptr<char> data;
};
END

#endif
