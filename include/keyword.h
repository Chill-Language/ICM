#ifndef _ICM_KEYWORD_H_
#define _ICM_KEYWORD_H_

#include "ast.h"
#include "objects.h"
#include "function.h"

namespace ICM
{
	void createDefFuncTable();
}

extern ICM::FuncTable DefFuncTable;
extern ICM::FuncTable AddFuncTable;
extern ICM::VariableTable DefVariableTable;
extern ICM::VariableTable AddVariableTable;

#endif
