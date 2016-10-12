#include "parser.h"
#include "objectsdef.h"
#include "number.h"

namespace ICM
{
	namespace Parser
	{
		using namespace Objects;

		Objects::Object* createObject(MatchType type, const string &str)
		{
			using namespace Objects;

			Objects::Object *object;
			switch (type)
			{
			//case ICM::MT_Nil:
			//	object = new Nil();
			//	break;
			case ICM::MT_Number:
				object = new Number(Common::Number::to_rational(str.c_str()));
				break;
			case ICM::MT_Boolean:
				object = new Boolean(str == "T");
				break;
			case ICM::MT_String:
				object = new String(str);
				break;
			case ICM::MT_Identifier:
				object = new Identifier(str);
				break;
			case ICM::MT_Keyword:
				object = new Keyword(DefKeywordTable.getValue(str));
				break;
			default:
				object = nullptr;
				break;
			}
			return object;
		}

		bool createAST(Match &match, AST &ast)
		{
			MatchResult mr = match.matchNext();

			// Check Unmatch
			while (mr.getType() == MT_Null || mr.getType() == MT_Comment) {
				if (mr.begin() != mr.end())
					mr = match.matchNext();
				else
					return false;
			}
			if (mr.getType() != MT_LBracket && mr.getType() != MT_LSBracket) {
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
				case MT_LBracket:
					ast.pushNode();
					firstMatchBraket = true;
					emptybreak = true;
					break;
				case MT_RBracket:
					if (firstMatchBraket) {
						println("Unfind Method in Line(", match.getCurLineNum(), ").");
						return false;
					}
					ast.retNode();
					break;
				case MT_LSBracket:
					ast.pushNode();
					ast.pushData(new Objects::Function(DefFuncTable.find("list")));
					firstMatchBraket = false;
					emptybreak = true;
					break;
				case MT_RSBracket:
					ast.retNode();
					break;
				case MT_Identifier: case MT_Keyword:
					ast.pushData(createObject(mr.getType(), mr.getString()));
					firstMatchBraket = false;
					break;
				case MT_Number: case MT_String: case MT_Boolean:
					if (firstMatchBraket) {
						printf("Error '%s' is not function in line(%d).\n", mr.getString().c_str(), match.getCurLineNum());
						return false;
					}
					ast.pushData(createObject(mr.getType(), mr.getString())); // TODO
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
