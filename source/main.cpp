#include "prints.h"
#include <map>
#include "parser.h"
using namespace System;

template <typename T>
void println(const T &obj)
{
	Output::println(to_string(obj));
}

using namespace ICM;

int main(void)
{
	const char *text = "(+ 5 6)";

	KeyWordMap KeyWords {
		KeyWord("+", 1),
		KeyWord("-", 2),
		KeyWord("*", 3),
		KeyWord("/", 4),
		KeyWord("=", 5),
		KeyWord(">", 6),
		KeyWord("<", 7),
		KeyWord(">=", 8),
		KeyWord("<=", 9),
		KeyWord("list",10),
	};

	// Parser

	Match match(text);

	while (!match.isend()) {
		AST *ast = createAST(match, KeyWords);
		println(ast);
		delete ast;
	}

	return 0;
}
