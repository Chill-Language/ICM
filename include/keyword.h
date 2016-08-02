#ifndef _ICM_KEYWORD_H_
#define _ICM_KEYWORD_H_

#include "objects.h"
#include "function.h"

namespace ICM
{
	class FuncTableBase
	{
	public:
		FuncTableBase(size_t id, const string &name, const FuncPtr &func, const FuncParameter &pars)
			: id(id), name(name), func(func), pars(pars) {}

		size_t getID() const { return id; }
		const string& getName() const { return name; }
		const FuncPtr& getFuncPtr() const { return func; }
		const FuncParameter& getParameter() const { return pars; }

	private:
		size_t id;
		string name;
		FuncPtr func;
		FuncParameter pars;
	};

	class FuncTableType
	{
	public:
		FuncTableType();

		void add(const string &name, const FuncPtr &func, const FuncParameter &pars);
		const FuncTableBase& operator[](size_t id) const;
		size_t find(const string &name) const;

	private:
		size_t count = 0;
		vector<FuncTableBase> data;
		map<string, size_t> keymap;
	};

	void createDefaultFuncTable();
	ObjectPtr checkCall(const ICM::FuncTableBase &ftb, const ICM::DataList &dl);
}

extern ICM::FuncTableType FuncTable;

#endif
