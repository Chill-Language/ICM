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
			case ICM::T_Null:
				object = nullptr;
				break;
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
			default:
				object = new Object();
				break;
			}
			return ObjectPtr(object);
		}

		AST* createAST(Match &match)
		{
			AST *ast = new AST();
			MatchResult mr;

			bool firstMatchBraket = false;
			bool emptybreak = false;

			mr = match.matchNext();
			while (mr.getType() == T_Null || mr.getType() == T_Comment) {
				if (mr.begin() != mr.end())
					mr = match.matchNext();
				else
					return nullptr;
			}

			if (mr.getType() != T_LBracket && mr.getType() != T_LSBracket) {
				printf("Syntax Error in line(%d).\n", match.getCurLineNum());
				return nullptr;
			}

			while (mr.begin() != mr.end()) {

				if (mr.getString()[0] != '\n') {
					//println(&mr);
				}

				switch (mr.getType()) {
				case T_LBracket:
					ast->pushNode();
					firstMatchBraket = true;
					emptybreak = true;
					break;
				case T_RBracket:
					ast->retNode();
					break;
				case T_LSBracket:
					ast->pushNode();
					ast->setFunc(FUNC_DEF, DefFuncTable.find("list"));
					firstMatchBraket = false;
					emptybreak = true;
					break;
				case T_RSBracket:
					ast->retNode();
					break;
				case T_Identifier:
					if (firstMatchBraket) {
						size_t i;
						if ((i = DefFuncTable.find(mr.getString()))) {
							// Default Function
							ast->setFunc(FUNC_DEF, i);
						}
						else if ((i = AddFuncTable.find(mr.getString()))) {
							// Adden Function
							ast->setFunc(FUNC_ADD, i);
						}
						firstMatchBraket = false;
					}
					else {
						size_t i;

						if ((i = DefVariableTable.find(mr.getString()))) {
							// Default Variable
							ASTNode *node = DefVariableTable[i].getData()->getRefNode();
							ast->pushNode(node);
						}
						else if ((i = AddVariableTable.find(mr.getString()))) {
							// Default Variable

							//ASTNode *node = AddVariableTable[i].getData()->getValue();
							//ast->pushNode(node);

							ObjectPtr data(AddVariableTable[i].getData());
							ast->pushData(data);
						}
						else {
							ASTNode *astnode = new ASTNode(ObjectPtr(new Nil()));
							ObjectPtr data(new Identifier(mr.getString(), astnode));
							ast->pushData(data);
						}
					}
					break;
				case T_Number: case T_String: case T_Boolean:
					if (firstMatchBraket) {
						printf("Error '%s' is not function in line(%d).\n", mr.getString().c_str(), match.getCurLineNum());
						return nullptr;
					}
					else {
						ObjectPtr data = createObject(mr.getType(), mr.getString());
						ast->pushData(data);
						break;
					}
				default:
					break;
				}

				if (emptybreak && ast->isend()) {
					break;
				}
				mr = match.matchNext();
			}
			if (ast->empty())
				return nullptr;

			if (!ast->isend()) {
				printf("Error match ')' in line(%d).\n", match.getCurLineNum());
				return nullptr;
			}
			return ast->reset();
		}
	}
}
