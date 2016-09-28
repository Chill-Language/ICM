#include "objectsdef.h"
#include "tostring.h"
#include "keyword.h"

namespace ICM
{
	namespace Objects
	{
		//=======================================
		// * Class String
		//=======================================
		String* String::add(const String *obj) {
			data = Common::charptr(self.data.to_string() + obj->data.to_string());
			return this;
		}

		//=======================================
		// * Class Symbol
		//=======================================

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
		List* List::add(const List *dl) {
			data.insert(data.end(), dl->data.begin(), dl->data.end());
			return this;
		}
		string List::to_string() const {
			return ICM::to_string(data);
		}
		string List::to_output() const {
			return Common::Convert::to_string<'[',']'>(data.begin(), data.end(), [](const ObjectPtr &op) { return op.to_output(); });
		}
		
		//=======================================
		// * Class Disperse
		//=======================================
		string Disperse::to_string() const {
			return Convert::to_string(data.begin(), data.end(), [](const ObjectPtr &op) { return ICM::to_string(op); });
		}
		string Disperse::to_output() const {
			string str;
			for (auto &op : data)
				str.append(op.to_output());
			return str;
		}

		//=======================================
		// * Class Identifier
		//=======================================
	}

	//=======================================
	// * Functions
	//=======================================
	// Create Object
	Objects::Object* createObject(DefaultType type)
	{
		using namespace Objects;
		switch (type)
		{
		case ICM::T_Object:     return new Object;
		case ICM::T_Nil:        return new Nil;
		case ICM::T_Error:      return new Error;
		case ICM::T_Identifier: return new Identifier;
		case ICM::T_Keyword:    return new Keyword;
		case ICM::T_List:       return new List;
		case ICM::T_Disperse:   return nullptr; // TODO
		case ICM::T_Number:     return new Objects::Number;
		case ICM::T_String:     return new String;
		case ICM::T_Char:       return nullptr; // TODO
		case ICM::T_Function:   return new Objects::Function;
		case ICM::T_Type:       return nullptr; // TODO
		case ICM::T_Boolean:    return new Boolean;
		case ICM::T_Symbol:     return new Symbol;
		default:
			println("Unfind Type.");
			return nullptr;
		}
	}
	// Create Error
	ObjectPtr createError(const string &errinfo) {
		return ObjectPtr(new Objects::Error(errinfo));
	}
	// Adjust ObjectPtr
	const ObjectPtr& adjustObjectPtr(const ObjectPtr &op) {
		if (op.isType(T_Identifier))
			return adjustObjectPtr(op.get<Objects::Identifier>()->getData());
		else
			return op;
	}
	// Get TypeObject
	TypeObject getTypeObject(const ObjectPtr &op)
	{
		if (op.isType(T_Identifier))
			return ICM::TypeObject(T_Identifier, getTypeObject(op.get<Objects::Identifier>()->getData()));
		else if (op.isType(T_Function)) {
			TypeObject t(T_Function);
			auto &ft = op.get<Objects::Function>()->get_data();
			t.setFuncTableUnit(&ft);
			return t;
		}
		else
			return ICM::TypeObject(op.type());
	}
	// Get Disperse Iterator
	vector<ObjectPtr>::iterator begin(Objects::Disperse *disp)
	{
		return disp->begin();
	}
	vector<ObjectPtr>::iterator end(Objects::Disperse *disp)
	{
		return disp->end();
	}
}
