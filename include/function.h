#ifndef _ICM_FUNCTION_H_
#define _ICM_FUNCTION_H_

#include "ast.h"
#include "objects.h"

namespace ICM
{
	//=======================================
	// * Enum FuncParaType
	//=======================================
	enum FuncParaType {
		FPT_Void,
		FPT_Fixed,
		FPT_Vary,
		FPT_VaryIdentical,
	};

	//=======================================
	// * Class FuncParameter
	//=======================================
	class FuncParameter
	{
	public:
		using TypeList = std::vector<DefaultType>;

		FuncParameter()
			: type(FPT_Void) {}

		FuncParameter(FuncParaType type, unsigned fixed_size)
			: type(type), fixed_size(fixed_size) {
			initialize();
		}

		FuncParameter(FuncParaType type, unsigned fixed_size, const TypeList &typelist)
			: type(type), fixed_size(fixed_size), typelist(typelist) {
			initialize();
		}

		bool checkType(const DataList &list) const;


	private:
		void initialize();
		bool checkTypeList(const DataList &list, unsigned size) const;
		bool checkTypeList(const DataList &list, unsigned begindex, unsigned endindex) const;


	private:
		FuncParaType type = FPT_Void;
		unsigned fixed_size = 0;
		TypeList typelist;
	};

	//=======================================
	// * Class VariableTableUnit
	//=======================================
	class VariableTableUnit
	{
	public:
		VariableTableUnit() = default;
		VariableTableUnit(size_t id, const string &name, ASTNode *data)
			: id(id), name(name), data(data) {}

		size_t getID() const { return id; }
		const string& getName() const { return name; }
		const ASTNode *getNode() const { return data; }
		ASTNode *getNode() { return data; }

	private:
		size_t id = 0;
		string name;
		ASTNode *data = nullptr;
	};

	//=======================================
	// * Class FuncTableUnit
	//=======================================
	class FuncTableUnit
	{
	public:
		FuncTableUnit() = default;
		FuncTableUnit(size_t id, const string &name, const FuncPtr &func, const FuncParameter &pars)
			: id(id), name(name), func(func), pars(pars) {}

		size_t getID() const { return id; }
		const string& getName() const { return name; }
		const FuncPtr& getFuncPtr() const { return func; }
		const FuncParameter& getParameter() const { return pars; }

	private:
		size_t id = 0;
		string name;
		FuncPtr func;
		FuncParameter pars;
	};

	//=======================================
	// * Class Table<Unit>
	//=======================================
	template <typename Unit>
	class Table
	{
	public:
		Table() {
			data.push_back(Unit());
		}

		template <typename... Args>
		void add(const string &name, const Args&... args) {
			count++;
			data.push_back(Unit(count, name, args...));
			keymap[name] = count;
		}
		const Unit& operator[](size_t id) const {
			return data[id];
		}
		Unit& operator[](size_t id) {
			return data[id];
		}
		Unit& operator[](const string &name) {
			return data[find(name)];
		}
		size_t find(const string &name) const {
			auto iter = keymap.find(name);
			if (iter != keymap.end())
				return iter->second;
			return 0;
		}

	private:
		size_t count = 0;
		vector<Unit> data;
		map<string, size_t> keymap;
	};

	using VariableTable = Table<VariableTableUnit>;
	using FuncTable = Table<FuncTableUnit>;

	//=======================================
	// * Function
	//=======================================
	ObjectPtr checkCall(const ICM::FuncTableUnit &ftb, const ICM::DataList &dl);
}

#endif
