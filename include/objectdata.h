#ifndef _ICM_OBJECTDATA_H_
#define _ICM_OBJECTDATA_H_

#include <string>

namespace ICM
{
	// ObjectData
	class ObjectData
	{
	public:
		ObjectData() = default;
		ObjectData(const ObjectData &obj) = default;
		template <typename T> explicit ObjectData(const T &data) { setData(data); }
		~ObjectData() { free(pointer); }

		template <typename T> void release();
		ObjectData* clone() const;

		template <typename T>
		void setData(const T &data);
		void setData(const std::string &data);

		template <typename T>
		T* getPointer() {
			return (T*)pointer;
		}
		template <typename T = void>
		const T* getPointer() const {
			return (const T*)pointer;
		}
		template <typename T>
		T getData() const {
			return *getPointer<T>();
		}

	private:
		void *pointer = nullptr;
		size_t size = 0;
	};

	template <typename T>
	void ObjectData::setData(const T &data) {
		if (!pointer) {
			this->size = sizeof(T);
			pointer = memcpy((char*)malloc(this->size), &data, this->size);
		}
		else {
			*getPointer<T>() = data;
		}
	}
}

#endif
