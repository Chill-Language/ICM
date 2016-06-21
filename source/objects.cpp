#include "objects.h"

namespace ICM
{
	namespace Objects
	{
		namespace Func
		{
			void print(const ObjectPtr &p) {
				switch (p->get_type())
				{
				case T_Number:
					Common::Output::print(p->to_string());
					break;
				case T_String:
					Common::Output::print(((String*)p.get())->get_data());
					break;
				default:
					Common::Output::print(p->to_string());
					break;
				}
			}
			template <>
			ObjectPtr sub<Number>(const DataList &list) {
				Number *result = new Number(*(Number*)(list.front().get()));
				result->add(*(Number*)(list.front().get()));
				for (auto &l : list)
					result->sub(*(Number*)(l.get()));
				return ObjectPtr(result);
			}

			ObjectPtr sum(const DataList &list) {
				Object* result;
				auto type = list.front()->get_type();
				switch (type)
				{
				case T_Number:
					return sum<Number>(list);
					break;
				case T_String:
					return sum<String>(list);
					break;
				default:
					result = new Object();
					break;
				}
				return ObjectPtr(result);
			}

			ObjectPtr sub(const DataList &list) {
				Object* result;
				auto type = list.front()->get_type();
				switch (type)
				{
				case T_Number:
					return sub<Number>(list);
					break;
				default:
					result = new Object();
					break;
				}
				return ObjectPtr(result);
			}
		}
		std::string to_string(const Object &obj) {
			return obj.to_string();
		}
	}
}