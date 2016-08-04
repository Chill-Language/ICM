#include "match.h"

namespace ICM
{
	MatchResult Match::matchNext()
	{
		MatchResult mr;
		DefaultType type = T_Null;
		const char *begin = currptr;
		int mode = 0;
		vector<char> findchars;
		vector<char> ignorechars;
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
					mr = MatchResult(T_LBracket, currptr - 1, currptr);
					goto EndMatch;
				case ')':  /* Right Bracket */
					++currptr;
					mr = MatchResult(T_RBracket, currptr - 1, currptr);
					goto EndMatch;
				case '[':  /* Left Square Bracket */
					++currptr;
					mr = MatchResult(T_LSBracket, currptr - 1, currptr);
					goto EndMatch;
				case ']':  /* Right Square Bracket */
					++currptr;
					mr = MatchResult(T_RSBracket, currptr - 1, currptr);
					goto EndMatch;
				case ';':  /* Comment */
					findchars = { '\n', '\0' };
					//ignorechars = { '\\' };
					type = T_Comment;
					begin = currptr;
					mode = 1;
					break;
				case '"':  /* String */
					findchars = { '"', '\0' };
					//ignorechars = { '\\' };
					type = T_String;
					begin = currptr + 1;
					mode = 1;
					break;
				case '\'':  /* Symbol */ 
					findchars = { '\'', '\0' };
					type = T_Symbol;
					begin = currptr + 1;
					mode = 1;
					break;
				default:
					if (isdigit(c)) /* Number */ {
						type = T_Number;
						begin = currptr;
						mode = 2;
					}
					else if (isalpha(c) || ispunct(c)) /* Idenit */ {
						type = T_Identifier;
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
			default:
				break;
			}
			if (c == '\0')
				break;
			++currptr;
		}
		EndMatch:
		if (mr.getType() == T_Identifier) {
			if (mr.getString() == "Nil")
				;//mr.setType(T_Nil);
			else if (mr.getString() == "T" || mr.getString() == "F")
				mr.setType(T_Boolean);
		}

		return mr;
	}
}
