#include "parser.h"
#include "objects.h"

namespace ICM
{
	using namespace Objects;

	ObjectPtr getData(DefaultType type, const string &str)
	{
		Object *object;
		switch (type)
		{
		case ICM::T_Null:
			object = nullptr;
			break;
		case ICM::T_Nil:
			object = new Nil();
			break;
		case ICM::T_Number:
			object = new Number(atoi(str.c_str()));
			break;
		case ICM::T_Boolean:
			object = new Boolean(str == "T");
			break;
		case ICM::T_String:
			object = new String(str);
			break;
		case ICM::T_Identifier:
			object = new Identifier(str);
			break;
		default:
			object = new Object();
			break;
		}
		return ObjectPtr(object);
	}

	inline void setData(AST *ast, const MatchResult &mr)
	{
		auto data = getData(mr.getType(), mr.getString());
		ast->pushNode(AST_DATA)->setdata(data);
	}
	AST* createAST(Match &match, const KeyWordMap &KeyWords)
	{
		AST *ast = new AST();
		MatchResult mr;

		bool firstMatchBraket = false;
		bool emptybreak = false;

		mr = match.matchNext();
		while (mr.begin() != mr.end()) {
			if (mr.getString()[0] != '\n') {
				//println(to_string(&mr));
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
					auto i = KeyWords.find(mr.getString());
					if (i == KeyWords.end()) {
						// TODO
						ast->setfunc(FUNC_ADD, 0);
					}
					else {
						ast->setfunc(FUNC_DEF, i->second);
					}
				}
				else {
					setData(ast, mr);
				}
				firstMatchBraket = false;
			}
			else if (mr.getType() == T_Number || mr.getType() == T_String || mr.getType() == T_Boolean) {
				setData(ast, mr);
			}

			if (emptybreak && ast->isend()) {
				break;
			}
			mr = match.matchNext();
		}
		if (!ast->isend())
			printf("Error match ')' in line(%d).\n", match.getCurLineNum());
		return ast->reset();
	}
}
