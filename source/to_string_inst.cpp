#include "basic.h"
#include "ast.h"
#include "instruction.h"

namespace ICM
{
	string to_string(Instruction::Instruction inst)
	{
		size_t id = Instruction::InstructionName.findKey(inst);
		if (id != Instruction::InstructionName.size())
			return Instruction::InstructionName.getValue(id);
		else
			return "UnknowInstruction";
	}
	string to_string(const Instruction::Element &elt) {
		return to_string(*(ASTBase::Element*)&elt);
	}
	string to_string(const vector<Instruction::Element> &elt) {
		return to_string(*(vector<ASTBase::Element>*)&elt);
	}
}
