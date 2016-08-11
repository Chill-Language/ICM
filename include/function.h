#ifndef _ICM_FUNCTION_H_
#define _ICM_FUNCTION_H_

#include "ast.h"
#include "objects.h"
#include "lightlist.h"

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
			using List = lightlist<DefaultType>;
			using InitList = std::initializer_list<DefaultType>;

			Signature() = default;
			Signature(const InitList &intype, DefaultType outtype, bool last_is_args = false)
				: InType(intype), OutType(outtype), last_is_args(last_is_args) {}
			Signature(const InitList &intype, const InitList &outtype, bool last_is_args = false)
				: InType(intype), OutType(outtype), last_is_args(last_is_args) {}

			const List& getInType() const { return InType; }
			const List& getOutType() const { return OutType; }
			bool isLastArgs() const { return last_is_args; }

			bool checkType(const DataList &list, DataList &dlp) const;

		private:
			List InType;
			List OutType;
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
				return func(dl);
			}

		private:
			FuncPtr func;
			Signature sign;
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
		FuncTableUnit() = default;
		FuncTableUnit(size_t id, const string &name, const std::initializer_list<Function::FuncObject> &func)
			: id(id), name(name), func(func) {}

		size_t getID() const { return id; }
		const string& getName() const { return name; }
		const vector<Function::FuncObject> getFunc() const { return func; }
		const size_t size() const { return func.size(); }
		const Function::FuncObject& operator[](size_t i) const { return func[i]; }

	private:
		size_t id = 0;
		string name;
		vector<Function::FuncObject> func;
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
