#ifndef _ICM_OBJECTSDEF_H_
#define _ICM_OBJECTSDEF_H_

#include "basic.h"
#include "objects.h"
#include "number.h"
#include "keyword.h"
#include "tabledata.h"

namespace ICM
{
	namespace TypeBase
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
		template <typename T>
		string to_string_code(const T &t) {
			return to_string<T>(t);
		}

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
		template <> string to_string<ListType>(const ListType &lt);
		template <> string to_output<ListType>(const ListType &lt);

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
		template <> string to_string<DisperseType>(const DisperseType &dt);
		template <> string to_output<DisperseType>(const DisperseType &dt);

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
		template <> string to_string<ErrorType>(const ErrorType &et);

		//=======================================
		// * Class Function
		//=======================================
		class FunctionType
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
			string to_output() const {
				return "F(" + get_data().getName() + ")";
			}
			string to_string_code() const {
				return get_data().getName();
			}
			void write(File &file) const {
				file.write(data);
			}
			void read(File &file) {
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
		template <> string to_string<FunctionType>(const FunctionType &ft);
		template <> string to_output<FunctionType>(const FunctionType &ft);
		template <> string to_string_code<FunctionType>(const FunctionType &ft);

		//=======================================
		// * Class Identifier
		//=======================================
		class IdentifierType
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
		template <> string to_string<IdentifierType>(const IdentifierType &it);
		template <> string to_output<IdentifierType>(const IdentifierType &it);
		template <> string to_string_code<IdentifierType>(const IdentifierType &it);
	}

	namespace Objects
	{
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
			DataObject* clone() const {
				return new DataObject(*this);
			}

		private:
			const T* _ptr() const { return Object::_ptr<T>(); }
			T* _ptr() { return Object::_ptr<T>(); }
			const T& _ref() const { return Object::_ref<T>(); }
			T& _ref() { return Object::_ref<T>(); }
		};

		class Nil;
		using Error = DataObject<TypeBase::ErrorType, T_Error>;
		using Boolean = DataObject<bool, T_Boolean>;
		using Number = DataObject<Common::Number::Rational, T_Number>;
		using String = DataObject<string, T_String>;

		using List = DataObject<TypeBase::ListType, T_List>;
		using Disperse = DataObject<TypeBase::DisperseType, T_Disperse>;
		using Keyword = DataObject<KeywordID, T_Keyword>;
		using Function = DataObject<TypeBase::FunctionType, T_Function>;
		class Identifier : public DataObject<TypeBase::IdentifierType, T_Identifier> {
		public:
			Identifier() : DataObject<TypeBase::IdentifierType, T_Identifier>() {}
			Identifier(const TypeBase::IdentifierType &it) : DataObject<TypeBase::IdentifierType, T_Identifier>(it) {}
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

			// Const
			static const DefaultType Type = T_Nil;

		};
	}

	namespace Types
	{
		class VoidType {
		public:
			bool operator==(const VoidType&) const { return true; }
		};
	}

	namespace Types
	{
#define DEFTYPE(_TE, _T) template <> struct TType<T_##_TE> { using Type = _T;  static TypeName Name; }; using _TE = TType<T_##_TE>::Type;

		DEFTYPE(Null, VoidType);
		DEFTYPE(Object, VoidType);
		DEFTYPE(Nil, VoidType);
		DEFTYPE(Vary, VoidType);
		DEFTYPE(Void, VoidType);
		DEFTYPE(Error, TypeBase::ErrorType);
		DEFTYPE(List, TypeBase::ListType);
		DEFTYPE(Disperse, TypeBase::DisperseType);
		// Literal Types
		DEFTYPE(Boolean, bool);
		DEFTYPE(Identifier, TypeBase::IdentifierType);
		DEFTYPE(Number, Common::Number::Rational);
		DEFTYPE(String, string);
		DEFTYPE(Symbol, string);
		// Identifier Types
		DEFTYPE(Function, TypeBase::FunctionType);
		DEFTYPE(Keyword, KeywordID);
		DEFTYPE(Reference, TypeBase::IdentifierType);
		DEFTYPE(Type, TypeBase::IdentifierType);
		DEFTYPE(Variable, TypeBase::IdentifierType);
		// Compile Types (Platform Dependent)
		DEFTYPE(Int, int);
		DEFTYPE(UInt, unsigned int);
		DEFTYPE(Short, short);
		DEFTYPE(UShort, unsigned short);
		DEFTYPE(Long, long);
		DEFTYPE(ULong, unsigned long);
		DEFTYPE(LLong, long long);
		DEFTYPE(ULLong, unsigned long long);
		DEFTYPE(Float, float);
		DEFTYPE(Double, double);
		DEFTYPE(LDouble, long double);
		DEFTYPE(CPointer, void*);
		// Compile Types (Platform Independent)
		DEFTYPE(Byte, byte);
		DEFTYPE(Word, word);
		DEFTYPE(DWord, dword);
		DEFTYPE(QWord, qword);
		DEFTYPE(Int8, int8_t);
		DEFTYPE(Int16, int16_t);
		DEFTYPE(Int32, int32_t);
		DEFTYPE(Int64, int64_t);
		DEFTYPE(UInt8, uint8_t);
		DEFTYPE(UInt16, uint16_t);
		DEFTYPE(UInt32, uint32_t);
		DEFTYPE(UInt64, uint64_t);
		// Test
		struct Test
		{
			Test() { println("Init Test"); }
			~Test() { println("Destory Test"); }
			Test(const Test &) { println("Copy Test"); }
			bool operator==(const Test&) const { return true; }
		};
		DEFTYPE(Test, Test);

#undef DEFTYPE
	}
}

#endif
