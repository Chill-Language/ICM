#include "basic.h"
#include "keyword.h"
#include "function.h"
#include "deffunc.h"
#include "objectdef.h"
#include "number.h"
#include "tabledata.h"

#include "temp-getelement2.h"

namespace ICM
{
	void addDefFuncs(FuncTable &DefFuncTable);
	// Create Default FuncTable
	void createIdentTable()
	{
		addDefFuncs(GlobalFunctionTable);
		//
		for (const auto &var : GlobalFunctionTable) {
			IdentBasicIndex id = insertFromCurrentIdentTable(Compiler::GlobalIdentNameMap[var.first], I_StFunc);
			getFromCurrentIdentTable(id).FunctionIndex = GlobalFunctionTable.find(var.first);
		}
		setDyVarbData(IdentIndex{ 0, insertFromCurrentIdentTable(Compiler::GlobalIdentNameMap["true"], I_DyVarb) }, &Static.True);
		setDyVarbData(IdentIndex{ 0, insertFromCurrentIdentTable(Compiler::GlobalIdentNameMap["false"], I_DyVarb) }, &Static.False);
		// Import TypeInfoTable
		for (const auto &elt : TypeInfoTable) {
			const TypeInfo &info = elt.second;
			IdentBasicIndex id = insertFromCurrentIdentTable(Compiler::GlobalIdentNameMap[info.name], I_Type);
			getFromCurrentIdentTable(id).TypeIndex = info.index;
		}

		// TODO : Memory leak
		//GlobalDyVarbTable.insert("Nil");
		//DefVariableTable.insert("NIL", nil);
		//DefVariableTable.insert("nil", nil);
		//ObjectPtr nan(new Objects::Identifier("NaN", ObjectPtr(new Objects::Number(Common::Number::Rational(0, 0)))));
		//DefVariableTable.insert("NaN", nan);
	}

	BijectionKVMap<string, Keyword::KeywordID> GlobalKeywordTable = {
		{ "p",        Keyword::p_        },
		{ "let",      Keyword::let_      },
		{ "cpy",      Keyword::cpy_      },
		{ "ref",      Keyword::ref_      },
		{ "if",       Keyword::if_       },
		{ "?",        Keyword::ife_      },
		{ "then",     Keyword::then_     },
		{ "else",     Keyword::else_     },
		{ "elsif",    Keyword::elsif_    },
		{ "loop",     Keyword::loop_     },
		{ "while",    Keyword::while_    },
		{ "break",    Keyword::break_    },
		{ "for",      Keyword::for_      },
		{ "in",       Keyword::in_       },
		{ "to",       Keyword::to_       },
		{ "case",     Keyword::case_     },
		{ "when",     Keyword::when_     },
		{ "function", Keyword::function_ },
		{ "disp",     Keyword::disp_     },
		{ "list",     Keyword::list_     },
		{ "defun",    Keyword::defun_    },
		{ "define",   Keyword::define_   },
		{ "call",     Keyword::call_     },
		{ "do",       Keyword::do_       },
		{ "restrict", Keyword::restrict_ },
		{ "dim",      Keyword::dim_      },
	};
}
