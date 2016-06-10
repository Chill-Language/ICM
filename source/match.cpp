#include "match.h"

namespace ICM
{
	MatchResult Match::matchNext() {
		DefaultType type = T_Null;
		const char *begin = currptr;
		int mode = 0;
		char findchar = '\0';
		for (; *currptr; ++currptr) {
			char c = *currptr;
			if (c == '\n')
				this->linenum++;
			switch (mode)
			{
			case 0:  // No Match
				if (c == '(') {
					++currptr;
					return MatchResult(T_LBracket, currptr - 1, currptr);
				}
				else if (c == ')') {
					++currptr;
					return MatchResult(T_RBracket, currptr - 1, currptr);
				}
				else if (c == ';') {
					type = T_Comment;
					begin = currptr;
					findchar = '\n';
					mode = 2;
				}
				else if (c == '"') {
					type = T_String;
					begin = currptr;
					findchar = '"';
					mode = 2;
				}
				else if (isdigit(c)) /* Number */ {
					type = T_Number;
					begin = currptr;
					mode = 1;
				}
				else if (ispunct(c)) /* Symbol */ {
					type = T_Identifier;
					begin = currptr;
					mode = 1;
				}
				else if (isalpha(c)) /* Idenit */ {
					type = T_Identifier;
					begin = currptr;
					mode = 1;
				}
				break;
			case 1:  // Match Long without findchar
				if (isBreakchar(c)) {
					auto mr = MatchResult(type, begin, currptr);
					if (isspace(c))
						++currptr;
					return mr;
				}
				break;
			case 2:  // Match Long with findchar
				if (c == findchar) {
					if (!isBreakchar(c) || isspace(c))
						++currptr;
					return MatchResult(type, begin, currptr);
					findchar = '\0';
				}
				break;
			default:
				break;
			}
		}

		return MatchResult(type, begin, currptr);
	}
}
