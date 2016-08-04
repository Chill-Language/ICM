#include "deffunc.h"
#include "objectsdef.h"
#include "tostring.h"
#include "keyword.h"
#include "tostring.h"

namespace ICM
{
	using namespace Objects;

	namespace DefFunc
	{
		std::string to_string_unfeined_function(const std::string &name, DefaultType type)
		{
			return "Function '" + name + "' is undefined for type(" + ICM::to_string(type) + ")";
		}

		ObjectPtr print(const DataList &dl) {
			for (auto &op : dl)
				Common::Output::print(op->to_output());
			return list(dl);
		}
		ObjectPtr println(const DataList &dl) {
			for (auto &op : dl)
				Common::Output::println(op->to_output());
			return list(dl);
		}
		ObjectPtr list(const DataList &dl) {
			return ObjectPtr(new List(dl));
		}
		ObjectPtr let(const DataList &dl) {
			string name = getPointer<Identifier>(dl[0])->getName();
			size_t id = AddVariableTable.find(name);
			Identifier *data;
			data = (id) ? AddVariableTable[id].getData() : getPointer<Identifier>(dl[0]);
			data->setValue(new ASTNode(dl[1])); // TODO
			if (!id)
				AddVariableTable.add(name, data);
			return ObjectPtr(data);
		}
		ObjectPtr dcall(const DataList &dl) {
			string name = getPointer<Identifier>(dl[1])->getName();
			if (DefFuncTable.find(name))
				return checkCall(DefFuncTable[name], DataList({ dl[0],dl[2] }));
			else
				return createError("Error in match function(" + name + ").");
		}
		ObjectPtr system(const DataList &dl) {
			int i = std::system(getPointer<Objects::String>(dl[0])->get_data().c_str());
			return ObjectPtr(new Number(i));
		}
		ObjectPtr exit(const DataList &dl) {
			std::exit(0);
			return nullptr;
		}
		ObjectPtr n_small(const DataList &dl) {
			bool result = getPointer<Number>(dl[0])->operator<(*getPointer<Number>(dl[1]));
			return ObjectPtr(new Boolean(result));
		}
		ObjectPtr n_smallequal(const DataList &dl) {
			bool result = getPointer<Number>(dl[0])->operator<=(*getPointer<Number>(dl[1]));
			return ObjectPtr(new Boolean(result));
		}
		ObjectPtr n_large(const DataList &dl) {
			bool result = getPointer<Number>(dl[0])->operator>(*getPointer<Number>(dl[1]));
			return ObjectPtr(new Boolean(result));
		}
		ObjectPtr n_largeequal(const DataList &dl) {
			bool result = getPointer<Number>(dl[0])->operator>=(*getPointer<Number>(dl[1]));
			return ObjectPtr(new Boolean(result));
		}

		ObjectPtr order(const DataList &dl) {
			List *list = getPointer<List>(dl[0]);
			std::sort(list->begin(), list->end(), [](const ObjectPtr &a, const ObjectPtr &b) {
				return getPointer<Number>(a)->operator<(*getPointer<Number>(b));
			});
			return ObjectPtr(list);
		}

		ObjectPtr inc(const DataList &dl) {
			autoptr<Number> tmp(new Number(1));
			getPointer<Number>(dl[0])->add(tmp.get());
			return dl[0];
		}
		ObjectPtr dec(const DataList &dl) {
			autoptr<Number> tmp(new Number(1));
			getPointer<Number>(dl[0])->sub(tmp.get());
			return dl[0];
		}

		template <typename T>
		ObjectPtr add(const DataList &list) {
			T *tmp = getPointer<T>(list.front())->clone();
			for (auto i : Range<size_t>(1, list.size() - 1))
				tmp->add(getPointer<T>(list[i]));
			return ObjectPtr(tmp);
		}

		ObjectPtr add(const DataList &list) {
			if (list.empty())
				return ObjectPtr(new Number(0));

			ObjectPtr front = list.front();
			switch (front->get_type()) {
			case T_Number: return add<Number>(list);
			case T_String: return add<String>(list);
			case T_List:   return add<List>(list);
			default:
				return createError(to_string_unfeined_function("+", list.front()->get_type()));
			}
		}

		ObjectPtr equ(const DataList &list) {
			ObjectPtr result;

			if (list.size() < 2)
				return ObjectPtr(new Error("Args 1 for 2."));

			result = ObjectPtr(adjustObjectPtr(list[0])->equ(adjustObjectPtr(list[1])));

			return result;
		}

		template <typename Func>
		ObjectPtr default_num_varfunc(const DataList &list, Func func);

		ObjectPtr sub(const DataList &list) {
			return default_num_varfunc(list, [](Number *num, Number *obj) { return num->sub(obj); });
		}
		ObjectPtr mul(const DataList &list) {
			return default_num_varfunc(list, [](Number *num, Number *obj) { return num->mul(obj); });
		}
		ObjectPtr div(const DataList &list) {
			return default_num_varfunc(list, [](Number *num, Number *obj) { return num->div(obj); });
		}
		ObjectPtr mod(const DataList &list) {
			return default_num_varfunc(list, [](Number *num, Number *obj) { return num->mod(obj); });
		}

		template <typename Func>
		ObjectPtr default_num_varfunc(const DataList &list, Func func) {
			if (list.empty())
				return ObjectPtr(new Number(0));
			Number *tmp = getPointer<Number>(list.front())->clone();
			for (auto i : Range<size_t>(1, list.size() - 1))
				func(tmp, getPointer<Number>(list[i]));
			return ObjectPtr(tmp);
		}
	}
}
