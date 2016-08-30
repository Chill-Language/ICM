#ifndef _ICM_OBJECTMANAGER_H_
#define _ICM_OBJECTMANAGER_H_

#include "basic.h"
#include "bijectionmap.h"

namespace ICM
{
	namespace Objects { class Object; }

	//=======================================
	// * Class ObjectManager
	//=======================================
	class ObjectManager
	{
	public:
		ObjectManager() : ObjectTypePool(END_TYPE_ENUM), ObjectTypeCount(END_TYPE_ENUM) {}

		size_t newObjectPtr(Objects::Object *obj);
		void destroyObjectPtr(DefaultType typeID, size_t id);

		template <typename _OTy = Objects::Object>
		_OTy* getObjectPtr(DefaultType typeID, size_t id) {
			return ObjectTypePool[typeID].getKey(id);
		}

		void increaseCount(DefaultType typeID, size_t id);
		void decreaseCount(DefaultType typeID, size_t id);

	private:
		using ObjectPool = SerialBijectionMap<Objects::Object*>;
		vector<ObjectPool> ObjectTypePool;
		vector<vector<size_t>> ObjectTypeCount;
	};

	//=======================================
	// * Global Variable
	//=======================================
	extern ObjectManager GlobalObjectManager;

	//=======================================
	// * Class ObjectPtr
	//=======================================
	class ObjectPtr
	{
	public:
		explicit ObjectPtr(Objects::Object *op = nullptr);
		ObjectPtr(const ObjectPtr &op);
		~ObjectPtr();
		ObjectPtr& operator=(const ObjectPtr &op);

		Objects::Object* get() const {
			return GlobalObjectManager.getObjectPtr(this->type, this->index);
		}
		template <typename _OTy>
		_OTy* get() const {
			return static_cast<_OTy*>(this->get());
		}
		Objects::Object* operator->() const {
			return get();
		}

		bool isType(DefaultType type) const {
			return this->type == type;
		}
		template <typename _OTy>
		bool isType() const {
			return this->type == _OTy::Type;
		}

		bool operator==(nullptr_t) const {
			return type == T_Null;
		}
		operator bool() const {
			return type != T_Null;
		}

	private:
		DefaultType type;
		size_t index;
	};
	// For Testing Speed.
	//class ObjectPtr
	//{
	//public:
	//	ObjectPtr() : data(nullptr) {}
	//	ObjectPtr(nullptr_t) : data(nullptr) {}
	//	ObjectPtr(Objects::Object*obj) : data(obj) {}

	//	Objects::Object* get() const {
	//		return data;
	//	}
	//	template <typename _OTy>
	//	_OTy* get() const {
	//		return static_cast<_OTy*>(data);
	//	}
	//	DefaultType getType() const;
	//	bool isType(DefaultType type) const {
	//		return getType() == type;
	//	}
	//	template <typename _OTy>
	//	bool isType() const {
	//		return getType() == _OTy::Type;
	//	}

	//	bool operator==(nullptr_t) const {
	//		return getType() == T_Null;
	//	}
	//	operator bool() const {
	//		return getType() != T_Null;
	//	}
	//	Objects::Object* operator->() const {
	//		return data;
	//	}

	//	Objects::Object *data;
	//};

	//=======================================
	// * SymbolTable
	//=======================================
	class SymbolTable
	{
	public:
		SymbolTable() {}
		size_t find(const string &str) {
			return MapData.findKey(str);
		}
		size_t add(const string &str) {
			size_t id = MapData.currentIndex();
			if (MapData.insert(str, id))
				return id;
			else
				return find(str);
		}
		const string& get(size_t id) {
			return MapData.getKey(id);
		}

	private:
		BijectionMap<string, size_t> MapData;
	};

	//=======================================
	// * Global Variable
	//=======================================
	extern SymbolTable GlobalSymbolTable;
}

#endif
