#ifndef _ICM_OBJECTMANAGER_H_
#define _ICM_OBJECTMANAGER_H_

#include "basic.h"
#include "bijectionmap.h"
#include "objectenum.h"

namespace ICM
{
	namespace Objects { class Object; }

	//=======================================
	// * Class ObjectManager
	//=======================================
	class ObjectManager
	{
	public:
		ObjectManager() : ObjectTypePool(END_TYPE_ENUM), ObjectTypeCount(END_TYPE_ENUM) {
			ObjectTypePool[T_Null].insert(nullptr);
			ObjectTypeCount[T_Null].push_back(0);
		}

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
			return GlobalObjectManager.getObjectPtr(this->_type, this->_index);
		}
		template <typename _OTy>
		_OTy* get() const {
			return static_cast<_OTy*>(this->get());
		}
		Objects::Object* operator->() const {
			return get();
		}

		bool isType(DefaultType type) const {
			return this->type() == type;
		}
		template <typename _OTy>
		bool isType() const {
			return this->type() == _OTy::Type;
		}

		bool operator==(std::nullptr_t) const {
			return type() == T_Null;
		}
		operator bool() const {
			return type() != T_Null;
		}
        DefaultType type() const {
            return _type;
        }
		string to_string() const;
		string to_output() const;

	private:
		DefaultType _type;
		size_t _index;
	};
	// For Testing Speed.
	/*class ObjectPtr
	{
	public:
		ObjectPtr() : data(nullptr) {}
		ObjectPtr(nullptr_t) : data(nullptr) {}
		ObjectPtr(Objects::Object *obj) : data(obj) {}
		ObjectPtr(const ObjectPtr &op) : data(op.data) {}

		Objects::Object* get() const {
			return data;
		}
		template <typename _OTy>
		_OTy* get() const {
			return static_cast<_OTy*>(data);
		}
		bool isType(DefaultType type) const {
			return this->type() == type;
		}
		template <typename _OTy>
		bool isType() const {
			return type() == _OTy::Type;
		}

		bool operator==(nullptr_t) const {
			return type() == T_Null;
		}
		operator bool() const {
			return type() != T_Null;
		}
		Objects::Object* operator->() const {
			return data;
		}
		string to_string() const;
		string to_output() const;
		DefaultType type() const;

		Objects::Object *data;
	};*/

	//=======================================
	// * Class ObjectPool
	//=======================================
	class ObjectPool
	{
		using Object = Objects::Object;
	public:
		struct ObjectPoolPtr
		{
			ObjectPoolPtr(DefaultType type, size_t index)
				: _type(type), _index(index) {}
			Object* get(const ObjectPool &objpool) const {
				return objpool.get(*this);
			}
			template <typename T>
			T* get(const ObjectPool &objpool) const {
				return static_cast<T*>(objpool.get(*this));
			}
			bool isType(DefaultType type) const {
				return this->type() == type;
			}
			template <typename _OTy>
			bool isType() const {
				return this->type() == _OTy::Type;
			}
			DefaultType type() const {
				return _type;
			}
			size_t index() const {
				return _index;
			}

		private:
			DefaultType _type;
			size_t _index;
		};
	public:
		ObjectPool() : data(END_TYPE_ENUM) {}
		~ObjectPool();
		ObjectPoolPtr set(Object *op);
		Object* get(const ObjectPoolPtr &op) const;
		void del(const ObjectPoolPtr &op);
		void write(File &file) const;
		void read(File &file);

	private:
		vector<vector<Object*>> data;
	};

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
