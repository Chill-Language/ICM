#pragma once
namespace ICM
{
	enum IdentType {
		I_Void = 0,
		// Data
		I_Data,
		// Function
		I_Function,
		// Variable
		I_DyVarb,   // Dynamic Variable
		I_StVarb,   // Static Variable
		I_ReVarb,   // Reference Variable
		// Type
		I_Type,
		// Struct
		I_Struct,
	};
}
