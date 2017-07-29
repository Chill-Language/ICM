#pragma once
#include "instruction.h"

namespace ICM
{
	struct Object;

	Object* Run(Instruction::InstructionList &instlist, const Compiler::ElementPool &EP);
}
