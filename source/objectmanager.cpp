#include "object.h"
#include "objectmanager.h"
#include "config.h"
#include "tabledata.h"

namespace ICM
{
	//=======================================
	// * Class ObjectManager
	//=======================================
	size_t ObjectManager::newObjectPtr(Object *obj) {
		TypeUnit typeID = obj->type;
		auto &table = ObjectTypePool[typeID];
		size_t id = table.currentIndex();
		bool res = table.insert(obj);
		if (res) {
			if (id == ObjectTypeCount[typeID].size()) {
				ObjectTypeCount[typeID].push_back(0);
			}
		}
		else {
			id = table.findKey(obj);
		}
		if (GlobalConfig.PrintObjectAllot)
			println("New : ", to_string((DefaultType)typeID), ",", id, " V : ", obj->to_string());
		return id;
	}
	void ObjectManager::destroyObjectPtr(DefaultType typeID, size_t id) {
		Object *obj = getObjectPtr(typeID, id);
		if (GlobalConfig.PrintObjectAllot)
			println("Destroy : ", to_string(typeID), ",", id, "; V : ", obj->to_string());
		ObjectTypePool[typeID].eraseID(id);
		delete obj;
	}

	void ObjectManager::increaseCount(DefaultType typeID, size_t id) {
		auto &count = ObjectTypeCount[typeID][id];
		//println(" Ic : ", to_string(typeID), ",", id, ",", count, "->", count + 1, " V : ", getObjectPtr(typeID, id)->to_string());
		count++;
	}
	void ObjectManager::decreaseCount(DefaultType typeID, size_t id) {
		auto &count = ObjectTypeCount[typeID][id];
		//println(" Dc : ", to_string(typeID), ",", id, ",", count, "->", count - 1, " V : ", getObjectPtr(typeID, id)->to_string());
		count--;
		if (count == 0)
			destroyObjectPtr(typeID, id);
	}

	//=======================================
	// * Class ObjectPtr
	//=======================================

	// For Testing Speed
#if !USE_RAW_POINTER
#if USE_DIRECT_POINTER
	DefaultType ObjectPtr::type() const {
		return (DefaultType)data->type;
	}
#else
	ObjectPtr::ObjectPtr(Object* op) {
		if (op == nullptr) {
			this->_type = T_Null;
			this->_index = 0;
		}
		else {
			this->_type = (DefaultType)op->type;
			this->_index = GlobalObjectManager.newObjectPtr(op);
		}
		GlobalObjectManager.increaseCount(this->_type, this->_index);
	}
	ObjectPtr::ObjectPtr(const ObjectPtr &op)
		: _type(op._type), _index(op._index) {
		GlobalObjectManager.increaseCount(this->_type, this->_index);
	}
	ObjectPtr::~ObjectPtr() {
		GlobalObjectManager.decreaseCount(this->_type, this->_index);
	}
	ObjectPtr& ObjectPtr::operator=(const ObjectPtr &op) {
		if (this->_type == op._type && this->_index == op._index)
			return *this;
		GlobalObjectManager.decreaseCount(this->_type, this->_index);
		this->_type = op._type;
		this->_index = op._index;
		GlobalObjectManager.increaseCount(this->_type, this->_index);
		return *this;
	}
#endif
	string ObjectPtr::to_string() const {
		if (this->get() == nullptr)
			return "Null";
		return this->get()->to_string();
	}
	string ObjectPtr::to_output() const {
		if (this->get() == nullptr)
			return "Null";
		return this->get()->to_output();
	}
#endif

	//=======================================
	// * Class ObjectPool
	//=======================================
	ObjectPool::~ObjectPool() {
		for (auto &v : data)
			for (auto *p : v)
				delete p;
	}
	ObjectPool::ObjectPoolPtr ObjectPool::set(Object *op) {
		DefaultType type = (DefaultType)op->type;
		size_t index = data[type].size();
		data[type].push_back(op);
		return ObjectPoolPtr(type, index);
	}
	ObjectPool::Object* ObjectPool::get(const ObjectPoolPtr &op) const {
		return data[op.type()][op.index()]->clone();
	}
	void ObjectPool::del(const ObjectPoolPtr &op) {

	}
	void ObjectPool::write(File &file) const {
		//for (auto i : Range<size_t>(T_Null, END_TYPE_ENUM)) {
		//	file.write<int32_t>(i);
		//	file.write<int32_t>(data[i].size());
		//	for (auto *op : data[i])
		//		op->write(file);
		//}
	}
	void ObjectPool::read(File &file) {
	//	int32_t type;
	//	int32_t size;
	//	for (auto i : Range<size_t>(T_Null, END_TYPE_ENUM)) {
	//		file.read(type);
	//		file.read(size);
	//		data[i].resize(size);
	//		for (auto &op : data[i]) {
	//			op = createObject((DefaultType)type);
	//			op->read(file);
	//		}
	//	}
	}

	//=======================================
	// * Global Variable
	//=======================================
	// It's necessary to put GlobalObjectManager front.
	// Because some Tables will use ObjectPtr, and when destory themselves, it would call function from GlobalObjectManager.
	// If put behind, GlobalObjectManager will destory before these Tables.
	ObjectManager GlobalObjectManager;
	SymbolTable GlobalSymbolTable;

	FuncTable DefFuncTable;
	FuncTable AddFuncTable;
	VariableTable DefVariableTable;
	VariableTable AddVariableTable;
}
