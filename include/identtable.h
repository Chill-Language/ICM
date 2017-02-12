#pragma once
#include "basic.h"
#include "identifier.h"
#include "object.h"

namespace ICM
{
	struct IdentTableUnit
	{
		IdentTableUnit(IdentType type = I_Void, size_t index = 0)
			: type(type), index(index) { initialize(); }

		void initialize() {
			switch (type) {
			case I_Data:
				break;
			case I_StVarb:
				StVarb = nullptr;
				dimtype = T_Null;
				break;
			case I_DyVarb:
				DyVarb = &Static.Nil;
				restype = T_Vary;
				break;
			default:
				break;
			}
		}

		IdentType type;
		size_t index;

		union {
			TypeUnit dimtype;  // Dim Type for StVarb
			TypeUnit restype;  // Restrict Type for DyVarb
		};

		union {
			Object *StVarb;
			Object *DyVarb;
			Object *Data;
			size_t FunctionIndex;
			TypeUnit TypeIndex;
		};
	};

	using IdentKey = size_t;
	class IdentTable : private IndexTable<IdentKey, IdentTableUnit>
	{
	public:
		IdentTable() {}

		size_t insert(const IdentKey &key, IdentType type) {
			size_t id = IndexTable::insert(key, IdentTableUnit(type));
			this->get(id).index = id;
			return id;
		}
		size_t find(const IdentKey &key) const {
			return IndexTable::find(key);
		}
		IdentTableUnit& at(size_t index) {
			return IndexTable::get(index);
		}
		size_t size() const {
			return IndexTable::size();
		}
		const IdentKey& getKey(size_t index) const {
			return IndexTable::getKey(index);
		}

	};

	enum IdentSpaceType
	{
		IS_Global,
		IS_Namespace,
		IS_Module,
		IS_Function,
		IS_Lambda,
		IS_Struct,
		IS_Expr,
	};

	struct IdentSpaceTableUnit
	{
		IdentSpaceTableUnit(size_t index, IdentSpaceType type, IdentSpaceTableUnit *father)
			: index(index), type(type), father(father), data(new IdentTable) {}

		size_t index;
		IdentSpaceType type;
		IdentSpaceTableUnit* father;
		unique_ptr<IdentTable> data;
	};

	class IdentSpaceTable
	{
	public:
		IdentSpaceTable() {}

	private:
		IdentSpaceType type;
	};
}
