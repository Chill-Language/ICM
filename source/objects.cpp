#include "objectsdef.h"
#include "keyword.h"

namespace ICM
{
	namespace Objects
	{
		//=======================================
		// * Class String
		//=======================================

		//=======================================
		// * Class Symbol
		//=======================================

		//=======================================
		// * Class List
		//=======================================
		ListType* ListType::push(const ObjectPtr &op) {
			data.push_back(op);
			return this;
		}
		ListType* ListType::push(const DataList &dl) {
			data.insert(data.end(), dl.begin(), dl.end());
			return this;
		}
		ListType& ListType::operator+=(const ListType &dl) {
			data.insert(data.end(), dl.data.begin(), dl.data.end());
			return *this;
		}
		string ListType::to_string() const {
			return ICM::to_string(data);
		}
		string ListType::to_output() const {
			return Common::Convert::to_string<'[', ']'>(data.begin(), data.end(), [](const ObjectPtr &op) { return op.to_output(); });
		}

		//=======================================
		// * Class Disperse
		//=======================================
		string DisperseType::to_string() const {
			return Convert::to_string(data.begin(), data.end(), [](const ObjectPtr &op) { return ICM::to_string(op); });
		}
		string DisperseType::to_output() const {
			string str;
			for (auto &op : data)
				str.append(op.to_output());
			return str;
		}

		//=======================================
		// * Class Identifier
		//=======================================

		void IdentifierType::setData(const ObjectPtr &op) {
			if (op->getType() == T_Identifier)
				data = op.get<Identifier>()->getData().getData();
			else
				data = op;
		}
		void IdentifierType::setCopy(const ObjectPtr &op) {
			if (op.isType<Identifier>())
				setCopy(op.get<Identifier>()->getData().getData());
			else
				data = ObjectPtr(op->clone());
		}
		void IdentifierType::setRefer(const ObjectPtr &op) {
			if (op->getType() == T_Identifier) {
				const ObjectPtr &sop = op.get<Identifier>()->getData().getData();
				const ObjectPtr &refop = sop.isType<Identifier>() ? sop : op;
				const ObjectPtr &refopdata = refop.get<Identifier>()->getData().getData();
				if (data.get() != refopdata.get())
					data = refop;
				else
					data = refopdata;
			}
			else
				data = op;
		}

		const ObjectPtr& IdentifierType::getRealData() const {
			if (data.isType(T_Identifier))
				return data.get<Objects::Identifier>()->getData().getData();
			else
				return data;
		}

		/*ObjectPtr GetElt(const ObjectPtr &op) {
			if (op.isType(T_Variable))
				return GetElt(op.get<Variable>()->getData());
			else if (op.isType(T_Reference))
				return GetElt(op.get<Reference>()->getData());
			else
				return op;
		}
		ObjectPtr GetRef(const ObjectPtr &op) {
			if (op.isType(T_Variable))
				return ObjectPtr(new Reference("R", op));
			if (op.isType(T_Reference))
				return op;
			else
				return ObjectPtr(new Reference("R", op));
		}*/
		string to_string(const ListType &lt) {
			return lt.to_string();
		}
		string to_string(const DisperseType &lt) {
			return lt.to_string();
		}
		string to_string(const ErrorType &et) {
			return et.to_string();
		}
		string to_string(const FunctionType &ft) {
			return ft.to_string();
		}
		string to_string(const IdentifierType &ft) {
			return ft.to_string();
		}
		string to_output(const ListType &lt) {
			return lt.to_output();
		}
		string to_output(const DisperseType &lt) {
			return lt.to_output();
		}
		string to_output(const FunctionType &ft) {
			return ft.to_output();
		}
		string to_output(const IdentifierType &it) {
			return it.to_output();
		}
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
		case ICM::T_Object:     return new Objects::Object;
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
		//case ICM::T_Symbol:     return new Symbol;
		default:
			println("Unfind Type.");
			return nullptr;
		}
	}
	// Create Error
	ObjectPtr createError(const string &errinfo) {
		return ObjectPtr(new Objects::Error(Objects::ErrorType(errinfo)));
	}
	// Adjust ObjectPtr
	const ObjectPtr& adjustObjectPtr(const ObjectPtr &op) {
		if (op.isType(T_Identifier))
			return adjustObjectPtr(op.get<Objects::Identifier>()->getData().getData());
		else
			return op;
	}
	// Get TypeObject
	TypeObject getTypeObject(const ObjectPtr &op)
	{
		if (op.isType(T_Identifier))
			return ICM::TypeObject(T_Identifier, getTypeObject(op.get<Objects::Identifier>()->getData().getData()));
		else if (op.isType(T_Function)) {
			TypeObject t(T_Function);
			auto &ft = op.get<Objects::Function>()->getData().get_data();
			t.setFuncTableUnit(&ft);
			return t;
		}
		else
			return ICM::TypeObject(op.type());
	}
	// Get Disperse Iterator
	vector<ObjectPtr>::iterator begin(Objects::Disperse *disp)
	{
		return disp->getData().begin();
	}
	vector<ObjectPtr>::iterator end(Objects::Disperse *disp)
	{
		return disp->getData().end();
	}
}
