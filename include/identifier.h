#pragma once
namespace ICM
{
	enum IdentType {
		I_Void,
		// Data
		I_Data,
		// Struct
		I_Struct,
		// Function
		I_Function,
		// Variable
		I_DyVarb,   // Dynamic Variable
		I_StVarb,   // Static Variable
		I_ReVarb,   // Reference Variable
	};
}
