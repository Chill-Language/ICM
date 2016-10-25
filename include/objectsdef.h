#ifndef _ICM_OBJECTSDEF_H_
#define _ICM_OBJECTSDEF_H_

#include "basic.h"
#include "objects.h"
#include "typebase.h"

namespace ICM
{

	namespace Types
	{
#define DEFTYPE(_TE, _T) template <> struct TType<T_##_TE> { using Type = _T;  static TypeName Name; }; using _TE = TType<T_##_TE>::Type;

		DEFTYPE(Null, TypeBase::VoidType);
		DEFTYPE(Object, TypeBase::VoidType);
		DEFTYPE(Nil, TypeBase::VoidType);
		DEFTYPE(Vary, TypeBase::VoidType);
		DEFTYPE(Void, TypeBase::VoidType);
		DEFTYPE(Error, TypeBase::ErrorType);
		// Literal Types
		DEFTYPE(Boolean, TypeBase::BooleanType);
		DEFTYPE(Identifier, TypeBase::IdentifierType);
		DEFTYPE(Number, TypeBase::NumberType);
		DEFTYPE(String, TypeBase::StringType);
		DEFTYPE(Symbol, TypeBase::SymbolType);
		// Identifier Types
		DEFTYPE(Function, TypeBase::FunctionType);
		DEFTYPE(Keyword, TypeBase::KeywordType);
		DEFTYPE(Reference, TypeBase::IdentifierType);
		DEFTYPE(Type, TypeBase::IdentifierType);
		DEFTYPE(Variable, TypeBase::IdentifierType);
		// List Types
		DEFTYPE(List, TypeBase::ListType);
		DEFTYPE(Disperse, TypeBase::DisperseType);
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
		
		class Nil : public Object {
		public:
			Nil() : Object(T_Nil) {}
			static const DefaultType Type = T_Nil;

		};
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

		vector<ObjectPtr>::iterator begin(Objects::Disperse *disp);
		vector<ObjectPtr>::iterator end(Objects::Disperse *disp);
	}
}

#endif
