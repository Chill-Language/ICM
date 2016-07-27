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
			virtual ~Object() {}
			virtual string to_string() const {
				return "Object";
			}
			virtual DefaultType get_type() const {
				return T_Object;
			}
			virtual Object* clone() const {
				return new Object(*this);
			}
			virtual Object* add(const Object *obj);
		};

		class Error : public Object
		{
		public:
			Error(std::string msg = "") : msg(msg) {}
			string to_string() const {
				return "Error(" + msg + ")";
			}
			DefaultType get_type() const {
				return T_Error;
			}

		private:
			std::string msg;
		};

		class Nil : public Object
		{
		public:
			Nil() {}
			string to_string() const {
				return "Nil";
			}
			DefaultType get_type() const {
				return T_Nil;
			}
			Nil* clone() const {
				return new Nil(*this);
			}

		};
		class Boolean : public Object
		{
		public:
			Boolean() {}
			Boolean(bool b) : data(b) {}
			string to_string() const {
				return Convert::to_string(data);
			}
			Boolean* clone() const {
				return new Boolean(*this);
			}

		private:
			bool data = false;
		};

		class Number : public Object
		{
		public:
			explicit Number(int dat = 0) : data(dat) {}

			Object* add(const Object *obj) {
				((Number*)this)->data += ((Number*)obj)->data;
				return this;
			}
			Object* sub(const Object *obj) {
				((Number*)this)->data -= ((Number*)obj)->data;
				return this;
			}
			Object* mul(const Object *obj) {
				((Number*)this)->data *= ((Number*)obj)->data;
				return this;
			}
			Object* div(const Object *obj) {
				((Number*)this)->data /= ((Number*)obj)->data;
				return this;
			}
			Object* mod(const Object *obj) {
				((Number*)this)->data %= ((Number*)obj)->data;
				return this;
			}

			string to_string() const {
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
			int& get_data(Object *obj)
			{
				return ((Number*)obj)->data;
			}
			const int& get_data(const Object *obj) const
			{
				return ((Number*)obj)->data;
			}
		};

		class String : public Object
		{
		public:
			explicit String(const std::string &dat = "") : data(dat) {}

			Object* add(const Object *obj) {
				data = Common::charptr(self.data.to_string() + ((String*)obj)->data.to_string());
				return this;
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

	using ObjectPtr = autoptr<Objects::Object>;
	using DataList = std::vector<ObjectPtr>;
	using FuncPtr = std::function<ObjectPtr(const DataList&)>;

	namespace Objects
	{
		ObjectPtr createObject(DefaultType type);

		namespace Func
		{
			ObjectPtr add(const DataList &list);
			ObjectPtr sub(const DataList &list);
			ObjectPtr mul(const DataList &list);
			ObjectPtr div(const DataList &list);
			ObjectPtr mod(const DataList &list);

			void print(const ObjectPtr &p);
		}

		std::string to_string(const Object &obj);
	}
}

#endif
