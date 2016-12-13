#include "basic.h"
#include "parser.h"
#include "objectdef.h"
#include "number.h"
#include "tabledata.h"
#include "literal.h"

namespace ICM
{
	namespace Parser
	{
		MatchResult Match::matchNext()
		{
			MatchResult mr;
			MatchType type = MT_Null;
			const char *begin = currptr;
			int mode = 0;
			vector<char> findchars;
			vector<char> ignorechars;
			bool firstfind = false;
			static const char breakfindchars[] = " \n\t()[];"; // include '\0'

			while (true) {
				char c = *currptr;
				if (c == '\n')
					this->linenum++;
				switch (mode)
				{
					case 0:  // No Match
						switch (c)
						{
							case '(':  /* Left Bracket */
								++currptr;
								mr = MatchResult(MT_LBracket, currptr - 1, currptr);
								goto EndMatch;
							case ')':  /* Right Bracket */
								++currptr;
								mr = MatchResult(MT_RBracket, currptr - 1, currptr);
								goto EndMatch;
							case '[':  /* Left Square Bracket */
								++currptr;
								mr = MatchResult(MT_LSBracket, currptr - 1, currptr);
								goto EndMatch;
							case ']':  /* Right Square Bracket */
								++currptr;
								mr = MatchResult(MT_RSBracket, currptr - 1, currptr);
								goto EndMatch;
							case ';':  /* Comment */
								findchars = { '\n', '\0' };
								//ignorechars = { '\\' };
								type = MT_Comment;
								begin = currptr;
								mode = 1;
								break;
							case '"':  /* String */
								findchars = { '"', '\0' };
								//ignorechars = { '\\' };
								type = MT_String;
								begin = currptr + 1;
								mode = 1;
								break;
							case '\'':  /* Symbol */
								findchars = { '\'', '\0' };
								type = MT_Symbol;
								begin = currptr + 1;
								mode = 1;
								break;
							case '-': /* (Maybe) Number */
								type = MT_Number;
								begin = currptr;
								mode = 3;
								firstfind = false;
								break;
							default:
								if (isdigit(c)) /* Number */ {
									type = MT_Number;
									begin = currptr;
									mode = 2;
								}
								else if (isalpha(c) || ispunct(c)) /* Idenit */ {
									type = MT_Identifier;
									begin = currptr;
									mode = 2;
								}
						}
						break;
					case 1:  // Match Long with findchar
						if (std::find(findchars.begin(), findchars.end(), c) != findchars.end()) {
							mr = MatchResult(type, begin, currptr);
							if (c != '\0')
								++currptr;
							goto EndMatch;
						}
						break;
					case 2:  // Match Long without findchar
						if (std::find(std::begin(breakfindchars), std::end(breakfindchars), c) != std::end(breakfindchars)) {
							mr = MatchResult(type, begin, currptr);
							goto EndMatch;
						}
						break;
					case 3:  // Match Long with function find
						if (isdigit(c)) {
							firstfind = true;
						}
						else {
							if (firstfind) {
								mr = MatchResult(type, begin, currptr);
								goto EndMatch;
							}
							else {
								currptr -= 2;
								type = MT_Identifier;
								mode = 2;
							}
						}
						break;
					default:
						break;
				}
				if (c == '\0')
					break;
				++currptr;
			}
			EndMatch:
			if (mr.getType() == MT_Identifier) {
				if (mr.getString() == "Nil")
					;//mr.setType(MT_Nil);
				else if (mr.getString() == "T" || mr.getString() == "F")
					mr.setType(MT_Boolean);
				else if (GlobalKeywordTable.findKey(mr.getString()) != GlobalKeywordTable.size())
					mr.setType(MT_Keyword);
			}

			return mr;
		}
	}
}

namespace ICM
{
	namespace Parser
	{
		AST::Element createElementData(MatchType type, const string &str)
		{
			switch (type) {
			case MT_Boolean:
				return AST::Element::Data(T_Boolean, Compiler::GlobalElementPool.setBoolean(str == "T"));
			case MT_Number:
				return AST::Element::Data(T_Number, Compiler::GlobalElementPool.setNumber(Common::Number::to_rational(str.c_str()).getNum() / Common::Number::to_rational(str.c_str()).getDen()));
			case MT_String:
				return AST::Element::Data(T_String, Compiler::GlobalElementPool.setString(str.c_str(), str.size() + 1));
			case MT_Keyword:
				return AST::Element::Keyword(GlobalKeywordTable.getValue(str));
			case MT_Identifier:
				return AST::Element::Identifier(str);
			default:
				return AST::Element::Data(T_Null, 0);
			}
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
					//println(mr);
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
					ast.pushData(AST::Element::Keyword(Keyword::list_));
					firstMatchBraket = false;
					emptybreak = true;
					break;
				case MT_RSBracket:
					ast.retNode();
					break;
				case MT_Identifier: case MT_Keyword:
				case MT_Number: case MT_String: case MT_Boolean:
					//if (firstMatchBraket) {
					//	printf("Error '%s' is not function in line(%d).\n", mr.getString().c_str(), match.getCurLineNum());
					//	return false;
					//}
					ast.pushData(createElementData(mr.getType(), mr.getString()));
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
			return true;
		}
	}
}
