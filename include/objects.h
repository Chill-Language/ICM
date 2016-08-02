#ifndef _ICM_OBJECTS_H_
#define _ICM_OBJECTS_H_

#include "basic.h"

namespace ICM
{
	namespace Objects
	{
		//=======================================
		// * Declarations
		//=======================================
		class Object;
		class Error;
		class Nil;
		class Boolean;
		class Number;
		class String;
		class List;
		class Identifier;
	}

	using ObjectPtr = autoptr<Objects::Object>;
	using DataList = std::vector<ObjectPtr>;
	using FuncPtr = std::function<ObjectPtr(const DataList&)>;

	namespace Objects
	{
		//=======================================
		// * Class Object
		//=======================================
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
			// Method
			virtual Boolean* equ(const ObjectPtr &obj) const;

		protected:
			bool type_equal(const ObjectPtr &obj) const {
				return this->get_type() == obj->get_type();
			}
		};
	}

	namespace Objects
	{
		//=======================================
		// * Class Error
		//=======================================
		class Error : public Object
		{
		public:
			explicit Error(std::string msg = "") : msg(msg) {}
			string to_string() const {
				return "Error(" + msg + ")";
			}
			DefaultType get_type() const {
				return T_Error;
			}

		private:
			std::string msg;
		};

		//=======================================
		// * Class Nil
		//=======================================
		class Nil : public Object
		{
		public:
			Nil() {}

			Boolean* equ(const ObjectPtr &obj) const;
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

		//=======================================
		// * Class Boolean
		//=======================================
		class Boolean : public Object
		{
		public:
			Boolean() {}
			explicit Boolean(bool b) : data(b) {}

			Boolean* equ(const ObjectPtr &obj) const;
			string to_string() const {
				return Convert::to_string(data);
			}
			Boolean* clone() const {
				return new Boolean(*this);
			}

		private:
			bool data = false;
		};

		//=======================================
		// * Class Number
		//=======================================
		class Number : public Object
		{
		public:
			explicit Number(int dat = 0) : data(dat) {}

			Boolean* equ(const ObjectPtr &obj) const;
			Number* add(const Number *obj);
			Number* sub(const Number *obj);
			Number* mul(const Number *obj);
			Number* div(const Number *obj);
			Number* mod(const Number *obj);

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

		//=======================================
		// * Class String
		//=======================================
		class String : public Object
		{
		public:
			explicit String(const std::string &dat = "") : data(dat) {}

			Boolean* equ(const ObjectPtr &obj) const;
			String* add(const String *obj);
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

		//=======================================
		// * Class List
		//=======================================
		class List : public Object
		{
		public:
			explicit List(const DataList &dl) : data(dl) {}

			Boolean* equ(const ObjectPtr &obj) const;
			List* push(const ObjectPtr &objp);
			List* push(const DataList &dl);
			List* add(const List *dl);

			string to_string() const;
			DefaultType get_type() const {
				return T_List;
			}
			List* clone() const {
				return new List(*this);
			}

		private:
			DataList data;
		};

		//=======================================
		// * Class Identifier
		//=======================================
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
			ObjectPtr equ(const DataList &list);
			ObjectPtr list(const DataList &list);
			ObjectPtr let(const DataList &dl);
			ObjectPtr print(const DataList &list);
		}

		std::string to_string(const Object &obj);
	}
}

#endif
