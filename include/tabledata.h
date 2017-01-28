#pragma once
#include "basic.h"
#include "object.h"
#include "function.h"
#include "keyword.h"
#include "identifier.h"

namespace ICM
{
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
	// * Class IdentTableUnit
	//=======================================
	class IdentTableUnit : public BaseTableUnit
	{
	public:
		IdentTableUnit() = default;
		IdentTableUnit(size_t id, const string &name)
			: BaseTableUnit(id, name) {}

		virtual IdentType type() const = 0;

		bool isVarb() const { return type() == I_Variable; };
		bool isFunc() const { return type() == I_Function; };
	};


	//=======================================
	// * Class VarbTableUnit
	//=======================================
	namespace TypeBase {
		struct VariableType
		{
			size_t index = MaxValue<size_t>();
			Object data;
			bool isrefer = false;
			bool isstatic = false;
		};
	}

	class VarbTableUnit : public IdentTableUnit
	{
	public:
		VarbTableUnit() = default;

		VarbTableUnit(size_t id, const string &name, const Object &data)
			: IdentTableUnit(id, name) {
			this->data.data = data;
		}

		IdentType type() const { return I_Variable; }

		void setData(const Object *data) { this->data.data = *data; }
		void setData(Object &&data) { this->data.data = data; }
		const Object *getData() const { return &data.data; }
		Object *getData() { return &data.data; }

	private:
		TypeBase::VariableType data;
	};

	//=======================================
	// * Class FuncTableUnit
	//=======================================
	class FuncTableUnit : public IdentTableUnit
	{
	public:
		using FuncObject = ICM::Function::FuncObject;
		using FuncInitObject = ICM::Function::FuncInitObject;

		FuncTableUnit() = default;
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<FuncObject> &func)
			: IdentTableUnit(id, name), func(func) {
			initSignTree();
		}
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<FuncInitObject*> &func)
			: IdentTableUnit(id, name) {
			for (auto *p : func)
				this->func.push_back(p->get_f());
			initSignTree();
		}

		IdentType type() const { return I_Function; }

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
	class VarbTable
	{
		using Unit = VarbTableUnit;
	public:
		VarbTable() {
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
	extern VarbTable GlobalVariableTable;
	extern BijectionKVMap<string, ICM::Keyword::KeywordID> GlobalKeywordTable;

}
