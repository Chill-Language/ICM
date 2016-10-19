#ifndef _ICM_TYPEOBJECT_H_
#define _ICM_TYPEOBJECT_H_

#include "basic.h"
#include "objects.h"
#include "lightlist.h"

namespace ICM
{
	namespace Function { class Signature; }
	class FuncTableUnit;

	class TypeObject
	{
	public:
		TypeObject() : TypeObject(T_Null) {}
		TypeObject(const DefaultType &type)
			: type(type) {
			if (type == T_Identifier)
				valuetype = new TypeObject(T_Vary);
		}
		TypeObject(const DefaultType &type, const TypeObject &valuetype)
			: type(type), valuetype(new TypeObject(valuetype)) {}
		TypeObject(const DefaultType &type, const ICM::Function::Signature &valuetype);

		DefaultType getType() const {
			return type;
		}
		TypeObject getValueType() const {
			return valuetype ? *valuetype : type;
		}
		const Function::Signature& getSign() const {
			return *sign;
		}
		void setFuncTableUnit(const FuncTableUnit *functableunit) {
			this->functableunit = functableunit;
		}
		const FuncTableUnit& getFuncTableUnit() const {
			return *functableunit;
		}
		bool isFunc() const {
			return type == T_Function;
		}
		bool isVary() const {
			return type == T_Vary;
		}
		bool operator==(const TypeObject &tobj) const {
			return type == tobj.type && (valuetype == nullptr || *valuetype == *tobj.valuetype); // TODO
		}
		bool operator!=(const TypeObject &tobj) const {
			return !(*this == tobj);
		}

		bool checkType(const TypeObject &type) const;

		string to_string() const;

	private:
		DefaultType type;
		union {
			TypeObject* valuetype = nullptr;
			Function::Signature* sign;
		};
		const FuncTableUnit *functableunit = nullptr;
	};

	ICM::TypeObject getTypeObject(const ObjectPtr &op);
	std::string to_string(const TypeObject &to);
	std::string to_string(const shared_ptr<TypeObject> &top);
}

#endif
