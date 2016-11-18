#ifndef _ICM_OBJECTSDEF_H_
#define _ICM_OBJECTSDEF_H_

#include "basic.h"
#include "object.h"
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
		// TODO : Delete this class.
		//=======================================
		// * Class DataObject<_Type>
		//=======================================
		template <DefaultType _Type>
		class DataObject : public Object
		{
			using T = typename TType<_Type>::Type;
		public:
			DataObject() : Object(_Type) { this->data = new T(); }
			DataObject(const T &dat) : Object(_Type) { this->data = new T(dat); }
		};
		
		using Nil = DataObject<T_Nil>;
		using Error = DataObject<T_Error>;
		using Boolean = DataObject<T_Boolean>;
		using Number = DataObject<T_Number>;
		using String = DataObject<T_String>;

		using List = DataObject<T_List>;
		using Disperse = DataObject<T_Disperse>;
		using Keyword = DataObject<T_Keyword>;
		using Function = DataObject<T_Function>;
		class Identifier : public DataObject<T_Identifier> {
		public:
			Identifier() : DataObject<T_Identifier>() {}
			Identifier(const TypeBase::IdentifierType &it) : DataObject<T_Identifier>(it) {}
		};

		vector<Object*>::iterator begin(Disperse *disp);
		vector<Object*>::iterator end(Disperse *disp);
	}
}

#endif
