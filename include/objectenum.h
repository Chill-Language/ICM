#ifndef _ICM_OBJECTENUM_H_
#define _ICM_OBJECTENUM_H_

namespace ICM
{
	// Type
	enum DefaultType {
		T_Null,       // Null
		T_Object,     // Obj
		T_Nil,        // Nil
		T_Vary,       // Var
		T_Error,      // Err
		T_Identifier, // Idt
		T_Keyword,    // Key
		T_List,       // L
		T_Disperse,   // D   *
		T_Number,     // N
		T_String,     // S
		T_Char,       // C
		T_Function,   // F
		T_Type,       // T
		T_Boolean,    // Bool
		T_Symbol,     // Sym
		END_TYPE_ENUM, // End Enum Mark
	}; // Interval : [ T_Null, END_TYPE_ENUM )
}

#endif
