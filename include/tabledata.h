#pragma once
#include "basic.h"
#include "function.h"

namespace ICM
{
	//=======================================
	// * Class FuncTableUnit
	//=======================================
	class FuncTableUnit
	{
	public:
		using FuncObject = ICM::Function::FuncObject;
		using FuncInitObject = ICM::Function::FuncInitObject;

		FuncTableUnit() = default;
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<FuncObject> &func)
			: id(id), name(name), func(func) {
			initSignTree();
		}
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<FuncInitObject*> &func)
			: id(id), name(name) {
			for (auto *p : func)
				this->func.push_back(p->get_f());
			initSignTree();
		}

		size_t size() const { return func.size(); }
		const FuncObject& operator[](size_t i) const { return func[i]; }

		const FuncObject* checkType(const lightlist<TypeObject> &typelist) const;

		void setID(size_t id) { this->id = id; }
		size_t getID() const { return id; }
		const string& getName() const { return name; }

	private:
		size_t id = 0;
		string name;
		vector<FuncObject> func;
		ICM::Function::SignTree ST;
		void initSignTree() {
			for (auto i : range(0, size())) {
				ST.insert(func[i]);
			}
		}
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
		auto begin() const {
			return keymap.begin();
		}
		auto end() const {
			return keymap.end();
		}

	private:
		size_t count = 0;
		vector<Unit> data;
		map<string, size_t> keymap;
	};

	void createIdentTable();

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

}

#include "identtable.h"
