#include "objects.h"
#include "objectmanager.h"
#include "tostring.h"
#include "config.h"

namespace ICM
{
	//=======================================
	// * Class ObjectManager
	//=======================================
	size_t ObjectManager::newObjectPtr(Objects::Object *obj) {
		DefaultType typeID = obj->get_type();
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
			println("New : ", to_string(typeID), ",", id, " V : ", obj->to_string());
		return id;
	}
	void ObjectManager::destroyObjectPtr(DefaultType typeID, size_t id) {
		Objects::Object *obj = getObjectPtr(typeID, id);
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
	/*DefaultType ObjectPtr::type() const {
		return data->get_type();
	}*/
	ObjectPtr::ObjectPtr(Objects::Object* op) {
		if (op == nullptr) {
			this->_type = T_Null;
			this->_index = 0;
		}
		else {
			this->_type = op->get_type();
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
	KeywordTable DefKetwordTable;
}
