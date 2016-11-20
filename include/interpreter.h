#ifndef _ICM_INTERPRETER_H_
#define _ICM_INTERPRETER_H_

#include "instruction.h"

namespace ICM
{
	struct Object;

	Object* Run(Instruction::InstructionList &instlist);
}

#endif
