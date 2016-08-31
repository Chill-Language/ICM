#include "parser.h"
#include "objectsdef.h"
#include "number.h"

namespace ICM
{
	namespace Parser
	{
		using namespace Objects;

		ObjectPtr createObject(DefaultType type, const string &str)
		{
			using namespace Objects;

			Object *object;
			switch (type)
			{
			case ICM::T_Nil:
				object = new Nil();
				break;
			case ICM::T_Number:
				object = new Number(Common::Number::to_rational(str.c_str()));
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
			case ICM::T_Keyword:
				object = new Keyword(DefKetwordTable[str].getData());
				break;
			default:
				object = nullptr;
				break;
			}
			return ObjectPtr(object);
		}

		bool createAST(Match &match, AST &ast)
		{
			MatchResult mr = match.matchNext();

			// Check Unmatch
			while (mr.getType() == T_Null || mr.getType() == T_Comment) {
				if (mr.begin() != mr.end())
					mr = match.matchNext();
				else
					return false;
			}
			if (mr.getType() != T_LBracket && mr.getType() != T_LSBracket) {
				printf("Syntax Error in line(%d).\n", match.getCurLineNum());
				return false;
			}

			// Start Match
			bool firstMatchBraket = false;
			bool emptybreak = false;

			while (mr.begin() != mr.end()) {

				if (mr.getString()[0] != '\n') {
					//println(&mr);
				}

				switch (mr.getType()) {
				case T_LBracket:
					ast.pushNode();
					firstMatchBraket = true;
					emptybreak = true;
					break;
				case T_RBracket:
					if (firstMatchBraket) {
						println("Unfind Method in Line(", match.getCurLineNum(), ").");
						return false;
					}
					ast.retNode();
					break;
				case T_LSBracket:
					ast.pushNode();
					ast.pushData(ICM::createObject<Objects::Function>(DefFuncTable.find("list")));
					firstMatchBraket = false;
					emptybreak = true;
					break;
				case T_RSBracket:
					ast.retNode();
					break;
				case T_Identifier: case T_Keyword:
					ast.pushData(Parser::createObject(mr.getType(), mr.getString()));
					firstMatchBraket = false;
					break;
				case T_Number: case T_String: case T_Boolean:
					if (firstMatchBraket) {
						printf("Error '%s' is not function in line(%d).\n", mr.getString().c_str(), match.getCurLineNum());
						return false;
					}
					ast.pushData(Parser::createObject(mr.getType(), mr.getString())); // TODO
					firstMatchBraket = false;
					break;
				default:
					break;
				}

				if (emptybreak && ast.isend()) {
					break;
				}
				mr = match.matchNext();
			}

			if (ast.empty())
				return false;

			if (!ast.isend()) {
				printf("Error match ')' in line(%d).\n", match.getCurLineNum());
				return false;
			}
			ast.reset();
			return true;
		}
	}
}
