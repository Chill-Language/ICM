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

		//=======================================
		// * Class Number
		//=======================================
		Number* Number::add(const Number *obj)
		{
			this->data += obj->data;
			return this;
		}
		Number* Number::sub(const Number *obj)
		{
			this->data -= obj->data;
			return this;
		}
		Number* Number::mul(const Number *obj)
		{
			this->data *= obj->data;
			return this;
		}
		Number* Number::div(const Number *obj)
		{
			if (obj->data != 0)
				this->data /= obj->data;
			else
				println("Error of dived zero.");
			return this;
		}
		Number* Number::mod(const Number *obj)
		{
			this->data %= obj->data;
			return this;
		}

		//=======================================
		// * Class String
		//=======================================
		String* String::add(const String *obj) {
			data = Common::charptr(self.data.to_string() + obj->data.to_string());
			return this;
		}

		//=======================================
		// * Class List
		//=======================================
		List* List::push(const ObjectPtr &op) {
			data.push_back(op);
			return this;
		}
		List* List::push(const DataList &dl) {
			data.insert(data.end(), dl.begin(),dl.end());
			return this;
		}

		string List::to_string() const {
			return ICM::to_string(data);
		}

		namespace Func
		{
			// Get Pointer
			template <typename T>
			T* getPointer(const ObjectPtr &op) {
				return static_cast<T*>(op.get());
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

			ObjectPtr add(const DataList &list) {
				ObjectPtr result;

				ObjectPtr front = list.front();
				if (front->get_type() == T_Number) {
					Number *tmp = getPointer<Number>(front)->clone();
					for (auto i : Range<size_t>(1, list.size() - 1))
						tmp->add(getPointer<Number>(list[i]));
					result = ObjectPtr(tmp);
				}
				else if (front->get_type() == T_String) {
					String *tmp = getPointer<String>(front)->clone();
					for (auto i : Range<size_t>(1, list.size() - 1))
						tmp->add(getPointer<String>(list[i]));
					result = ObjectPtr(tmp);
				}
				else {
					result = ObjectPtr(new Error(to_string_unfeined_function("+", list.front()->get_type())));
				}
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

			template <typename Func>
			ObjectPtr default_num_varfunc(const DataList &list, Func func, const std::string &name) {
				ObjectPtr result;
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
