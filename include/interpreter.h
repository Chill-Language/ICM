#ifndef _ICM_INTERPRETER_H_
#define _ICM_INTERPRETER_H_

#include "basic.h"
#include "objectdef.h"
#include "instruction.h"

namespace ICM
{
	class Interpreter
	{
	public:
		Interpreter(Instruction::InstructionList &InstList)
			: InstList(InstList), TempResult(InstList.size(), &Static.Null) {}

		DataList createDataList(const vector<AST::Element> &args);
		Object* run();

	private:
		Instruction::InstructionList& InstList;
		Object* Result = &Static.Nil;
		vector<Object*> TempResult;

		Object* getObject(AST::Element &element);

		struct {
			struct {
				Types::Function Func;
				DataList Args;
			} Func;
		} Global;
	};
}

#endif
