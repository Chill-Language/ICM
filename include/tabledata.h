#pragma once
#include "basic.h"
#include "object.h"
#include "function.h"
#include "keyword.h"
#include "identifier.h"

#define _USE_IDENTTABLE_ false

namespace ICM
{
	//=======================================
	// * Class IdentTableUnit
	//=======================================
	class IdentTableUnit_OLD
	{
	public:
		IdentTableUnit_OLD() = default;
		IdentTableUnit_OLD(size_t id, const string &name)
			: id(id), name(name) {}

		size_t getID() const { return id; }
		const string& getName() const { return name; }

	private:
		size_t id = 0;
		string name;
	};


	//=======================================
	// * Class DyVarbTableUnit
	//=======================================
	class DyVarbTableUnit : public IdentTableUnit_OLD
	{
	public:
		DyVarbTableUnit() = default;

		DyVarbTableUnit(size_t id, const string &name, const Object &data)
			: IdentTableUnit_OLD(id, name) {
			this->data = data;
		}

		void setData(const Object *data) { this->data = *data; }
		void setData(Object &&data) { this->data = data; }
		const Object *getData() const { return &data; }
		Object *getData() { return &data; }

	private:
		Object data;
	};

	//=======================================
	// * Class FuncTableUnit
	//=======================================
	class FuncTableUnit : public IdentTableUnit_OLD
	{
	public:
		using FuncObject = ICM::Function::FuncObject;
		using FuncInitObject = ICM::Function::FuncInitObject;

		FuncTableUnit() = default;
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<FuncObject> &func)
			: IdentTableUnit_OLD(id, name), func(func) {
			initSignTree();
		}
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<FuncInitObject*> &func)
			: IdentTableUnit_OLD(id, name) {
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
	// * Class VarbTable
	//=======================================
	class DyVarbTable
	{
		using Unit = DyVarbTableUnit;
	public:
		DyVarbTable() {
			data.insert("", Unit());
		}
		template <typename... Args>
		size_t insert(const string &name) {
			return data.insert(name, Unit(data.currentIndex(), name, Objects::Nil()));
		}
		size_t find(const string &name) const {
			auto iter = data.find(name);
			if (iter != data.size())
				return iter;
			return 0;
		}
		const Unit& operator[](size_t id) const {
			return data[id];
		}
		Unit& operator[](size_t id) {
			return data[id];
		}

	private:
		IndexTable<string, Unit> data;
	};

	//=======================================
	// * Class FuncTable
	//=======================================
	class FuncTable
	{
		using Unit = FuncTableUnit;
	public:
		FuncTable() {
			data.push_back(Unit());
		}
		void insert(const string &name, const char *nameref) {
			insert(name, string(nameref));
		}
		void insert(const string &name, const string &nameref) {
			size_t id = find(nameref);
			if (id) {
				count++;
				data.push_back(Unit());
				keymap[name] = id;
			}
		}
		template <typename... Args>
		size_t insert(const string &name, Args... args) {
			count++;
			data.push_back(Unit(count, name, args...));
			keymap[name] = count;
			return count;
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
#if _USE_IDENTTABLE_
	public: // TODO
#endif
		map<string, size_t> keymap;
	};

	void createDefFuncTable();

	//=======================================
	// * Class StringTable
	//=======================================
	class StringTable
	{
	public:
		
	private:
		SerialBijectionMap<string> data;
	};

	extern FuncTable GlobalFunctionTable;
	extern DyVarbTable GlobalDyVarbTable;
	extern BijectionKVMap<string, ICM::Keyword::KeywordID> GlobalKeywordTable;

}

#include "identtable.h"

namespace ICM
{
	extern IdentTable GlobalIdentTable;
}
