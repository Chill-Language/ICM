// Name:	file.h
// Date:    02/14/2016
// Version: 1.1.2.0

#ifndef _SYSTEM_FILE_H_
#define _SYSTEM_FILE_H_
#include "macro.h"
#include "charptr.h"
#include <cstdio>
#include <cstring>
#include <string>

SYSTEM BEGIN
class File
{
public:
	File(const char *mode) : mode(mode) { pointer = NULL; }
	File(const char *path, const char *mode) : path(path), mode(mode) { priopen(); }
	~File() { priclose(); }
	File& open(const char *path) {
		priclose();
		if (path)
			this->path = path;
		priopen();
		return *this;
	}
	File& open(const std::string &str) {
		open(str.c_str());
		return *this;
	}
	File& close() {
		priclose();
		return *this;
	}
	const char *name() const { return path; }
	operator FILE*() const { return pointer; }
	bool good() const {
		return pointer != nullptr;
	}
	bool bad() const {
		return pointer == nullptr;
	}
	bool end() const {
		return feof(pointer) != 0;
	}
	template <typename T>
	void save(const T &sv) const {
		if (bad()) return;
		fwrite(&sv, sizeof(T), 1, pointer);
	}
	template <typename T>
	void save(const T *sv, size_t n) const {
		if (bad()) return;
		fwrite(sv, sizeof(T), n, pointer);
	}

	template <typename T>
	void load(T &sv) const {
		if (bad()) return;
		fread(&sv, sizeof(T), 1, pointer);
	}
	template <typename T>
	void load(T *sv, size_t n) const {
		if (bad()) return;
		fread(sv, sizeof(T), n, pointer);
	}

	template <typename T>
	void appe(const T &sv) const {
		if (bad()) return;
		fwrite(&sv, sizeof(T), 1, pointer);
	}
	template <typename T>
	void appe(const T *sv, size_t n) const {
		if (bad()) return;
		fwrite(sv, sizeof(T), n, pointer);
	}
	void seek(long offn) {
		if (bad()) return;
		fseek(pointer, offn, SEEK_SET);
	}
	void setbegin() {
		if (bad()) return;
		fseek(pointer, 0, SEEK_SET);
	}
	long size() const {
		fpos_t pos;
		fgetpos(pointer, &pos);
		fseek(pointer, 0, SEEK_END);
		int size = ftell(pointer);
		fsetpos(pointer, &pos);
		return size;
	}
	std::string get_text() const {
		if (bad()) return std::string();
		long fs = size();
		fseek(pointer, 0, SEEK_SET);
		charptr tmp(fs);
		fread(tmp, sizeof(char), fs, pointer);
		fseek(pointer, 0, SEEK_SET);
		return tmp.to_string();
	}
	char* get_textdata() const {
		if (bad()) return nullptr;
		long fs = size();
		fseek(pointer, 0, SEEK_SET);
		char *str = new char[fs + 1]();
		fread(str, sizeof(char), fs, pointer);
		fseek(pointer, 0, SEEK_SET);
		return str;
	}
	std::string getline() const {
		charptr ptr(0xff);
		fgets(ptr, 0xff, pointer);
		size_t p = strlen(ptr)-1;
		ptr[p] = (ptr[p] == '\n') ? '\0' : ptr[p];
		return ptr.to_string();
	}
	void putline(const std::string &str) {
		if (bad()) return;
		fputs((str+"\n").c_str(), pointer);
	}

private:
	FILE *pointer = nullptr;
	const char *path, *mode;
	bool priopen() {
		pointer = fopen(path, mode);
		if (!pointer) {
			FILE *tmp = fopen(path, ((mode[1] == 't') ? "wt+" : "wb+"));
			if (tmp)
				fclose(tmp);
			else
				printf("Error in open %s", path);
			return false;
		}
		return true;
	}
	void priclose() {
		if (pointer)
			fclose(pointer);
		pointer = nullptr;
	}
public:
	typedef const char * const Const;
	static Const LOAD_TEXT;
	static Const SAVE_TEXT;
	static Const ADDN_TEXT;
	static Const LOAD_BINA;
	static Const SAVE_BINA;
	static Const APPE_BINA;
};

File::Const File::LOAD_TEXT = "rt";
File::Const File::SAVE_TEXT = "wt";
File::Const File::ADDN_TEXT = "at";
File::Const File::LOAD_BINA = "rb";
File::Const File::SAVE_BINA = "wb";
File::Const File::APPE_BINA = "ab";

std::string get_path(const char *filename)
{
	//charptr buffer(0xff);
	//getcwd(buffer,0xff);
    return filename;
}

namespace SaveLoad
{
	template <typename T>
	void save(const T &sv, const char *path) {
		File(path, File::SAVE_BINA).save(sv);
	}
	template <typename T>
	void load(T &sv, const char *path) {
		File(path, File::LOAD_BINA).load(sv);
	}
	void clear(const char *path) {
		File(path, File::SAVE_BINA);
	}

	void set(const std::string &inc, const char *path) {
		fputs(inc.c_str(), File(path, File::SAVE_TEXT));
	}
	std::string get(const char *path) {
		return File(path, File::LOAD_TEXT).get_text();
	}
}
END

#endif