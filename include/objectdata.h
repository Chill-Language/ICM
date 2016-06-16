#ifndef _ICM_OBJECTDATA_H_
#define _ICM_OBJECTDATA_H_

#include "basic.h"
#include "objects.h"

namespace ICM
{
	class ObjectData
	{
	public:
		ObjectData() = default;
		explicit ObjectData(Objects::Object *dat) : pointer(dat) {}

		ObjectData* clone() const {
			return new ObjectData(pointer->clone());
		}
		void setData(const ObjectPtr &data) {
			pointer = data;
		}

		Objects::Object* getPointer() {
			return pointer.get();
		}
		Objects::Object& getData() {
			return *getPointer();
		}

		const Objects::Object* getPointer() const {
			return pointer.get();
		}
		const Objects::Object& getData() const {
			return *getPointer();
		}

	private:
		ObjectPtr pointer = nullptr;
	};
}

#endif
