#include "objects.h"

namespace ICM
{
	namespace Objects
	{
		namespace Func
		{
			template <typename T>
			T& getData(const ObjectPtr &op) {
				return *(T*)(op.get());
			}
			void print(const ObjectPtr &p) {
				switch (p->get_type())
				{
				case T_Number:
					Common::Output::print(p->to_string());
					break;
				case T_String:
					Common::Output::print(getData<String>(p).get_data());
					break;
				default:
					Common::Output::print(p->to_string());
					break;
				}
			}
			template <>
			ObjectPtr sub<Number>(const DataList &list) {
				Number *result = new Number(getData<Number>(list.front()));
				result->add(*(Number*)(list.front().get()));
				for (auto &l : list)
					result->sub(getData<Number>(l));
				return ObjectPtr(result);
			}
			template <>
			ObjectPtr mul<Number>(const DataList &list) {
				Number *result = new Number(1);
				for (auto &l : list)
					result->mul(getData<Number>(l));
				return ObjectPtr(result);
			}
			template <>
			ObjectPtr div<Number>(const DataList &list) {
				Number *result = new Number(getData<Number>(list.front()));
				result->mul(getData<Number>(list.front()));
				for (auto &l : list)
					result->div(*(Number*)(l.get()));
				return ObjectPtr(result);
			}

			template <>
			ObjectPtr mod<Number>(const DataList &list) {
				Number *result = new Number(getData<Number>(list.front()));
				result->mod(getData<Number>(list.at(1)));
				return ObjectPtr(result);
			}

			ObjectPtr add(const DataList &list) {
				Object* result;
				auto type = list.front()->get_type();
				switch (type)
				{
				case T_Number:
					return add<Number>(list);
					break;
				case T_String:
					return add<String>(list);
					break;
				default:
					result = new Object();
					break;
				}
				return ObjectPtr(result);
			}

			ObjectPtr temp_num_func(const DataList &list, const ObjectPtr &op) {
				Object* result;
				auto type = list.front()->get_type();
				switch (type)
				{
				case T_Number:
					return op;
					break;
				default:
					result = new Object();
					break;
				}
				return ObjectPtr(result);
			}

			ObjectPtr sub(const DataList &list) {
				return temp_num_func(list, sub<Number>(list));
			}
			ObjectPtr mul(const DataList &list) {
				return temp_num_func(list, mul<Number>(list));
			}
			ObjectPtr div(const DataList &list) {
				return temp_num_func(list, div<Number>(list));
			}
			ObjectPtr mod(const DataList &list) {
				return temp_num_func(list, mod<Number>(list));
			}
		}
		std::string to_string(const Object &obj) {
			return obj.to_string();
		}
	}
}