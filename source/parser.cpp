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
			default:
				object = nullptr;
				break;
			}
			return ObjectPtr(object);
		}
		bool createIdentFuncASTNode(AST *ast, const MatchResult &mr);
		bool createIdentDataASTNode(AST *ast, const MatchResult &mr);
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
					if (firstMatchBraket) {
						println("Unfind Method in Line(", match.getCurLineNum(), ")."); // TODO
						return nullptr;
					}
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
						bool result = createIdentFuncASTNode(ast, mr);
						if (!result) {
							println("Undefined method '" + mr.getString() + "' in line(" + std::to_string(match.getCurLineNum()) + ").");
							return nullptr;
						}
						firstMatchBraket = false;
					}
					else {
						createIdentDataASTNode(ast, mr);
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
		bool createIdentFuncASTNode(AST *ast, const MatchResult &mr)
		{
			size_t i;
			if ((i = DefFuncTable.find(mr.getString()))) {
				// Default Function
				ast->setFunc(FUNC_DEF, i);
			}
			else if ((i = AddFuncTable.find(mr.getString()))) {
				// Adden Function
				ast->setFunc(FUNC_ADD, i);
			}
			else if ((i = AddVariableTable.find(mr.getString()))) {
				// Default Variable
				ObjectPtr op = AddVariableTable[i].getData()->getData();
				if (op->get_type() == T_Function) {
					AddFuncTable.add(mr.getString(), getPointer<Objects::Function>(op)->get_data());
					size_t id = AddFuncTable.getCurrentID();
					ast->setFunc(FUNC_ADD, id);
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
			return true;
		}
		void createFuncData(const FuncTableUnit &r, AST *ast, const MatchResult &mr)
		{
			ObjectPtr data(new Identifier(mr.getString(), ObjectPtr(new Objects::Function(&r))));
			ast->pushData(data);
		}
		bool createIdentDataASTNode(AST *ast, const MatchResult &mr)
		{
			size_t i;

			if ((i = DefFuncTable.find(mr.getString()))) {
				// Default Function
				createFuncData(DefFuncTable[i], ast, mr); // Get Func Value
			}
			else if ((i = AddFuncTable.find(mr.getString()))) {
				// Adden Function
				createFuncData(AddFuncTable[i], ast, mr); // Get Func Value
			}
			else if ((i = DefVariableTable.find(mr.getString()))) {
				// Default Variable
				ObjectPtr data = DefVariableTable[i].getData()->getData(); // Get Value
				ast->pushData(data);
			}
			else if ((i = AddVariableTable.find(mr.getString()))) {
				// Adden Variable
				ObjectPtr data(AddVariableTable[i].getData()); // Get Ident
				ast->pushData(data);
			}
			else {
				ObjectPtr data(new Identifier(mr.getString(), ObjectPtr(new Nil())));
				ast->pushData(data);
			}
			return true;
		}
	}
}
