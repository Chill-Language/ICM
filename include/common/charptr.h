// Name:	charptr.h
// Date:    02/27/2016
// Version: 1.0.0.1

#ifndef _SYSTEM_CHARPTR_H_
#define _SYSTEM_CHARPTR_H_
#include "macro.h"
#include <string>

SYSTEM BEGIN
class charptr
{
public:
	explicit charptr(size_t length) : data(new char[length + 1]()) {}
	~charptr() { delete[] data; }
	operator char*() { return data; }
	std::string to_string() const { return data; }

private:
	char* data;
};
END

#endif
