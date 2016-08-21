#ifndef _ICM_OBJECTSDEF_H_
#define _ICM_OBJECTSDEF_H_

#include "basic.h"
#include "objects.h"
#include "tostring.h"
#include "number.h"

namespace ICM
{
	namespace Objects
	{
		//=======================================
		// * Class Boolean
		//=======================================
		class Boolean : public Object
		{
		public:
			Boolean() {}
			explicit Boolean(bool b) : data(b) {}

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
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

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			Boolean* equ(const ObjectPtr &obj) const;
			string to_string() const {
				return Common::Number::to_string(data);
			}
			DefaultType get_type() const {
				return Type;
			}
			Number* clone() const {
				return new Number(*this);
			}
			NumType get_data() const {
				return data;
			}
			// Const
			static const DefaultType Type = T_Number;

		private:
			NumType data;
			// TODO
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
			String* add(const String *obj);
			std::string to_output() const {
				return data.to_string();
			}

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			Boolean* equ(const ObjectPtr &obj) const;
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
			size_t size() const {
				return data.size();
			}

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			Boolean* equ(const ObjectPtr &obj) const;
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

			DataList::iterator begin() {
				return data.begin();
			}
			DataList::iterator end() {
				return data.end();
			}
			const DataList& get_data() const {
				return data;
			}

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			Boolean* equ(const ObjectPtr &obj) const;
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
			Identifier(const std::string &name, const ObjectPtr &op) : name(name), data(op) {}
			std::string getName() const {
				return name.to_string();
			}
			ObjectPtr getData() const {
				return data;
			}
			void setData(const ObjectPtr &op) {
				if (op->get_type() == T_Identifier)
					data = getPointer<Identifier>(op)->getData();
				else
					data = op;
			}
			void setCopy(const ObjectPtr &op) {
				if (op->get_type() == T_Identifier)
					setCopy(getPointer<Identifier>(op)->getData());
				else
					data = ObjectPtr(op->clone());
			}
			void setRefer(const ObjectPtr &op) {
				if (op->get_type() == T_Identifier) {
					ObjectPtr sop = getPointer<Identifier>(op)->getData();
					ObjectPtr refop = (sop->get_type() == T_Identifier) ? sop : op;
					ObjectPtr refopdata = getPointer<Identifier>(refop)->getData();
					if (data.get() != refopdata.get())
						data = refop;
					else
						data = refopdata;
				}
				else
					data = op;
			}
			DefaultType getValueType() const {
				return data->get_type();
			}
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			DefaultType get_type() const {
				return Type;
			}
			Identifier* clone() const {
				return new Identifier(name.to_string(), data);
			}
			string to_string() const {
				return name.to_string() + "(" + ICM::to_string(data) + ")";
			}
			string to_output() const {
				return data->to_output();
			}
			// Const
			static const DefaultType Type = T_Identifier;

		private:
			Common::charptr name;
			ObjectPtr data;
		};
		
		//=======================================
		// * Class TypeClass
		//=======================================
		class TypeClass : public Object
		{
		public:
			explicit TypeClass(const TypeObject &data)
				: data(data) {}
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			DefaultType get_type() const {
				return Type;
			}
			TypeClass* clone() const {
				return new TypeClass(*this);
			}
			string to_string() const {
				return data.to_string();
			}
			// Const
			static const DefaultType Type = T_Type;

		private:
			TypeObject data;
		};

		//=======================================
		// * Class Function
		//=======================================
		class Function : public Object
		{
		public:
			Function(const FuncTableUnit *data) : data(data) {}
			const FuncTableUnit& get_data() const {
				return *data;
			}
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			DefaultType get_type() const {
				return Type;
			}
			Function* clone() const {
				return new Function(*this);
			}
			string to_string() const {
				return "F(" + data->getName() + ")";
			}
			// Const
			static const DefaultType Type = T_Function;

		private:
			shared_ptr<const FuncTableUnit> data;
		};

		//=======================================
		// * Class Error
		//=======================================
		class Error : public Object
		{
		public:
			explicit Error(std::string msg = "") : msg(msg) {}

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			string to_string() const {
				return "Error(" + msg + ")";
			}
			DefaultType get_type() const {
				return Type;
			}
			Object* clone() const {
				return new Error(*this);
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

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
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
	}
}

#endif
