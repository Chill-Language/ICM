#include "objectsdef.h"
#include "tostring.h"
#include "keyword.h"
#include "runast.h"

namespace ICM
{
	namespace Objects
	{
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
			this->data /= obj->data;
			return this;
		}
		Number* Number::mod(const Number *obj) {
			this->data = Common::Number::mod(this->data, obj->data);
			return this;
		}
		Number* Number::rem(const Number *obj) {
			this->data = Common::Number::rem(this->data, obj->data);
			return this;
		}
		bool Number::operator<(const Number &obj) {
			return self.data < obj.data;
		}
		bool Number::operator<=(const Number &obj) {
			return self.data <= obj.data;
		}
		bool Number::operator>(const Number &obj) {
			return self.data > obj.data;
		}
		bool Number::operator>=(const Number &obj) {
			return self.data >= obj.data;
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
		string List::to_output() const {
			return Common::Convert::to_string<'[',']'>(data.begin(), data.end(), [](const ObjectPtr &op) { return op->to_output(); });
		}
		
		//=======================================
		// * Class Disperse
		//=======================================
		Boolean* Disperse::equ(const ObjectPtr &obj) const {
			return new Boolean(this->data == getPointer<Disperse>(obj)->data);
		}
		string Disperse::to_string() const {
			return Convert::to_string(data.begin(), data.end(), [](const ObjectPtr &op) { return ICM::to_string(op); });
		}
		string Disperse::to_output() const {
			string str;
			for (auto &op : data)
				str.append(op->to_output());
			return str;
		}

		//=======================================
		// * Class Identifier
		//=======================================
	}

	//=======================================
	// * Functions
	//=======================================
	// Create Error
	ObjectPtr createError(const string &errinfo)
	{
		return ObjectPtr(new Objects::Error(errinfo));
	}
	// Adjust ObjectPtr
	ObjectPtr adjustObjectPtr(const ObjectPtr &ptr) {
		if (ptr->get_type() == T_Identifier)
			return getPointer<Objects::Identifier>(ptr)->getData();
		else
			return ptr;
	}
	// Get TypeObject
	TypeObject getTypeObject(const ObjectPtr &op)
	{
		if (op->get_type() == T_Identifier)
			return ICM::TypeObject(T_Identifier, getTypeObject(getPointer<Objects::Identifier>(op)->getData()));
		else if (op->get_type() == T_Function) {
			TypeObject t =  TypeObject(T_Function);
			auto &ft = getPointer<Objects::Function>(op)->get_data();
			t.setFuncTableUnit(&ft);
			return t;
		}
		else
			return ICM::TypeObject(op->get_type());
	}
	// Get Disperse Iterator
	DataList::iterator begin(Objects::Disperse *disp)
	{
		return disp->begin();
	}
	DataList::iterator end(Objects::Disperse *disp)
	{
		return disp->end();
	}
}
