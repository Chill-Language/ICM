#pragma once
#include "basic.h"
#include "identifier.h"
#include "object.h"

namespace ICM
{
	struct IdentTableUnit
	{
		IdentTableUnit(IdentType type = I_Void, size_t index = 0)
			: type(type), index(index) {
			DyVarb = &Static.Nil;
		}

		IdentType type;
		size_t index;
		/*union {
			Object  svdata; // Reference From StVarb
			Object* dvref;  // Reference From DyVarb
			size_t  rvref;  // Reference From ReVarb
		};*/

		TypeUnit restype = T_Vary; // Restrict Type

		// TODO
		Object StVarb;
		Object *DyVarb;
		Object *Data;
		size_t FunctionIndex;
		TypeUnit TypeIndex;
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
}
