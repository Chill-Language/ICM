#ifndef _ICM_OBJECTSDEF_H_
#define _ICM_OBJECTSDEF_H_

#include "basic.h"
#include "ast.h"
#include "objects.h"
#include "tostring.h"
#include "number.h"

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
				return Type;
			}
			// Const
			static const DefaultType Type = T_Error;

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
				return Type;
			}
			Nil* clone() const {
				return new Nil(*this);
			}
			// Const
			static const DefaultType Type = T_Nil;

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
			DefaultType get_type() const {
				return Type;
			}
			operator bool() const { return data; }
			// Const
			static const DefaultType Type = T_Boolean;

		private:
			bool data = false;
		};

		//=======================================
		// * Class Number
		//=======================================
		class Number : public Object
		{
		public:
			using NumType = Common::Number::Rational;
			explicit Number(NumType dat = 0) : data(dat) {}

			Boolean* equ(const ObjectPtr &obj) const;
			Number* add(const Number *obj);
			Number* sub(const Number *obj);
			Number* mul(const Number *obj);
			Number* div(const Number *obj);
			Number* mod(const Number *obj);
			Number* rem(const Number *obj);

			bool operator<(const Number &obj);
			bool operator<=(const Number &obj);
			bool operator>(const Number &obj);
			bool operator>=(const Number &obj);

			string to_string() const {
				return Common::Number::to_string(data);
			}
			DefaultType get_type() const {
				return Type;
			}
			Number* clone() const {
				return new Number(*this);
			}
			// Const
			static const DefaultType Type = T_Number;

		private:
			NumType data;
			NumType& get_data(Object *obj)
			{
				return ((Number*)obj)->data;
			}
			const NumType& get_data(const Object *obj) const
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
				return Type;
			}
			String* clone() const {
				return new String(*(this->data.clone()));
			}
			// Const
			static const DefaultType Type = T_String;

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
			const DataList& get_data() const {
				return data;
			}

			string to_string() const;
			string to_output() const;
			DefaultType get_type() const {
				return Type;
			}
			List* clone() const {
				return new List(*this);
			}
			// Const
			static const DefaultType Type = T_List;

		private:
			DataList data;
		};

		//=======================================
		// * Class Disperse
		//=======================================
		class Disperse : public Object
		{
		public:
			explicit Disperse(const DataList &dl) : data(dl) {}

			Boolean* equ(const ObjectPtr &obj) const;
			DataList::iterator begin() {
				return data.begin();
			}
			DataList::iterator end() {
				return data.end();
			}
			const DataList& get_data() const {
				return data;
			}

			string to_string() const;
			string to_output() const;
			DefaultType get_type() const {
				return Type;
			}
			Disperse* clone() const {
				return new Disperse(*this);
			}
			// Const
			static const DefaultType Type = T_Disperse;

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
				return Type;
			}
			Identifier* clone() const {
				return new Identifier(name.to_string());
			}
			// Const
			static const DefaultType Type = T_Identifier;

		private:
			Common::charptr name;
			ASTNode* data;
		};
	}
}

#endif
