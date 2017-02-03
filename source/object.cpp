#include "basic.h"
#include "objectdef.h"
#include "keyword.h"

namespace ICM
{
	//===========================================================
	// * Namespace Types
	//===========================================================
	namespace Types
	{
#define DEFTYPENAME(_TE) TypeName TType<T_##_TE>::Name = #_TE;
		//
		DEFTYPENAME(Null);
		DEFTYPENAME(Object);
		DEFTYPENAME(Nil);
		DEFTYPENAME(Vary);
		DEFTYPENAME(Void);
		DEFTYPENAME(Error);
		// Literal Types
		DEFTYPENAME(Boolean);
		DEFTYPENAME(Number);
		DEFTYPENAME(String);
		DEFTYPENAME(Symbol);
		// Identifier Types
		DEFTYPENAME(Function);
		DEFTYPENAME(Keyword);
		DEFTYPENAME(Type);
		// List Types
		DEFTYPENAME(List);
		DEFTYPENAME(Disperse);
		// Compile Types (Platform Dependent)
		DEFTYPENAME(Int);
		DEFTYPENAME(UInt);
		DEFTYPENAME(Float);
		DEFTYPENAME(Double);
		DEFTYPENAME(CPointer);
		// Compile Types (Platform Independent)
		DEFTYPENAME(Byte);
		DEFTYPENAME(Word);
		DEFTYPENAME(DWord);
		DEFTYPENAME(QWord);
		DEFTYPENAME(Int8);
		DEFTYPENAME(Int16);
		DEFTYPENAME(Int32);
		DEFTYPENAME(Int64);
		DEFTYPENAME(UInt8);
		DEFTYPENAME(UInt16);
		DEFTYPENAME(UInt32);
		DEFTYPENAME(UInt64);
		// Test
		DEFTYPENAME(Test);
#undef DEFTYPENAME
	}

	template <TypeUnit _TU>
	typename TType<_TU>::Type* get(void *data) {
		return static_cast<typename TType<_TU>::Type*>(data);
	}

	template <TypeUnit _TU>
	string to_string(const void *data) {
		using std::to_string;
		using Common::Convert::to_string;
		using TypeBase::to_string;
		return to_string(*get<_TU>((void*)data));
	}
	template <>
	string to_string<T_Test>(const void *data) {
		return "Test";
	}
	template <>
	string to_string<T_Nil>(const void *data) {
		return "Nil";
	}
	template <>
	string to_string<T_Null>(const void *data) {
		return "Null";
	}
	template <TypeUnit _TU>
	string to_output(const void *data) {
		return TypeBase::to_output<typename TType<_TU>::Type>(*get<_TU>((void*)data));
	}
	template <TypeUnit _TU>
	string to_string_code(const void *data) {
		return TypeBase::to_string_code<typename TType<_TU>::Type>(*get<_TU>((void*)data));
	}
	template <>
	string to_output<T_Test>(const void *data) {
		return "Test";
	}
	template <>
	string to_output<T_Nil>(const void *data) {
		return "Nil";
	}
	template <>
	string to_output<T_Null>(const void *data) {
		return "Null";
	}
	template <>
	string to_string_code<T_Test>(const void *data) {
		return "Test";
	}
	template <>
	string to_string_code<T_Nil>(const void *data) {
		return "Nil";
	}
	template <>
	string to_string_code<T_Null>(const void *data) {
		return "Null";
	}
	template <TypeUnit _TU>
	bool equal(const void *dat1, const void *dat2) {
		using Type = typename TType<_TU>::Type;
		return *(Type*)dat1 == *(Type*)dat2;
	}

	//=======================================
	//=======================================
#define TEMPFUNC template <TypeUnit _TU> inline
	// construct/destruct
	TEMPFUNC void construct(void *data) {
		new (data) typename TType<_TU>::Type();
	}
	TEMPFUNC void destruct(void *data) {
		using T = typename TType<_TU>::Type;
		get<_TU>(data)->~T();
	}
	TEMPFUNC void ncopy(void *dst, const void *src) {
		using T = typename TType<_TU>::Type;
		new (dst) T(*static_cast<const T*>(src));
	}

	//=======================================
	//=======================================

	template <TypeUnit _TU>
	TypeInfo initTypeInfo() {
		return{
			_TU,
			TType<_TU>::Name,
			sizeof(typename TType<_TU>::Type),
			construct<_TU>,
			destruct<_TU>,
			ncopy<_TU>,
			equal<_TU>,
			to_string<_TU>,
			to_output<_TU>,
			to_string_code<_TU>,
		};
	}

	template <TypeUnit _TU>
	std::pair<TypeUnit, TypeInfo> TypeInfoPair() {
		return{ _TU, initTypeInfo<_TU>() };
	}

	map<TypeUnit, TypeInfo> TypeInfoTable = {
		TypeInfoPair<T_Null>(),
		TypeInfoPair<T_Nil>(),
		TypeInfoPair<T_Error>(),
		// Literal Types
		TypeInfoPair<T_Boolean>(),
		TypeInfoPair<T_Number>(),
		TypeInfoPair<T_String>(),
		TypeInfoPair<T_Symbol>(),
		// Identifier Types
		TypeInfoPair<T_Function>(),
		TypeInfoPair<T_Keyword>(),
		TypeInfoPair<T_Type>(),
		// List Types
		//TypeInfoPair<T_Array>(),
		TypeInfoPair<T_List>(),
		TypeInfoPair<T_Disperse>(),
		// Compile Types (Platform Dependent)
		TypeInfoPair<T_Int>(),
		TypeInfoPair<T_UInt>(),
		TypeInfoPair<T_Float>(),
		TypeInfoPair<T_Double>(),
		TypeInfoPair<T_CPointer>(),
		// Compile Types (Platform Independent)
		TypeInfoPair<T_Byte>(),
		TypeInfoPair<T_Word>(),
		TypeInfoPair<T_DWord>(),
		TypeInfoPair<T_QWord>(),
		TypeInfoPair<T_Int8>(),
		TypeInfoPair<T_Int16>(),
		TypeInfoPair<T_Int32>(),
		TypeInfoPair<T_Int64>(),
		TypeInfoPair<T_UInt8>(),
		TypeInfoPair<T_UInt16>(),
		TypeInfoPair<T_UInt32>(),
		TypeInfoPair<T_UInt64>(),
		TypeInfoPair<T_Test>(),
	};

	//=======================================
	// * Functions
	//=======================================
	// Create Object
	Object* createObject(DefaultType type)
	{
		using namespace Objects;
		switch (type)
		{
		case ICM::T_Object:     return new Object;
		case ICM::T_Nil:        return new Nil;
		case ICM::T_Error:      return new Error;
		case ICM::T_Keyword:    return new Objects::Keyword;
		case ICM::T_List:       return new List;
		case ICM::T_Disperse:   return nullptr; // TODO
		case ICM::T_Number:     return new Objects::Number;
		case ICM::T_String:     return new String;
		case ICM::T_Char:       return nullptr; // TODO
		case ICM::T_Function:   return new Objects::Function;
		case ICM::T_Type:       return nullptr; // TODO
		case ICM::T_Boolean:    return new Boolean;
		//case ICM::T_Symbol:     return new Symbol;
		default:
			println("Unfind Type.");
			return nullptr;
		}
	}
	// Create Error
	ObjectPtr createError(const string &errinfo) {
		return ObjectPtr(new Objects::Error(TypeBase::ErrorType(errinfo)));
	}
	namespace Objects
	{
		// Get Disperse Iterator
		vector<Object*>::iterator begin(Objects::Disperse *disp)
		{
			return disp->get<T_Disperse>()->begin();
		}
		vector<Object*>::iterator end(Objects::Disperse *disp)
		{
			return disp->get<T_Disperse>()->end();
		}
	}

	bool operator==(TypeUnit t1, DefaultType t2)
	{
		return t1 == (TypeUnit)t2;
	}

	StaticObject Static;
}
