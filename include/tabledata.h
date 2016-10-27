#ifndef _ICM_TABLEDATA_H_
#define _ICM_TABLEDATA_H_

#include "basic.h"
#include "object.h"
#include "function.h"
#include "keyword.h"

namespace ICM
{
	namespace Objects { class Identifier; }
	//=======================================
	// * Class BaseTableUnit
	//=======================================
	class BaseTableUnit
	{
	public:
		BaseTableUnit() = default;
		BaseTableUnit(size_t id, const string &name)
			: id(id), name(name) {}

		size_t getID() const { return id; }
		const string& getName() const { return name; }

	private:
		size_t id = 0;
		string name;
	};
	//=======================================
	// * Class VariableTableUnit
	//=======================================
#define USE_VARIABLE !true
#if USE_VARIABLE
	namespace TypeBase {
		struct VariableType
		{
			size_t index = MaxValue<size_t>();
			Object data;
			bool isrefer = false;
			bool isstatic = false;
		};
	}
	class VariableTableUnit : public BaseTableUnit
	{
	public:
		VariableTableUnit() = default;
		VariableTableUnit(size_t id, const string &name, const Object *data)
			: BaseTableUnit(id, name) {
			this->data.data = *data;
		}

		void setData(const Object *data) { this->data.data = *data; }
		const Object *getData() const { return &data.data; }
		Object *getData() { return &data.data; }

	private:
		TypeBase::VariableType data;
	};
#else
	class VariableTableUnit : public BaseTableUnit
	{
		using Identifier = Objects::Identifier;
	public:
		VariableTableUnit() = default;
		VariableTableUnit(size_t id, const string &name, const ObjectPtr &data)
			: BaseTableUnit(id, name), data(data) {}

		const Identifier *getData() const { return (Identifier*)data.get(); }
		Identifier *getData() { return (Identifier*)(data.get()); }

	private:
		ObjectPtr data;
	};
#endif

	//=======================================
	// * Class FuncTableUnit
	//=======================================
	class FuncTableUnit : public BaseTableUnit
	{
	public:
		using FuncObject = ICM::Function::FuncObject;
		using FuncInitObject = ICM::Function::FuncInitObject;

		FuncTableUnit() = default;
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<FuncObject> &func)
			: BaseTableUnit(id, name), func(func) {
			initSignTree();
		}
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<FuncInitObject*> &func)
			: BaseTableUnit(id, name) {
			for (auto *p : func)
				this->func.push_back(p->get_f());
			initSignTree();
		}

		size_t size() const { return func.size(); }
		const FuncObject& operator[](size_t i) const { return func[i]; }

		const FuncObject* checkType(const lightlist<TypeObject> &typelist) const;

	private:
		vector<FuncObject> func;
		ICM::Function::SignTree ST;
		void initSignTree() {
			for (auto i : range(0, size())) {
				ST.insert(func[i]);
			}
		}
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
		void add(const string &name, const char *nameref) {
			add(name, string(nameref));
		}
		void add(const string &name, const string &nameref) {
			size_t id = find(nameref);
			if (id) {
				count++;
				data.push_back(Unit());
				keymap[name] = id;
			}
		}
		template <typename... Args>
		void add(const string &name, Args... args) {
			count++;
			data.push_back(Unit(count, name, args...));
			keymap[name] = count;
		}
		void add(const string &name, const Unit &unit) {
			count++;
			data.push_back(unit);
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
		size_t getCurrentID() const {
			return count;
		}

	private:
		size_t count = 0;
		vector<Unit> data;
		map<string, size_t> keymap;
	};

	using VariableTable = Table<VariableTableUnit>;
	using FuncTable = Table<FuncTableUnit>;

	void createDefFuncTable();

	extern FuncTable DefFuncTable;
	extern FuncTable AddFuncTable;
	extern VariableTable DefVariableTable;
	extern VariableTable AddVariableTable;
	extern BijectionKVMap<string, KeywordID> DefKeywordTable;

}
#endif
