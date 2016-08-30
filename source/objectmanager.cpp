#include "objects.h"
#include "objectmanager.h"
#include "tostring.h"
#include "config.h"
#include "keyword.h"

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
		//ObjectTypeCount[typeID].erase(id);
		delete obj;
	}

	void ObjectManager::increaseCount(DefaultType typeID, size_t id) {
		if (typeID == T_Null) return;
		auto &count = ObjectTypeCount[typeID][id];
		//println(" Ic : ", to_string(typeID), ",", id, ",", count, "->", count + 1, " V : ", getObjectPtr(typeID, id)->to_string());
		count++;
	}
	void ObjectManager::decreaseCount(DefaultType typeID, size_t id) {
		if (typeID == T_Null) return;
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
	//DefaultType ObjectPtr::getType() const {
	//	return data->get_type();
	//}
	ObjectPtr::ObjectPtr(Objects::Object* op) {
		if (op == nullptr) {
			this->type = T_Null;
		}
		else {
			this->type = op->get_type();
			this->index = GlobalObjectManager.newObjectPtr(op);
		}
		GlobalObjectManager.increaseCount(this->type, this->index);
	}
	ObjectPtr::ObjectPtr(const ObjectPtr &op)
		: type(op.type), index(op.index) {
		GlobalObjectManager.increaseCount(this->type, this->index);
	}
	ObjectPtr::~ObjectPtr() {
		GlobalObjectManager.decreaseCount(this->type, this->index);
	}
	ObjectPtr& ObjectPtr::operator=(const ObjectPtr &op) {
		if (this->type == op.type && this->index == op.index)
			return *this;
		GlobalObjectManager.decreaseCount(this->type, this->index);
		this->type = op.type;
		this->index = op.index;
		GlobalObjectManager.increaseCount(this->type, this->index);
		return *this;
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
