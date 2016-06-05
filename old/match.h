#pragma once

#include "prints.h"
#include <cctype>

namespace ICM
{
	enum DefaultType {
		Unmatch,
		Null,
		Error,
		LBracket,
		RBracket,
		Number,
		Identifier, // Un Whole Match
		String,
		Comment,
		// Identifiers:
		Variable,
		Method,
		Keyword,
		Class,
		Void,
	};
	enum DefaultMethod
	{
		Nop,
		Add,
		Sub,
		Mul,
		Div,
		Equ,
		Sml,
		Sme,
		Lag,
		Lae,
		Dot,

		Assign,
		Lambda,
	};

	enum NumType { n_bin, n_oct, n_dec, n_hex, n_err };
	enum KeyType { k_nul, k_let, k_lambda };

	namespace Match
	{
		class MatchResult
		{
		public:
			MatchResult() = default;
			MatchResult(DefaultType type, const char *begin)
				: type(type), beginPtr(begin), endPtr(begin + 1), addmsg(0) { }
			MatchResult(DefaultType type, const char *begin, const char *end, int addmsg = 0)
				: type(type), beginPtr(begin), endPtr(end), addmsg(addmsg) { }

			DefaultType getType() const { return type; }
			std::string getString() const { return std::string(begin(), end()); }
			int getAddMsg() const { return addmsg; }
			const char *begin() const { return beginPtr; }
			const char *end() const { return endPtr; }

		private:
			DefaultType type = Unmatch;
			int addmsg = 0; // Additional Message
			const char *beginPtr = nullptr;
			const char *endPtr = nullptr;
		};

		MatchResult match(const char *begin);
		MatchResult matchNumber(const char *begin);
		MatchResult matchSymbol(const char *begin);
		MatchResult matchIdentifier(const char *begin);
		MatchResult matchLong(const char *begin, DefaultType type, char matchar, bool incend);

		MatchResult match(const char *begin)
		{
			const char *ptr = begin;
			while (*ptr) {
				char c = *ptr;
				/* Blank  */ if (isspace(c)) ptr++;
				/* Number */ else if (isdigit(c)) return matchNumber(ptr);
				/* Symbol */ else if (ispunct(c)) return matchSymbol(ptr);
				/* Idenit */ else if (isalpha(c)) return matchIdentifier(ptr);
			}
			return MatchResult();
		}
		MatchResult matchIdentifier(const char *begin)
		{
			const char *ptr = begin;
			DefaultType type = Identifier;
			KeyType t = k_nul;

			do { ptr++; } while (*ptr && (isalnum(*ptr) || *ptr == '_'));

			auto str = std::string(begin, ptr);
			if (str == "let") {
				type = Keyword;
				t = k_let;
			}
			else if (str == "lambda") {
				type = Keyword;
				t = k_lambda;
			}

			return MatchResult(type, begin, ptr, t);
		}
		MatchResult matchLong(const char *begin, DefaultType type, char matchar, bool incend)
		{
			const char *ptr = begin + 1;
			while (*ptr && *ptr != matchar) ptr++;
			return MatchResult(type, begin, ptr + (incend ? 1 : 0));
		}
		MatchResult matchNumber(const char *begin)
		{
			const char *ptr = begin;
			NumType t = n_dec;
			char fc = ptr[0], sc = ptr[1];

			if (isspace(sc) || ispunct(sc) || sc == '\0')
				return MatchResult(Number, begin, ptr + 1, n_dec);
			if (fc == '0') {
				if (isdigit(sc)) {
					if (sc != '8' || sc != '9') t = n_oct;
					else t = n_err;
				}
				else if (sc == 'b' || sc == 'B') {
					t = n_bin;
				}
				else if (sc == 'x' || sc == 'X') {
					t = n_hex;
				}
			}
			ptr += 2;
			
			while (*ptr)
			{
				char c = *ptr;
				// Judge Error
				if (!isxdigit(c)) {
					if (isspace(c) || ispunct(c)) break;
					else t = n_err;
				}
				else if (isalpha(c) && t != n_hex) {
					t = n_err;
				}
				// Get End
				switch (t)
				{
				case n_bin: if (c != '0' && c != '1') goto End; break;
				case n_oct: if (!(isdigit(c) && c != '8' || c != '9')) goto End; break;
				case n_dec: if (!isdigit(c)) goto End; break;
				case n_hex: if (!isxdigit(c)) goto End; break;
				case n_err: goto End;
				}
				ptr++;
			}

		End:
			if (t == n_err)
				return MatchResult(Error, begin, ptr + 1);
			else
				return MatchResult(Number, begin, ptr, t);
		}
		MatchResult matchSymbol(const char *begin)
		{
			DefaultType type = Method;
			DefaultMethod dm = Nop;
			const char *ptr = begin;

			switch (*begin) {
			case '(':  return MatchResult(LBracket, ptr, ptr + 1, 0);
			case ')':  return MatchResult(RBracket, ptr, ptr + 1, 0);
			case '[':  return MatchResult(LBracket, ptr, ptr + 1, 1);
			case ']':  return MatchResult(RBracket, ptr, ptr + 1, 1);
			case '\"': return matchLong(begin, String, '\"', true);
			case ';':  return matchLong(begin, Comment, '\n', false);
			case '.': dm = Dot; break;
			case '+': dm = Add; break;
			case '-': dm = Sub; break;
			case '*': dm = Mul; break;
			case '/': dm = Div; break;
			case '=': dm = Equ; break;
			case '<': dm = Sml; break;
			case '>': dm = Lag; break;
			default: type = Unmatch;
			}
			char c = ptr[1];
			if (c != '\0' && !isspace(c)) {
				switch (dm) {
				case Sml: if (c == '=') dm = Sme; ++ptr; break;
				case Lag: if (c == '=') dm = Lae; ++ptr; break;
				}
			}
			return MatchResult(type, begin, ptr + 1, (int)dm);
		}
	}

	const char *getTypeName(DefaultType type)
	{
		switch (type) {
		case Unmatch:    return "Unmatch";
		case Null:       return "Null";
		case Error:      return "Error";
		case LBracket:   return "LBracket";
		case RBracket:   return "RBracket";
		case Number:     return "Number";
		case Identifier: return "Identifier";
		case String:     return "String";
		case Comment:    return "Comment";
		case Variable:   return "Variable";
		case Method:     return "Method";
		case Keyword:    return "Keyword";
		case Class:      return "Class";
		case Void:       return "Void";
		default:              return "";
		}
	}

	void print(const Match::MatchResult &mr)
	{
		System::Output::print('(');
		System::Output::print(getTypeName(mr.getType()));
		System::Output::print(',');
		System::Output::print(' ');
		System::Output::print('\'');
		for (auto &c : mr) System::Output::print(c);
		System::Output::print('\'');
		System::Output::print(')');
	}
	void println(const Match::MatchResult &mr)
	{
		print(mr);
		System::Output::println();
	}
}
