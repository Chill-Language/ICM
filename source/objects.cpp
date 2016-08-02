#include "objects.h"
#include "tostring.h" // TODO

namespace ICM
{
	namespace Objects
	{
		//=======================================
		// * Functions
		//=======================================
		// Get Pointer
		template <typename T>
		T* getPointer(const ObjectPtr &op) {
			return static_cast<T*>(op.get());
		}

		//=======================================
		// * Class Object
		//=======================================
		Boolean* Object::equ(const ObjectPtr &obj) const {
			return new Boolean(type_equal(obj));
		}

		//=======================================
		// * Class Nil
		//=======================================
		Boolean* Nil::equ(const ObjectPtr &obj) const {
			return new Boolean(true);
		}
		//=======================================
		// * Class Boolean
		//=======================================
		Boolean* Boolean::equ(const ObjectPtr &obj) const {
			return new Boolean(this->data == getPointer<Boolean>(obj)->data);
		}

		//=======================================
		// * Class Number
		//=======================================
		Boolean* Number::equ(const ObjectPtr &obj) const {
			return new Boolean(this->data == getPointer<Number>(obj)->data);
		}
		Number* Number::add(const Number *obj) {
			this->data += obj->data;
			return this;
		}
		Number* Number::sub(const Number *obj) {
			this->data -= obj->data;
			return this;
		}
		Number* Number::mul(const Number *obj) {
			this->data *= obj->data;
			return this;
		}
		Number* Number::div(const Number *obj) {
			if (obj->data != 0)
				this->data /= obj->data;
			else
				println("Error of dived zero.");
			return this;
		}
		Number* Number::mod(const Number *obj) {
			this->data %= obj->data;
			return this;
		}

		//=======================================
		// * Class String
		//=======================================
		Boolean* String::equ(const ObjectPtr &obj) const {
			return new Boolean(this->to_string() == getPointer<String>(obj)->to_string());
		}
		String* String::add(const String *obj) {
			data = Common::charptr(self.data.to_string() + obj->data.to_string());
			return this;
		}

		//=======================================
		// * Class List
		//=======================================
		Boolean* List::equ(const ObjectPtr &obj) const {
			return new Boolean(this->data == getPointer<List>(obj)->data);
		}
		List* List::push(const ObjectPtr &op) {
			data.push_back(op);
			return this;
		}
		List* List::push(const DataList &dl) {
			data.insert(data.end(), dl.begin(),dl.end());
			return this;
		}
		List* List::add(const List *dl) {
			data.insert(data.end(), dl->data.begin(), dl->data.end());
			return this;
		}

		string List::to_string() const {
			return ICM::to_string(data);
		}

		namespace Func
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
					result = ObjectPtr(new Error(to_string_unfeined_function("+", list.front()->get_type())));
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
					result = ObjectPtr(new Error(to_string_unfeined_function(name, list.front()->get_type())));
				}
				return result;
			}
		}
	}
}
