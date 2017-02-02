#pragma once
#include "basic.h"
#include "identifier.h"
#include "object.h"

namespace ICM
{
	struct IdentTableUnit
	{
		IdentTableUnit(IdentType type = I_Void, size_t index = 0)
			: type(type), index(index) {}

		IdentType type;
		size_t index;
		/*union {
			Object  svdata; // Reference From StVarb
			Object* dvref;  // Reference From DyVarb
			size_t  rvref;  // Reference From ReVarb
		};*/

		// TODO
		Object StVarb;
		Object *DyVarb;
		size_t FunctionIndex;
	};


	class IdentTable : private IndexTable<string, IdentTableUnit>
	{
	public:
		IdentTable() {}

		size_t insert(const string &name, IdentType type) {
			size_t id = IndexTable::insert(name, IdentTableUnit(type));
			this->get(id).index = id;
			return id;
		}
		size_t find(const string &name) const {
			return IndexTable::find(name);
		}
		IdentTableUnit& at(size_t index) {
			return IndexTable::get(index);
		}
		size_t size() const {
			return IndexTable::size();
		}
		const string& getName(size_t index) const {
			return IndexTable::getKey(index);
		}

	};
}
