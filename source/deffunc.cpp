#include "deffunc.h"
#include "objectsdef.h"
#include "tostring.h"
#include "keyword.h"

namespace ICM
{
	using namespace Objects;

	namespace DefFunc
	{
		std::string to_string_unfeined_function(const std::string &name, DefaultType type)
		{
			return "Function '" + name + "' is undefined for type(" + ICM::to_string(type) + ")";
		}

		void print(const ObjectPtr &p) {
			switch (p->get_type())
			{
			case T_String:
				Common::Output::print(getPointer<String>(p)->get_data());
				break;
			default:
				Common::Output::print(p->to_string());
				break;
			}
		}
		ObjectPtr print(const DataList &dl) {
			for (auto &op : dl)
				print(op);
			return list(dl);
		}
		ObjectPtr list(const DataList &dl) {
			return ObjectPtr(new List(dl));
		}
		ObjectPtr let(const DataList &dl) {
			AddVariableTable.add(dl[0]->to_string(), new ASTNode(dl[1]));
			return dl[1];
		}
		ObjectPtr dcall(const DataList &dl) {
			string name = getPointer<Identifier>(dl[1])->getName();
			if (DefFuncTable.find(name))
				return checkCall(DefFuncTable[name], DataList({ dl[0],dl[2] }));
			else
				return createError("Error in match function(" + name + ").");
		}


		template <typename Func>
		ObjectPtr default_num_varfunc(const DataList &list, Func func, const std::string &name);

		template <typename T>
		ObjectPtr add(const DataList &list) {
			T *tmp = getPointer<T>(list.front())->clone();
			for (auto i : Range<size_t>(1, list.size() - 1))
				tmp->add(getPointer<T>(list[i]));
			return ObjectPtr(tmp);
		}

		ObjectPtr add(const DataList &list) {
			ObjectPtr result;

			if (list.empty())
				return ObjectPtr(new Number(0));

			ObjectPtr front = list.front();
			switch (front->get_type()) {
			case T_Number:
				result = add<Number>(list);
				break;
			case T_String:
				result = add<String>(list);
				break;
			case T_List:
				result = add<List>(list);
				break;
			default:
				result = createError(to_string_unfeined_function("+", list.front()->get_type()));
			}
			return result;
		}

		template <typename T>
		ObjectPtr equ(const DataList &list) {
			return ObjectPtr(list[0]->equ(getPointer<T>(list[1])));
		}

		ObjectPtr equ(const DataList &list) {
			ObjectPtr result;

			result = ObjectPtr(list[0]->equ(list[1]));

			return result;
		}

		ObjectPtr sub(const DataList &list) {
			return default_num_varfunc(list, [](Number *num, Number *obj) { return num->sub(obj); }, "-");
		}
		ObjectPtr mul(const DataList &list) {
			return default_num_varfunc(list, [](Number *num, Number *obj) { return num->mul(obj); }, "*");
		}
		ObjectPtr div(const DataList &list) {
			return default_num_varfunc(list, [](Number *num, Number *obj) { return num->div(obj); }, "/");
		}
		ObjectPtr mod(const DataList &list) {
			return default_num_varfunc(list, [](Number *num, Number *obj) { return num->mod(obj); }, "%");
		}

		template <typename Func>
		ObjectPtr default_num_varfunc(const DataList &list, Func func, const std::string &name) {
			ObjectPtr result;
			if (list.empty())
				return ObjectPtr(new Number(0));
			if (list.front()->get_type() == T_Number) {
				Number *tmp = getPointer<Number>(list.front())->clone();
				for (auto i : Range<size_t>(1, list.size() - 1))
					func(tmp, getPointer<Number>(list[i]));
				result = ObjectPtr(tmp);
			}
			else {
				result = createError(to_string_unfeined_function(name, list.front()->get_type()));
			}
			return result;
		}
	}
}
