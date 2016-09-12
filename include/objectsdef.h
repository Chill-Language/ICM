#ifndef _ICM_OBJECTSDEF_H_
#define _ICM_OBJECTSDEF_H_

#include "basic.h"
#include "objects.h"
#include "tostring.h"
#include "number.h"
#include "keyword.h"

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
			DefaultType getType() const {
				return Type;
			}
			operator bool() const { return data; }
			virtual void write(File &file) const {
				file.write(data);
			}
			virtual void read(File &file) {
				file.read(data);
			}
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

			NumType get_data() const {
				return data;
			}
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			Boolean* equ(const ObjectPtr &obj) const;
			string to_string() const {
				return Common::Number::to_string(data);
			}
			DefaultType getType() const {
				return Type;
			}
			Number* clone() const {
				return new Number(*this);
			}
			virtual void write(File &file) const {
				file.write(data);
			}
			virtual void read(File &file) {
				file.read(data);
			}
			// Const
			static const DefaultType Type = T_Number;

		private:
			NumType data;
			// TODO
			static NumType& get_data(Object *obj)
			{
				return ((Number*)obj)->data;
			}
			static const NumType& get_data(const Object *obj)
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
			DefaultType getType() const {
				return Type;
			}
			String* clone() const {
				return new String(*(this->data.clone()));
			}
			virtual void write(File &file) const {
				size_t length = data.length();
				file.write(length);
				file.write((const char*)data, length);
			}
			virtual void read(File &file) {
				size_t length;
				file.read(length);
				data = charptr(length);
				file.read((char*)data, length);
			}
			// Const
			static const DefaultType Type = T_String;

		private:
			Common::charptr data;
			explicit String(const Common::charptr &dat) : data(dat) {}
		};

		//=======================================
		// * Class Symbol
		//=======================================
		class Symbol : public Object
		{
		public:
			explicit Symbol(size_t id) : data(id) {}
			explicit Symbol(const std::string &dat = "") {
				this->data = GlobalSymbolTable.find(dat);
			}

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			Boolean* equ(const ObjectPtr &obj) const;
			std::string to_string() const {
				return '\'' + to_output() + '\'';
			}
			DefaultType getType() const {
				return Type;
			}
			Symbol* clone() const {
				return new Symbol(*this);
			}
			virtual void write(File &file) const {
				file.write(data);
			}
			virtual void read(File &file) {
				file.read(data);
			}
			// Const
			static const DefaultType Type = T_String;

		private:
			size_t data;
			std::string get_data() const {
				return GlobalSymbolTable.get(data);
			}
		};

		//=======================================
		// * Class List
		//=======================================
		class List : public Object
		{
		public:
			List() {}
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
			const DataList& getData() const {
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
			DefaultType getType() const {
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
			const DataList& getData() const {
				return data;
			}

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			Boolean* equ(const ObjectPtr &obj) const;
			string to_string() const;
			string to_output() const;
			DefaultType getType() const {
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
			const ObjectPtr& getData() const {
				return data;
			}
			const ObjectPtr& getRealData() const {
				if (data.isType(T_Identifier))
					return data.get<Objects::Identifier>()->getData();
				else
					return data;
			}
			void setData(const ObjectPtr &op) {
				if (op->getType() == T_Identifier)
					data = op.get<Identifier>()->getData();
				else
					data = op;
			}
			void setCopy(const ObjectPtr &op) {
				if (op.isType<Identifier>())
					setCopy(op.get<Identifier>()->getData());
				else
					data = ObjectPtr(op->clone());
			}
			void setRefer(const ObjectPtr &op) {
				if (op->getType() == T_Identifier) {
					const ObjectPtr &sop = op.get<Identifier>()->getData();
					const ObjectPtr &refop = sop.isType<Identifier>() ? sop : op;
					const ObjectPtr &refopdata = refop.get<Identifier>()->getData();
					if (data.get() != refopdata.get())
						data = refop;
					else
						data = refopdata;
				}
				else
					data = op;
			}
			DefaultType getValueType() const {
				return data.type();
			}
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			DefaultType getType() const {
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
			string to_string_code() const {
				return name.to_string();
			}
			virtual void write(File &file) const {
				file.write(data);
			}
			virtual void read(File &file) {
				file.read(data);
			}
			// Const
			static const DefaultType Type = T_Identifier;

		private:
			Common::charptr name;
			ObjectPtr data;
		};

		//=======================================
		// * Class Keyword
		//=======================================
		class Keyword : public Object
		{
		public:
			Keyword() {}
			explicit Keyword(const KeywordID &data)
				: data(data) {}
			KeywordID getData() const {
				return data;
			}
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			DefaultType getType() const {
				return Type;
			}
			Keyword* clone() const {
				return new Keyword(*this);
			}
			string to_string() const {
				return ICM::to_string(data);
			}
			virtual void write(File &file) const {
				file.write(data);
			}
			virtual void read(File &file) {
				file.read(data);
			}
			// Const
			static const DefaultType Type = T_Keyword;

		private:
			KeywordID data;
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
			DefaultType getType() const {
				return Type;
			}
			TypeClass* clone() const {
				return new TypeClass(*this);
			}
			string to_string() const {
				return data.to_string();
			}
			virtual void write(File &file) const {
				file.write(data);
			}
			virtual void read(File &file) {
				file.read(data);
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
			Function() {}
			Function(size_t id) : data(id) {}
			const FuncTableUnit& get_data() const {
				return DefFuncTable[data];
			}
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			DefaultType getType() const {
				return Type;
			}
			Function* clone() const {
				return new Function(*this);
			}
			string to_string() const {
				return "F(" + get_data().getName() + ")";
			}
			string to_string_code() const {
				return get_data().getName();
			}
			virtual void write(File &file) const {
				file.write(data);
			}
			virtual void read(File &file) {
				file.read(data);
			}
			// Const
			static const DefaultType Type = T_Function;

		private:
			size_t data;
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
			DefaultType getType() const {
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
			DefaultType getType() const {
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
