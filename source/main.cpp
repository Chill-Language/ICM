#include "prints.h"
#include <map>
#include "asts.h"
using namespace System;

template <typename T>
void println(const T &obj)
{
	Output::println(to_string(obj));
}

using namespace ICM;

int main(void)
{
	const char *text = "(+ 5 6 (- 7 8))";

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
	};

	AST *ast;
	Match match(text);

	while (!match.isend()) {
		ast = createAST(match, KeyWords);
		println(ast);
		delete ast;
	}

	return 0;
}
