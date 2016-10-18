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

		template <typename T>
		string to_string(const T &t) {
			using Common::Convert::to_string;
			using std::to_string;
			using ICM::to_string;
			return to_string(t);
		}
		template <typename T>
		string to_output(const T &t) {
			return to_string<T>(t);
		}

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
			DataObject() : Object(_Type) { this->data = new T(); }
			DataObject(const T &dat) : Object(_Type) { this->data = new T(dat); }
			DataObject(const DataObject &dot) : Object(_Type) {
				this->data = new T(dot._ref());
			}

			T& getData() {
				return _ref();
			}
			const T& getData() const {
				return _ref();
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
				return Objects::to_string(_ref());
			}
			string toOutput() const {
				return Objects::to_output(_ref());
			}
			DataObject* clone() const {
				return new DataObject(*this);
			}
			void set(const Object *op) {
				_ref() = ((const DataObject*)op)->_ref();
			}
			bool equ(const Object* obj) const {
				return _ref() == static_cast<const DataObject*>(obj)->_ref();
			}

		private:
			const T* _ptr() const { return (const T*)this->data; }
			T* _ptr() { return (T*)this->data; }
			const T& _ref() const { return *_ptr(); }
			T& _ref() { return *_ptr(); }
		};

		//=======================================
		// * Class List
		//=======================================
		class ListType
		{
			using DataList = vector<ObjectPtr>;
		public:
			ListType() {}
			explicit ListType(const lightlist<ObjectPtr> &dl) : data(dl.begin(), dl.end()) {}
			explicit ListType(const DataList &dl) : data(dl) {}
			ListType(const ListType &lt) : data(lt.data) {}

			ListType* push(const ObjectPtr &objp);
			ListType* push(const DataList &dl);
			ListType& operator+=(const ListType &dl);
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
			// Method
			bool operator==(const ListType &lt) const {
				return this->data == lt.data;
			}
			string to_string() const;
			string to_output() const;

		private:
			DataList data;
		};
		string to_string(const ListType &lt);
		string to_output(const ListType &lt);

		//=======================================
		// * Class Disperse
		//=======================================
		class DisperseType
		{
			using DataList = vector<ObjectPtr>;
		public:
			explicit DisperseType(const lightlist<ObjectPtr> &dl) : data(dl.begin(), dl.end()) {}
			explicit DisperseType(const DataList &dl) : data(dl) {}

			DataList::iterator begin() {
				return data.begin();
			}
			DataList::iterator end() {
				return data.end();
			}
			const DataList& getData() const {
				return data;
			}
			// Method
			bool operator==(const DisperseType &lt) const {
				return this->data == lt.data;
			}
			string to_string() const;
			string to_output() const;

		private:
			DataList data;
		};
		string to_string(const DisperseType &lt);
		string to_output(const DisperseType &lt);

		//=======================================
		// * Class Error
		//=======================================
		class ErrorType
		{
		public:
			explicit ErrorType(std::string msg = "") : msg(msg) {}
			// Method
			string to_string() const {
				return "Error(" + msg + ")";
			}
			bool operator==(const ErrorType &et) const {
				return msg == et.msg;
			}

		private:
			std::string msg;
		};
		string to_string(const ErrorType &et);
		//=======================================
		// * Class Function
		//=======================================
		class FunctionType : public Object
		{
		public:
			FunctionType() {}
			FunctionType(size_t id) : data(id) {}
			const FuncTableUnit& get_data() const {
				return DefFuncTable[data];
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
			bool operator==(const FunctionType &ft) const {
				return data == ft.data;
			}
			// Const
			static const DefaultType Type = T_Function;

		private:
			size_t data;
		};
		string to_string(const FunctionType &ft);
		string to_output(const FunctionType &lt);
		//=======================================
		// * Class Identifier
		//=======================================
		class IdentifierType : public Object
		{
		public:
			explicit IdentifierType(const std::string &name = "") : name(name) {}
			IdentifierType(const std::string &name, const ObjectPtr &op) : name(name), data(op) {}
			std::string getName() const {
				return name.to_string();
			}
			const ObjectPtr& getData() const {
				return data;
			}
			const ObjectPtr& getRealData() const;
			void setData(const ObjectPtr &op);
			void setCopy(const ObjectPtr &op);
			void setRefer(const ObjectPtr &op);
			DefaultType getValueType() const {
				return data.type();
			}
			bool operator==(const IdentifierType &it) const {
				return data == it.data;
			}
			// Method
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

		private:
			Common::charptr name;
			ObjectPtr data;
		};
		string to_string(const IdentifierType &ft);
		string to_output(const IdentifierType &ft);

		class Nil;
		using Error = DataObject<ErrorType, T_Error>;
		using Boolean = DataObject<bool, T_Boolean>;
		using Number = DataObject<Common::Number::Rational, T_Number>;
		using String = DataObject<string, T_String>;

		using List = DataObject<ListType, T_List>;
		using Disperse = DataObject<DisperseType, T_Disperse>;
		using Keyword = DataObject<KeywordID, T_Keyword>;
		using Function = DataObject<FunctionType, T_Function>;
		class Identifier : public DataObject<IdentifierType, T_Identifier> {
		public:
			Identifier() : DataObject<IdentifierType, T_Identifier>() {}
			Identifier(const IdentifierType &it) : DataObject<IdentifierType, T_Identifier>(it) {}
		};
	}

	vector<ObjectPtr>::iterator begin(Objects::Disperse *disp);
	vector<ObjectPtr>::iterator end(Objects::Disperse *disp);

	namespace Objects
	{
		//=======================================
		// * Class Nil
		//=======================================
		class Nil : public Object
		{
		public:
			Nil() : Object(T_Nil) {}

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
			Nil* clone() const {
				return new Nil(*this);
			}
			// Const
			static const DefaultType Type = T_Nil;

		};
	}
}

#endif
