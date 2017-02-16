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

	using IdentSpaceIndex = uinth_t;  // TODO : Element now include uint_t, using IdentIndex (2 * uinth_t)
	using IdentBasicIndex = uinth_t;

	struct IdentIndex
	{
		IdentSpaceIndex space_index;
		IdentBasicIndex ident_index;
	};

	struct IdentSpaceUnit
	{
		IdentSpaceUnit(IdentSpaceIndex index, IdentSpaceType type, IdentSpaceIndex superior)
			: index(index), type(type), superior(superior), data(new IdentTable()) {}

		IdentTable& getIdentTable() {
			return *data;
		}

		IdentSpaceIndex index;
		IdentSpaceType type;
		IdentSpaceIndex superior;
		unique_ptr<IdentTable> data;
	};

	class IdentManager
	{
	public:
		IdentManager() {
			Table.push_back(IdentSpaceUnit(0, IS_Global, 0));
			reset();
		}

		void addSpace(IdentSpaceType istype) {
			IdentSpaceIndex index = static_cast<IdentSpaceIndex>(Table.size());
			IdentSpaceIndex superior = CurrentSpace->index;
			Table.push_back(IdentSpaceUnit(index, istype, superior));
			CurrentSpace = &Table.back();
		}

		IdentSpaceUnit& getSpace(IdentSpaceIndex sindex) {
			return Table.at(sindex);
		}

		IdentSpaceUnit& getCurrentSpace() {
			return *CurrentSpace;
		}
		IdentTable& getCurrentIdentTable() {
			return getCurrentSpace().getIdentTable();
		}

		void reset() {
			CurrentSpace = &Table.at(0);
		}

	private:
		IdentSpaceUnit *CurrentSpace;
		vector<IdentSpaceUnit> Table;


	};

	extern IdentManager GlobalIdentManager;

	// IdentTable
	inline IdentTable& getCurrentIdentTable() {
		return GlobalIdentManager.getCurrentIdentTable();
	}
	inline size_t findFromIdentTable(const IdentKey &key) {
		return getCurrentIdentTable().find(key);
	}
	inline size_t getIdentTableSize() {
		return getCurrentIdentTable().size();
	}
	inline IdentTableUnit& getFromIdentTable(size_t index) {
		return getCurrentIdentTable().at(index);
	}
	inline size_t insertFromIdentTable(const IdentKey &key, IdentType type) {
		return getCurrentIdentTable().insert(key, type);
	}
	inline auto getKeyFromIdentTable(size_t index) {
		return getCurrentIdentTable().getKey(index);
	}
}
