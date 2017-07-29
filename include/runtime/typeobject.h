#pragma once
#include "basic.h"
#include "parser/objectenum.h"

namespace ICM
{
	namespace Function { class Signature; }
	class FuncTableUnit;

	class TypeObject
	{
	public:
		TypeObject() : TypeObject(T_Null) {}
		TypeObject(const DefaultType &type) : type(type) {}
		TypeObject(const DefaultType &type, const TypeObject &valuetype)
			: type(type) {}
		TypeObject(const DefaultType &type, const ICM::Function::Signature &valuetype);

		DefaultType getType() const {
			return type;
		}
		const ICM::Function::Signature& getSign() const {
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
			return type == tobj.type; // TODO
		}
		bool operator!=(const TypeObject &tobj) const {
			return !(*this == tobj);
		}

		bool checkType(const TypeObject &type) const;

	private:
		DefaultType type;
		ICM::Function::Signature* sign = nullptr;
		const FuncTableUnit *functableunit = nullptr;
	};

	// Get TypeObject
	string to_string(const TypeObject &to);
}
