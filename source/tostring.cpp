#include "basic.h"
#include "object.h"
#include "typeobject.h"
#include "function.h"
#include "typebase.h"
#include "tabledata.h"

namespace ICM
{
	// ToString
	string to_string(DefaultType type)
	{
		if (TypeInfoTable.find(type) != TypeInfoTable.end())
			return TypeInfoTable[type].name;
		else {
			switch (type) {
			case T_Null:       return "Null";
			case T_Object:     return "Object";
			case T_Nil:        return "Nil";
			case T_Identifier: return "Identifier";
			case T_Keyword:    return "Keyword";
			case T_Number:     return "Number";
			case T_Boolean:    return "Boolean";
			case T_String:     return "String";
			case T_Symbol:     return "Symbol";
			case T_List:       return "List";
			case T_Disperse:   return "Disperse";
			case T_Vary:       return "Vary";
			case T_Function:   return "Function";
			case T_Type:       return "Type";
			default:           return "UnfoundType";
			}
		}
	}
	string to_string(const ObjectPtr &obj) {
		return obj.to_string();
	}
	string to_string(const DataList &list) {
		return Convert::to_string<'[', ']'>(list.begin(), list.end(), [](const ObjectPtr &obj) { return to_string(obj); });
	}
	string to_string(const vector<ObjectPtr> &list) {
		return Convert::to_string<'[', ']'>(list.begin(), list.end(), [](const ObjectPtr &obj) { return to_string(obj); });
	}
	string to_string(const vector<Object*> &list) {
		return Convert::to_string<'[', ']'>(list.begin(), list.end(), [](const Object* obj) { return obj->to_string(); });
	}
	// Function
	string to_string(const Function::Signature::List &list) {
		return Convert::to_string(list.begin(), list.end());
	}

	string to_string(const Function::Signature &sign) {
		std::string str;

		const auto &its = sign.getInType();
		const auto &ots = sign.getOutType();
		if (!its.empty()) {
			if (its.size() != 1) str.push_back('(');
			str.append(Convert::to_string(its.begin(), its.end()));
			if (sign.isLastArgs()) str.push_back('*');
			if (its.size() != 1) str.push_back(')');
		}
		else {
			str.append("Void");
		}
		str.append(" -> ");
		str.append(to_string(ots));

		return str;
	}
	string to_string(const TypeObject &to) {
		string str(ICM::to_string(to.getType()));
		if (to.isFunc()) {
			str.append("(" + ICM::to_string(to.getSign()) + ")");
		}
		return str;
	}
	string to_string(const TypeBase::FunctionType& ft)
	{
		return TypeBase::to_string(ft);
	}
}
