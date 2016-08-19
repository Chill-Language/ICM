#ifndef _ICM_FUNCTION_H_
#define _ICM_FUNCTION_H_

#include "ast.h"
#include "objects.h"
#include "lightlist.h"
#include "typeobject.h"

namespace ICM
{
	//=======================================
	// * Namespace Function
	//=======================================
	namespace Function
	{
		//=======================================
		// * Class Signature
		//=======================================
		class Signature
		{
		public:
			using List = lightlist<shared_ptr<TypeObject>>;
			using InitList1 = std::initializer_list<TypeObject>;

			Signature() = default;
			Signature(const InitList1 &intype, const TypeObject& outtype, bool last_is_args = false);

			const List& getInType() const { return InType; }
			const TypeObject& getOutType() const { return *OutType; }
			bool isLastArgs() const { return last_is_args; }

			bool checkType(const DataList &list, DataList &dlp) const;

			string to_string() const;

		private:
			List InType;
			shared_ptr<TypeObject> OutType;
			bool last_is_args;

			bool checkSub(ObjectPtr ptr, DefaultType checktype, DataList &dlp) const;
		};

		//=======================================
		// * Class FuncObject
		//=======================================
		class FuncObject
		{
		public:
			FuncObject() = default;
			FuncObject(const FuncPtr &func, const Signature &sign)
				: func(func), sign(sign) {}

			template <typename... Args>
			bool checkType(Args&... args) const {
				return sign.checkType(args...);
			}
			ObjectPtr call(const DataList &dl) const {
				// If there was a crash, make sure that the pointer 'func' is effective.
				return func(dl);
			}

		private:
			FuncPtr func;
			Signature sign;
		};

		//=======================================
		// * Class FuncInitObject
		//=======================================
		class FuncInitObject
		{
		public:
			FuncObject get_f() {
				return FuncObject(std::bind((&FuncInitObject::func), this, std::placeholders::_1), sign());
			}

		protected:
			virtual Signature sign() const = 0;
			virtual ObjectPtr func(const DataList &list) const = 0;
		};
	}

	//=======================================
	// * Class VariableTableUnit
	//=======================================
	class VariableTableUnit
	{
		using Identifier = Objects::Identifier;
	public:
		VariableTableUnit() = default;
		VariableTableUnit(size_t id, const string &name, Identifier *data)
			: id(id), name(name), data(data) {}

		size_t getID() const { return id; }
		const string& getName() const { return name; }
		const Identifier *getData() const { return data; }
		Identifier *getData() { return data; }

	private:
		size_t id = 0;
		string name;
		Identifier *data = nullptr;
	};

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
			: id(id), name(name), is_ref(false), func(func) {}
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<FuncInitObject*> &func)
			: id(id), name(name), is_ref(true) {
			for (auto *p : func)
				funcp.push_back(shared_ptr<FuncInitObject>(p));
		}

		size_t getID() const { return id; }
		const string& getName() const { return name; }
		const size_t size() const { return is_ref ? funcp.size() : func.size(); }
		FuncObject operator[](size_t i) const { return is_ref ? funcp[i]->get_f() : func[i]; }

	private:
		size_t id = 0;
		string name;
		bool is_ref = false;
		vector<FuncObject> func;
		vector<shared_ptr<FuncInitObject>> funcp;
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
	// * Functions
	//=======================================
	ObjectPtr checkCall(const ICM::FuncTableUnit &ftb, const ICM::DataList &dl);
}

#endif
