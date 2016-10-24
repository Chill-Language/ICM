#include "objectsdef.h"
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
		DEFTYPENAME(List);
		DEFTYPENAME(Disperse);
		// Literal Types
		DEFTYPENAME(Boolean);
		DEFTYPENAME(Identifier);
		DEFTYPENAME(Number);
		DEFTYPENAME(String);
		DEFTYPENAME(Symbol);
		// Identifier Types
		DEFTYPENAME(Function);
		DEFTYPENAME(Keyword);
		DEFTYPENAME(Reference);
		DEFTYPENAME(Type);
		DEFTYPENAME(Variable);
		// Compile Types (Platform Dependent)
		DEFTYPENAME(Int);
		DEFTYPENAME(UInt);
		DEFTYPENAME(Short);
		DEFTYPENAME(UShort);
		DEFTYPENAME(Long);
		DEFTYPENAME(ULong);
		DEFTYPENAME(LLong);
		DEFTYPENAME(ULLong);
		DEFTYPENAME(Float);
		DEFTYPENAME(Double);
		DEFTYPENAME(LDouble);
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
		return static_cast<TType<_TU>::Type*>(data);
	}

	template <TypeUnit _TU>
	string to_string(const void *data) {
		using std::to_string;
		using Common::Convert::to_string;
		using Objects::to_string;
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
		return Objects::to_output<typename TType<_TU>::Type>(*get<_TU>((void*)data));
	}
	template <TypeUnit _TU>
	string to_string_code(const void *data) {
		return Objects::to_string_code<typename TType<_TU>::Type>(*get<_TU>((void*)data));
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

	template <TypeUnit _TU>
	TypeInfo initTypeInfo() {
		return{
			_TU,
			TType<_TU>::Name,
			sizeof(typename TType<_TU>::Type),
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

	class NilType {};
	map<TypeUnit, TypeInfo> TypeInfoTable = {
		TypeInfoPair<T_Number>(),
		TypeInfoPair<T_Null>(),
		TypeInfoPair<T_String>(),
		TypeInfoPair<T_Boolean>(),
		TypeInfoPair<T_Function>(),
		TypeInfoPair<T_Identifier>(),
		TypeInfoPair<T_Error>(),
		TypeInfoPair<T_Nil>(),
		TypeInfoPair<T_List>(),
		TypeInfoPair<T_Disperse>(),
		//
		TypeInfoPair<T_Int>(),
		TypeInfoPair<T_UInt>(),
		TypeInfoPair<T_Short>(),
		TypeInfoPair<T_UShort>(),
		TypeInfoPair<T_Long>(),
		TypeInfoPair<T_ULong>(),
		TypeInfoPair<T_LLong>(),
		TypeInfoPair<T_ULLong>(),
		TypeInfoPair<T_Float>(),
		TypeInfoPair<T_Double>(),
		TypeInfoPair<T_LDouble>(),
		TypeInfoPair<T_CPointer>(),
		//
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

	namespace Objects
	{
		//=======================================
		// * Class List
		//=======================================
		ListType* ListType::push(const ObjectPtr &op) {
			data.push_back(op);
			return this;
		}
		ListType* ListType::push(const DataList &dl) {
			data.insert(data.end(), dl.begin(), dl.end());
			return this;
		}
		ListType& ListType::operator+=(const ListType &dl) {
			data.insert(data.end(), dl.data.begin(), dl.data.end());
			return *this;
		}
		string ListType::to_string() const {
			return ICM::to_string(data);
		}
		string ListType::to_output() const {
			return Common::Convert::to_string<'[', ']'>(data.begin(), data.end(), [](const ObjectPtr &op) { return op.to_output(); });
		}

		//=======================================
		// * Class Disperse
		//=======================================
		string DisperseType::to_string() const {
			return Convert::to_string(data.begin(), data.end(), [](const ObjectPtr &op) { return ICM::to_string(op); });
		}
		string DisperseType::to_output() const {
			string str;
			for (auto &op : data)
				str.append(op.to_output());
			return str;
		}

		//=======================================
		// * Class Identifier
		//=======================================

		void IdentifierType::setData(const ObjectPtr &op) {
			if (op->type == T_Identifier)
				data = op.get<Identifier>()->getData().getData();
			else
				data = op;
		}
		void IdentifierType::setCopy(const ObjectPtr &op) {
			if (op.isType<Identifier>())
				setCopy(op.get<Identifier>()->getData().getData());
			else
				data = ObjectPtr(op->clone());
		}
		void IdentifierType::setRefer(const ObjectPtr &op) {
			if (op->type == T_Identifier) {
				const ObjectPtr &sop = op.get<Identifier>()->getData().getData();
				const ObjectPtr &refop = sop.isType<Identifier>() ? sop : op;
				const ObjectPtr &refopdata = refop.get<Identifier>()->getData().getData();
				if (data.get() != refopdata.get())
					data = refop;
				else
					data = refopdata;
			}
			else
				data = op;
		}

		const ObjectPtr& IdentifierType::getRealData() const {
			if (data.isType(T_Identifier))
				return data.get<Objects::Identifier>()->getData().getData();
			else
				return data;
		}

		/*ObjectPtr GetElt(const ObjectPtr &op) {
			if (op.isType(T_Variable))
				return GetElt(op.get<Variable>()->getData());
			else if (op.isType(T_Reference))
				return GetElt(op.get<Reference>()->getData());
			else
				return op;
		}
		ObjectPtr GetRef(const ObjectPtr &op) {
			if (op.isType(T_Variable))
				return ObjectPtr(new Reference("R", op));
			if (op.isType(T_Reference))
				return op;
			else
				return ObjectPtr(new Reference("R", op));
		}*/
		template <> string to_string<ListType>(const ListType &lt) {
			return lt.to_string();
		}
		template <> string to_string<DisperseType>(const DisperseType &lt) {
			return lt.to_string();
		}
		template <> string to_string<ErrorType>(const ErrorType &et) {
			return et.to_string();
		}
		template <> string to_string<FunctionType>(const FunctionType &ft) {
			return ft.to_string();
		}
		template <> string to_string<IdentifierType>(const IdentifierType &ft) {
			return ft.to_string();
		}
		template <> string to_output<ListType>(const ListType &lt) {
			return lt.to_output();
		}
		template <> string to_output<DisperseType>(const DisperseType &lt) {
			return lt.to_output();
		}
		template <> string to_output<FunctionType>(const FunctionType &ft) {
			return ft.to_output();
		}
		template <> string to_output<IdentifierType>(const IdentifierType &it) {
			return it.to_output();
		}
		template <> string to_string_code<FunctionType>(const FunctionType &ft) {
			return ft.to_string_code();
		}
		template <> string to_string_code<IdentifierType>(const IdentifierType &it) {
			return it.to_string_code();
		}
	}

	//=======================================
	// * Functions
	//=======================================
	// Create Object
	Objects::Object* createObject(DefaultType type)
	{
		using namespace Objects;
		switch (type)
		{
		case ICM::T_Object:     return new Objects::Object;
		case ICM::T_Nil:        return new Nil;
		case ICM::T_Error:      return new Error;
		case ICM::T_Identifier: return new Identifier;
		case ICM::T_Keyword:    return new Keyword;
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
		return ObjectPtr(new Objects::Error(Objects::ErrorType(errinfo)));
	}
	// Adjust ObjectPtr
	const ObjectPtr& adjustObjectPtr(const ObjectPtr &op) {
		if (op.isType(T_Identifier))
			return adjustObjectPtr(op.get<Objects::Identifier>()->getData().getData());
		else
			return op;
	}
	// Get TypeObject
	TypeObject getTypeObject(const ObjectPtr &op)
	{
		if (op.isType(T_Function)) {
			TypeObject t(T_Function);
			auto &ft = op.get<Objects::Function>()->getData().get_data();
			t.setFuncTableUnit(&ft);
			return t;
		}
		else
			return ICM::TypeObject(op.type());
	}
	// Get Disperse Iterator
	vector<ObjectPtr>::iterator begin(Objects::Disperse *disp)
	{
		return disp->getData().begin();
	}
	vector<ObjectPtr>::iterator end(Objects::Disperse *disp)
	{
		return disp->getData().end();
	}
}
