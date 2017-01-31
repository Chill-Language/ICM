#include "basic.h"
#include "keyword.h"
#include "function.h"
#include "deffunc.h"
#include "objectdef.h"
#include "number.h"
#include "tabledata.h"

#include "temp-getelement.h"

namespace ICM
{
	void addDefFuncs(FuncTable &DefFuncTable);
	// Create Default FuncTable
	void createDefFuncTable()
	{
		addDefFuncs(GlobalFunctionTable);
		//
#if _USE_IDENTTABLE_
		for (const auto &var : GlobalFunctionTable.keymap) {
			size_t id = GlobalIdentTable.insert(var.first, I_Function);
			GlobalIdentTable.at(id).FunctionIndex = GlobalFunctionTable.find(var.first);
		}
		GlobalIdentTable.insert("Nil", I_DyVarb);
#endif
		// TODO : Memory leak
		GlobalDyVarbTable.insert("Nil");
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
	};
}
