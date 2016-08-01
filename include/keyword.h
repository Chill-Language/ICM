#ifndef _ICM_KEYWORD_H_
#define _ICM_KEYWORD_H_

#include "objects.h"

namespace ICM
{
	class FuncTableBase
	{
	public:
		FuncTableBase(size_t id, const string &name, const FuncPtr &func)
			: id(id), name(name), func(func) {}

		size_t getID() const { return id; }
		const string& getName() const { return name; }
		const FuncPtr& getFuncPtr() const { return func; }

	private:
		size_t id;
		string name;
		FuncPtr func;
	};

	class FuncTableType
	{
	public:
		FuncTableType();

		void add(const string &name, const FuncPtr &func);
		const FuncTableBase& operator[](size_t id) const;
		size_t find(const string &name) const;

	private:
		size_t count = 0;
		vector<FuncTableBase> data;
		map<string, size_t> keymap;
	};

	void createDefaultFuncTable();
}

extern ICM::FuncTableType FuncTable;

#endif
