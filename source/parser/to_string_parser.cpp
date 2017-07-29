#include "basic.h"
#include "parser/parser.h"
#include "parser/keywordtable.h"

namespace ICM
{
	string to_string(Parser::MatchType type)
	{
		using namespace Parser;

		switch (type) {
		case MT_Null:       return "Null";
		case MT_Blank:      return "Blank";
		case MT_Comment:    return "Comment";
		case MT_LBracket:   return "LBracket";
		case MT_RBracket:   return "RBracket";
		case MT_LSBracket:  return "LSBracket";
		case MT_RSBracket:  return "RSBracket";
		case MT_LLBracket:  return "LLBracket";
		case MT_RLBracket:  return "RLBracket";
		case MT_Identifier: return "Identifier";
		case MT_Suffix:     return "Suffix";
		case MT_Keyword:    return "Keyword";
		case MT_Number:     return "Number";
		case MT_Boolean:    return "Boolean";
		case MT_String:     return "String";
		case MT_DSymbol:    return "DSymbol";
		default:            return "UnfoundType";
		}
	}
	// MatchResult
	string to_string(const Parser::MatchResult &mr) {
		// Main
		string str;
		str.append("(");
		str.append(ICM::to_string(mr.getType()));
		str.append(", \'");
		str.append(mr.getString());
		str.append("')");
		return str;
	}
	string to_string(Keyword::KeywordID key)
	{
		size_t id = GlobalKeywordTable.findValue(key);
		if (id != GlobalKeywordTable.size())
			return GlobalKeywordTable.getData(id).first;
		else
			return "UnfoundKeyword";
	}
}
