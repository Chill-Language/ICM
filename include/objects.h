#ifndef _ICM_OBJECTS_H_
#define _ICM_OBJECTS_H_

#include "basic.h"

namespace ICM
{
	namespace Objects
	{
		class Object
		{
		public:
			//Object() {}
			virtual ~Object() {}
			virtual std::string to_string() const {
				return "Object";
			}
			virtual DefaultType get_type() const {
				return T_Nil;
			}
			virtual Object* clone() const {
				return new Object(*this);
			}
		
		private:
			//Object(const Object&) {}
		};

		class Number : public Object
		{
		public:
			explicit Number(int dat = 0) : data(dat) {}
			//Number(const Number &num) : data(num.data) {}

			Number& add(const Number &b) {
				self.data += b.data;
				return self;
			}
			Number& sub(const Number &b) {
				self.data -= b.data;
				return self;
			}
			std::string to_string() const {
				return std::to_string(data);
			}
			DefaultType get_type() const {
				return T_Number;
			}
			Number* clone() const {
				return new Number(*this);
			}

		private:
			int data;
		};

		class String : public Object
		{
		public:
			explicit String(const std::string &dat = "") : data(dat) {}

			String& add(const String &b) {
				data = Common::charptr(self.data.to_string() + b.data.to_string());
				return self;
			}
			std::string to_string() const {
				return '"' + data.to_string() + '"';
			}
			std::string get_data() const {
				return data.to_string();
			}
			DefaultType get_type() const {
				return T_String;
			}
			String* clone() const {
				return new String(*(this->data.clone()));
			}

		private:
			Common::charptr data;
			explicit String(const Common::charptr &dat) : data(dat) {}
		};

		class Identifier : public Object
		{
		public:
			explicit Identifier(const std::string &dat = "") : name(dat) {}
			std::string to_string() const {
				return name.to_string();
			}
			DefaultType get_type() const {
				return T_Identifier;
			}
			Identifier* clone() const {
				return new Identifier(name.to_string());
			}

		private:
			Common::charptr name;
		};
	}

	/*class ObjectPtr
	{
	public:
		ObjectPtr(Objects::Object *p) : ptr(p) {}
		Objects::Object* get() { return ptr; }
		const Objects::Object* get() const { return ptr; }
		Objects::Object* operator->() { return ptr; }
		const Objects::Object* operator->() const { return ptr; }
		bool operator==(void *p) const { return ptr == p; }
		operator bool() const { return ptr != nullptr; }

		Objects::Object *ptr;
	};*/

	using ObjectPtr = autoptr<Objects::Object>;
	using DataList = std::vector<ObjectPtr>;

	namespace Objects
	{
		namespace Func
		{
			// Default Function
			template <typename T>
			ObjectPtr sum(const DataList &list) {
				T *result = new T;
				for (auto &l : list)
					result->add(*((T*)l.get()));
				return ObjectPtr(result);
			}
			template <typename T>
			ObjectPtr sub(const DataList &list);

			ObjectPtr sum(const DataList &list);
			ObjectPtr sub(const DataList &list);

			void print(const ObjectPtr &p);
		}

		std::string to_string(const Object &obj);
	}
}

#endif
