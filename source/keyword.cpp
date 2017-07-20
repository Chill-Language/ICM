#include "basic.h"
#include "keyword.h"

namespace ICM
{
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
