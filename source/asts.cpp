#include "asts.h"

namespace ICM
{
	AST* createAST(Match &match, const KeyWordMap &KeyWords)
	{
		AST *ast = new AST();
		MatchResult mr;

		bool firstMatchBraket = false;
		bool emptybreak = false;

		mr = match.matchNext();
		while (mr.begin() != mr.end()) {
			if (mr.getString()[0] != '\n') {
				//println(&mr);
			}
			if (mr.getType() == T_LBracket) {
				ast->pushNode(AST_FUNC);
				firstMatchBraket = true;
				emptybreak = true;
			}
			else if (mr.getType() == T_RBracket) {
				ast->retNode();
			}
			else if (mr.getType() == T_Identifier) {
				if (firstMatchBraket) {
					ast->setfunc(FUNC_DEF, KeyWords.at(mr.getString()));
				}
				else {
					ast->pushNode(AST_DATA)->setdata(mr.getString());
				}
				firstMatchBraket = false;
			}
			else if (mr.getType() == T_Number) {
				ast->pushNode(AST_DATA)->setdata(atoi(mr.getString().c_str()))->settype(T_Number);
			}
			else if (mr.getType() == T_String) {
				ast->pushNode(AST_DATA)->setdata<std::string>(mr.getString())->settype(T_String);
			}
			if (emptybreak && ast->isend()) {
				break;
			}
			mr = match.matchNext();
		}
		return ast;
	}
}