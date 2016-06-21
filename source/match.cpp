#include "match.h"

namespace ICM
{
	MatchResult Match::matchNext() {
		MatchResult mr;
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
					mr = MatchResult(T_LBracket, currptr - 1, currptr);
					goto EndMatch;
				}
				else if (c == ')') {
					++currptr;
					mr = MatchResult(T_RBracket, currptr - 1, currptr);
					goto EndMatch;
				}
				else if (c == ';') {
					type = T_Comment;
					begin = currptr;
					findchar = '\n';
					mode = 2;
				}
				else if (c == '"') {
					type = T_String;
					begin = currptr + 1;
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
					mr = MatchResult(type, begin, currptr);
					if (isspace(c))
						++currptr;
					goto EndMatch;
				}
				break;
			case 2:  // Match Long with findchar
				if (c == findchar) {
					if (!isBreakchar(c) || isspace(c))
						++currptr;
					mr = MatchResult(type, begin, currptr - 1);
					goto EndMatch;
				}
				break;
			default:
				break;
			}
		}

	EndMatch:
		if (mr.getType() == T_Identifier) {
			if (mr.getString() == "Nil")
				mr.setType(T_Nil);
			else if (mr.getString() == "T" || mr.getString() == "F")
				mr.setType(T_Boolean);
		}

		return mr;
	}
}
