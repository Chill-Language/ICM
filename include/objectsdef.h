#ifndef _ICM_OBJECTSDEF_H_
#define _ICM_OBJECTSDEF_H_

#include "basic.h"
#include "ast.h"
#include "objects.h"
#include "tostring.h"

namespace ICM
{
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
			using Type = int;
			explicit Number(Type dat = 0) : data(dat) {}

			Boolean* equ(const ObjectPtr &obj) const;
			Number* add(const Number *obj);
			Number* sub(const Number *obj);
			Number* mul(const Number *obj);
			Number* div(const Number *obj);
			Number* mod(const Number *obj);

			bool operator<(const Number &obj);
			bool operator<=(const Number &obj);
			bool operator>(const Number &obj);
			bool operator>=(const Number &obj);

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
			Type data;
			Type& get_data(Object *obj)
			{
				return ((Number*)obj)->data;
			}
			const Type& get_data(const Object *obj) const
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
			std::string to_output() const {
				return data.to_string();
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
			DataList::iterator begin() {
				return data.begin();
			}
			DataList::iterator end() {
				return data.end();
			}

			string to_string() const;
			string to_output() const;
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
			explicit Identifier(const std::string &name = "") : name(name) {}
			Identifier(const std::string &name, ASTNode *node) : name(name) {
				setValue(node);
			}

			std::string getName() const {
				return name.to_string();
			}
			ASTNode* getRefNode();
			ASTNode* getDatNode();
			void setValue(ASTNode *node);
			DefaultType getValueType() const;
			std::string to_string() const;
			string to_output() const;

			DefaultType get_type() const {
				return T_Identifier;
			}
			Identifier* clone() const {
				return new Identifier(name.to_string());
			}

		private:
			Common::charptr name;
			ASTNode* data;
		};
	}
}

#endif
