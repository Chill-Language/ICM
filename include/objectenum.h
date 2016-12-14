#pragma once

namespace ICM
{
	//===========================================================
	// * Type TypeUnit
	//===========================================================
	enum DefaultType {
		T_Null,          // Null
		T_Object,        // Obj
		T_Nil,           // Nil
		T_Vary,          // Var
		T_Void,          // Void
		T_Error,         // Err
		// Literal Types
		T_Boolean,       // Bool
		T_Identifier,    // Ident
		T_Number,        // Num
		T_String,        // Str
		T_Symbol,        // Sym
		// Identifier Types
		T_Function,      // Func
		T_Keyword,       // Key
		T_Reference,     // Ref
		T_Type,          // Type
		T_Variable,      // Varb
		// List Types
		T_Array,         // Array
		T_List,          // List
		T_Disperse,      // Disp
		// String Types
		T_Char,          // Char
		// Compile Types (Platform Dependent)
		T_Int,           // Int      : int_t
		T_UInt,          // UInt     : uint_t
		T_Float,         // Float    : float
		T_Double,        // Double   : double
		T_CPointer,      // CPointer : void*
		// Compile Types (Platform Independent)
		T_Byte,          // Byte   :  1 byte
		T_Word,          // Word   :  2 byte
		T_DWord,         // DWord  :  4 byte
		T_QWord,         // QWord  :  8 byte
		T_Int8,          // Int8   :  8 bit signed
		T_Int16,         // Int16  : 16 bit signed
		T_Int32,         // Int32  : 32 bit signed
		T_Int64,         // Int64  : 64 bit signed
		T_UInt8,         // UInt8  :  8 bit unsigned
		T_UInt16,        // UInt16 : 16 bit unsigned
		T_UInt32,        // UInt32 : 32 bit unsigned
		T_UInt64,        // UInt64 : 64 bit unsigned
		// Test Types
		T_Test,          // Test
		END_TYPE_ENUM,   // End Enum Mark
	}; // Interval : [ T_Null, END_TYPE_ENUM )

	using TypeUnit = uint_t;
	string to_string(DefaultType type);
	bool operator==(TypeUnit t1, DefaultType t2);
}
