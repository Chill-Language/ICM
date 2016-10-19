#include "typeobject.h"
#include "keyword.h"
#include "tabledata.h"

namespace ICM
{
	TypeObject::TypeObject(const DefaultType &type, const Function::Signature &valuetype)
		: type(type), sign(new Function::Signature(valuetype)) {}
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
	bool checkType(const TypeObject &parT, const TypeObject &argT)
	{
		if (parT.isVary())
			return true;
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
		return parT.getType() == argT.getType();
	}
	bool TypeObject::checkType(const TypeObject &tobj) const
	{
		return ICM::checkType(*this, tobj);
	}
}
