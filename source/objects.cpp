#include "objects.h"
#include "tostring.h" // TODO

namespace ICM
{
	namespace Objects
	{
		std::string to_string_unfeined_function(const std::string &name, DefaultType type)
		{
			return "Function '"+ name + "' is undefined for type(" + ICM::to_string(type) + ")";
		}

		Object* Object::add(const Object *obj)
		{
			std::string msg(to_string_unfeined_function("+", get_type()));
			println(msg);
			return new Error(msg);
		}

		ObjectPtr createObject(DefaultType type)
		{
			using namespace Objects;

			Object *object;
			switch (type)
			{
			case ICM::T_Number:
				object = new Number();
				break;
			case ICM::T_String:
				object = new String();
				break;
			default:
				object = new Object();
				break;
			}
			return ObjectPtr(object);
		}

		namespace Func
		{

			template <typename T>
			T* getPointer(const ObjectPtr &op) {
				return (T*)(op.get());
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

			template <typename Func>
			ObjectPtr default_num_varfunc(const DataList &list, Func func, const std::string &name);

			ObjectPtr add(const DataList &list) {
				Object *front = list.front().get();
				Object *result = front->clone();
				for (auto i : Range<size_t>(1, list.size() - 1))
					result->add(list[i].get());
				return ObjectPtr(result);
			}

			ObjectPtr sub(const DataList &list) {
				return default_num_varfunc(list, [](Number *num, Object *obj) { return num->sub(obj); }, "-");
			}
			ObjectPtr mul(const DataList &list) {
				return default_num_varfunc(list, [](Number *num, Object *obj) { return num->mul(obj); }, "*");
			}
			ObjectPtr div(const DataList &list) {
				return default_num_varfunc(list, [](Number *num, Object *obj) { return num->div(obj); }, "/");
			}

			template <typename Func>
			ObjectPtr default_num_varfunc(const DataList &list, Func func, const std::string &name) {
				ObjectPtr result;
				if (list.front()->get_type() == T_Number) {
					Number *tmp = (Number*)list.front().get()->clone();
					for (auto i : Range<size_t>(1, list.size() - 1))
						func(tmp, list[i].get());
					result = ObjectPtr(tmp);
				}
				else {
					result = ObjectPtr(new Error(to_string_unfeined_function(name, list.front()->get_type())));
				}
				return result;
			}
		}
		std::string to_string(const Object &obj) {
			return obj.to_string();
		}
	}
}
