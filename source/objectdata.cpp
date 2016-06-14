#include "objectdata.h"
#include <cstring>
#include <cstdlib>

namespace ICM
{
	// ObjectData
	template <typename T>
	void ObjectData::release() {
		getPointer<T>()->~T();
		pointer = nullptr;
		size = 0;
	}
	ObjectData* ObjectData::clone() const {
		ObjectData *cpy = new ObjectData(*this);
		if (this->size) {
			cpy->pointer = memcpy((char*)malloc(this->size), this->pointer, this->size);
		}
		return cpy;
	}
	void ObjectData::setData(const std::string &data) {
		if (!pointer) {
			pointer = new std::string(data);
		}
		else {
			delete (std::string*)pointer;
			pointer = new std::string(data);
			//*(std::string*)pointer = std::string(data.c_str());
		}
	}
}
