#ifndef _ICM_MATCH_H_
#define _ICM_MATCH_H_

#include "basic.h"

namespace ICM
{
	enum NumType { n_bin, n_oct, n_dec, n_hex, n_err };

	enum MatchType {
		MT_Null,
		MT_LBracket,
		MT_RBracket,
		MT_LSBracket,
		MT_RSBracket,
		MT_Comment,
		MT_String,
		MT_Symbol,
		MT_Number,
		MT_Boolean,
		MT_Keyword,
		MT_Identifier,
	};

	string to_string(MatchType type);

	class MatchResult
	{
	public:
		MatchResult() = default;
		MatchResult(MatchType type, const char *begin)
			: type(type), beginPtr(begin), endPtr(begin + 1), addmsg(0) { }
		MatchResult(MatchType type, const char *begin, const char *end, int addmsg = 0)
			: type(type), beginPtr(begin), endPtr(end), addmsg(addmsg) { }
		
		void setType(MatchType type) { this->type = type; }
		MatchType getType() const { return type; }
		std::string getString() const { return std::string(begin(), end()); }
		int getAddMsg() const { return addmsg; }
		const char *begin() const { return beginPtr; }
		const char *end() const { return endPtr; }

		friend std::string to_string(const MatchResult *mr);

	private:
		MatchType type = MT_Null;
		const char *beginPtr = nullptr;
		const char *endPtr = nullptr;
		int addmsg = 0; // Additional Message
	};

	class Match
	{
	public:
		explicit Match(const char *source) : source(source), currptr(source) {}

		MatchResult matchNext();
		static bool isBreakchar(char c) {
			return isspace(c) || c == '(' || c == ')';
		}
		unsigned getCurLineNum() {
			return linenum;
		}
		bool isend() {
			return *currptr == '\0';
		}
		void reset() {
			this->currptr = this->source;
		}

	private:
		unsigned linenum = 1;
		const char *source;
		const char *currptr;
	};
}

#endif
