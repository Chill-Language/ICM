#ifndef _ICM_OBJECTSDEF_H_
#define _ICM_OBJECTSDEF_H_

#include "basic.h"
#include "objects.h"
#include "number.h"
#include "keyword.h"
#include "tabledata.h"

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

		//=======================================
		// * Class DataObject<T, _Type>
		//=======================================
		template <typename T, DefaultType _Type>
		class DataObject : public Object
		{
		public:
			using VType = T;
			static const DefaultType Type = _Type;
		public:
			DataObject() {}
			DataObject(const T &dat)
				: data(dat) {}

			T& getData() {
				return data;
			}
			const T& getData() const {
				return data;
			}
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			string to_string() const { // Delete
				return toString();
			}
			string to_output() const { // Delete
				return toOutput();
			}
			bool equ(const ObjectPtr &obj) const { // Delete
				return equ(obj.get());
			}
			string toString() const {
				return data.toString();
			}
			string toOutput() const {
				return data.toOutput();
			}
			DefaultType getType() const {
				return _Type;
			}
			DataObject* clone() const {
				return new DataObject(*this);
			}
			void set(const Object *op) {
				this->data = ((const DataObject*)op)->data;
			}
			bool equ(const Object* obj) const {
				return this->data == static_cast<const DataObject*>(obj)->data;
			}
			void write(File &file) const {
				data.write(file);
			}
			void read(File &file) {
				data.read(file);
			}

		private:
			T data;
		};

		template <typename _DTy>
		class ObjectStruct
		{
		public:
			ObjectStruct() {}
			ObjectStruct(const _DTy &dat) : data(dat) {}
			operator _DTy() const { return data; }
			const _DTy& getData() const { return data; }
			_DTy& getData() { return data; }
			string toString() const {
				using Common::Convert::to_string;
				return to_string(data);
			}
			bool operator==(const ObjectStruct &os) const {
				return data == os.data;
			}
			ObjectStruct& operator=(const ObjectStruct &os) {
				data = os.data;
				return *this;
			}
			string toOutput() const { return toString(); }
			void write(File &file) const { file.write(data); }
			void read(File &file) { file.read(data); }

		private:
			_DTy data;
		};

		using Boolean = DataObject<ObjectStruct<bool>, T_Boolean>;
		using Number = DataObject<ObjectStruct<Common::Number::Rational>, T_Number>;

		class String;
		class Symbol;
		class List;
		class Disperse;
		class Identifier;
		class Keyword;
		class Function;
		class TypeClass;
	}

	vector<ObjectPtr>::iterator begin(Objects::Disperse *disp);
	vector<ObjectPtr>::iterator end(Objects::Disperse *disp);

	namespace Objects
	{
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
			bool equ(const ObjectPtr &obj) const {
				return this->to_string() == obj.get<String>()->to_string();
			}
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
			void set(const Object *op) {
				this->data = ((const String*)op)->data;
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
			bool equ(const ObjectPtr &obj) const {
				return this->data == obj.get<Symbol>()->data;
			}
			std::string to_string() const {
				return '\'' + to_output() + '\'';
			}
			DefaultType getType() const {
				return Type;
			}
			Symbol* clone() const {
				return new Symbol(*this);
			}
			void set(const Object *op) {
				this->data = ((const Symbol*)op)->data;
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
			using DataList = vector<ObjectPtr>;
		public:
			List() {}
			explicit List(const lightlist<ObjectPtr> &dl) : data(dl.begin(), dl.end()) {}
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
			bool equ(const ObjectPtr &obj) const {
				return this->data == obj.get<List>()->data;
			}
			string to_string() const;
			string to_output() const;
			DefaultType getType() const {
				return Type;
			}
			List* clone() const {
				return new List(*this);
			}
			void set(const Object *op) {
				this->data = ((const List*)op)->data;
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
			using DataList = vector<ObjectPtr>;
		public:
			explicit Disperse(const lightlist<ObjectPtr> &dl) : data(dl.begin(), dl.end()) {}
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
			bool equ(const ObjectPtr &obj) const {
				return this == obj.get<Disperse>();
			}
			string to_string() const;
			string to_output() const;
			DefaultType getType() const {
				return Type;
			}
			Disperse* clone() const {
				return new Disperse(*this);
			}
			void set(const Object *op) {
				this->data = ((const Disperse*)op)->data;
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
			virtual void setData(const ObjectPtr &op) {
				if (op->getType() == T_Identifier)
					data = op.get<Identifier>()->getData();
				else
					data = op;
			}
			virtual void setCopy(const ObjectPtr &op) {
				if (op.isType<Identifier>())
					setCopy(op.get<Identifier>()->getData());
				else
					data = ObjectPtr(op->clone());
			}
			virtual void setRefer(const ObjectPtr &op) {
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
			virtual DefaultType getType() const {
				return Type;
			}
			Identifier* clone() const {
				return new Identifier(name.to_string(), data);
			}
			void set(const Object *op) {
				this->data = ((const Identifier*)op)->data;
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
			void write(File &file) const {
				file.write(data);
			}
			void read(File &file) {
				file.read(data);
			}
			// Const
			static const DefaultType Type = T_Identifier;

		private:
			Common::charptr name;
			ObjectPtr data;
		};

		ObjectPtr GetElt(const ObjectPtr &op);
		ObjectPtr GetRef(const ObjectPtr &op);
		
		//=======================================
		// * Class Variable
		//=======================================
		class Variable : public Identifier
		{
		public:
			explicit Variable(const std::string &name = "") : Identifier(name) {}
			Variable(const std::string &name, const ObjectPtr &op) : Identifier(name, op) {}

			void setData(const ObjectPtr &op) {

			}

		private:
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			DefaultType getType() const {
				return Type;
			}
			// Const
			static const DefaultType Type = T_Variable;
		};
		
		//=======================================
		// * Class Reference
		//=======================================
		class Reference : public Identifier
		{
		public:
			explicit Reference(const std::string &name = "") : Identifier(name) {}
			Reference(const std::string &name, const ObjectPtr &op) : Identifier(name, op) {}

			void setRefer(const ObjectPtr &op) {

			}

		private:
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			DefaultType getType() const {
				return Type;
			}
			// Const
			static const DefaultType Type = T_Reference;
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
			bool equ(const ObjectPtr &obj) const {
				return true;
			}
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
