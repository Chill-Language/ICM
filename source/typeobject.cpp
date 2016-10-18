#include "typeobject.h"
#include "tostring.h"
#include "keyword.h"
#include "tabledata.h"

namespace ICM
{
	TypeObject::TypeObject(const DefaultType &type, const Function::Signature &valuetype)
		: type(type), valuetype(new TypeObject(valuetype.getOutType())), sign(new Function::Signature(valuetype)) {}
	string TypeObject::to_string() const {
		string str(ICM::to_string(type));
		if (type == T_Identifier) {
			str.append("(" + valuetype->to_string() + ")");
		}
		else if (type == T_Function) {
			str.append("(" + sign->to_string() + ")");
		}
		return str;
	}
	// P :Pars, A : Args
	// N : Numberr, I : Ident
	//  P\A    N  I(N) I(I(N))
	//   N     T   T     T
	//  I(N)   F   T     T
	// I(I(N)) F   F     T
	//  Var    T   T     T
	// 'parT' is par type, 'argT' is arg type.
	bool checkNonIdentType(const TypeObject &parT, const TypeObject &argT);
	bool checkType(const TypeObject &parT, const TypeObject &argT)
	{
		if (parT.isVary())
			return true;
		if (!parT.isIdent())
			return checkNonIdentType(parT, argT);
		if (argT.isIdent())
			return checkType(parT.getValueType(), argT.getValueType());
		return false;
	}
	bool checkNonIdentType(const TypeObject &parT, const TypeObject &argT)
	{
		if (parT.isFunc()) {
			if (argT.isFunc()) {
				auto &ps = parT.getSign();
				auto &as = argT.getSign();
				if (&ps) {
					if (&as)
						return ps.checkType(as);
					else {
						auto &ft = argT.getFuncTableUnit();
						for (size_t i : Range<size_t>(0, ft.size()))
							if (ps.checkType(ft[i].getSign()))
								return true;
						return false;
					}
				}
				else {
					return true;
				}
			}
		}
		if (!argT.isIdent())
			return parT.getType() == argT.getType();
		return checkNonIdentType(parT, argT.getValueType());
	}
	bool TypeObject::checkType(const TypeObject &tobj) const
	{
		return ICM::checkType(*this, tobj);
	}
}
