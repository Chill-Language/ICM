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
				return T_Nil;
			}
			virtual Object* clone() const {
				return new Object(*this);
			}
		};

		class Nil : public Object
		{
		public:
			Nil() {}
			string to_string() const {
				return "Nil";
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

			Number& add(const Number &b) {
				self.data += b.data;
				return self;
			}
			Number& sub(const Number &b) {
				self.data -= b.data;
				return self;
			}
			Number& mul(const Number &b) {
				self.data *= b.data;
				return self;
			}
			Number& div(const Number &b) {
				self.data /= b.data;
				return self;
			}
			Number& mod(const Number &b) {
				self.data %= b.data;
				return self;
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

	using ObjectPtr = autoptr<Objects::Object>;
	using DataList = std::vector<ObjectPtr>;

	namespace Objects
	{
		namespace Func
		{
			// Default Function
			template <typename T>
			ObjectPtr add(const DataList &list) {
				T *result = new T;
				for (auto &l : list)
					result->add(*((T*)l.get()));
				return ObjectPtr(result);
			}
			template <typename T>
			ObjectPtr sub(const DataList &list);
			template <typename T>
			ObjectPtr mul(const DataList &list);
			template <typename T>
			ObjectPtr div(const DataList &list);
			template <typename T>
			ObjectPtr mod(const DataList &list);

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
