#pragma once
namespace ICM
{
	enum IdentType {
		I_Void = 0,
		// Data
		I_Data,
		// Function
		I_StFunc,   // Static Function
		I_DyFunc,   // Dynamic Function
		// Variable
		I_DyVarb,   // Dynamic Variable
		I_StVarb,   // Static Variable
		I_ReVarb,   // Reference Variable
		// Type
		I_Type,
		// Struct
		I_Struct,
		// Module
		I_Module,
	};
}
