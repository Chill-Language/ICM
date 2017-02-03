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
	void createIdentTable()
	{
		addDefFuncs(GlobalFunctionTable);
		//
		for (const auto &var : GlobalFunctionTable) {
			size_t id = GlobalIdentTable.insert(var.first, I_Function);
			GlobalIdentTable.at(id).FunctionIndex = GlobalFunctionTable.find(var.first);
		}
		setDyVarbData(GlobalIdentTable.insert("true", I_DyVarb), &Static.True);
		setDyVarbData(GlobalIdentTable.insert("false", I_DyVarb), &Static.False);
		// Import TypeInfoTable
		for (const auto &elt : TypeInfoTable) {
			const TypeInfo &info = elt.second;
			size_t id = GlobalIdentTable.insert(info.name, I_Type);
			GlobalIdentTable.at(id).TypeIndex = info.index;
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
