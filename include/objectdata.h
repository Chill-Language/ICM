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

		template <typename T>
		T* getPointer() {
			return (T*)pointer;
		}
		template <typename T>
		const T* getPointer() const {
			return (const T*)pointer;
		}
		template <typename T>
		T getData() const {
			return *getPointer<T>();
		}

		friend std::string to_string(const ObjectData *obj);

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