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
		string List::to_output() const {
			return Common::Convert::to_string(data.begin(), data.end(), [](const ObjectPtr &op) { return op->to_output(); });
		}

		//=======================================
		// * Class Identifier
		//=======================================
		ASTNode* Identifier::getRefNode() {
			return data;
		}
		ASTNode* Identifier::getDatNode() {
			return calcASTNode(getRefNode());
		}

		void Identifier::setValue(ASTNode *node) {
			if (node->getNodeType() == AST_DATA && node->getObjtype() == T_Identifier)
				this->data = getPointer<Identifier>(node->getdata())->data;
			else
				this->data = node;
		}
		DefaultType Identifier::getValueType() const {
			return this->data->getObjtype();
		}

		string Identifier::to_string() const {
			return name.to_string() + "(" + ICM::to_string(data) + ")";
		}
		string Identifier::to_output() const {
			return data->getdata()->to_output();
		}
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
			return getPointer<Objects::Identifier>(ptr)->getRefNode()->getdata();
		else
			return ptr;
	}
}
