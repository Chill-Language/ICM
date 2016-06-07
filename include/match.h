namespace ICM
{
	enum DefaultType {
		T_Null,
		T_LBracket,
		T_RBracket,
		T_Number,
		T_String,
		T_Comment,
		T_Identifier, // Un Whole Match
	};
	enum NumType { n_bin, n_oct, n_dec, n_hex, n_err };

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
		DefaultType type = T_Null;
		int addmsg = 0; // Additional Message
		const char *beginPtr = nullptr;
		const char *endPtr = nullptr;
	};

	class Match
	{
	public:
		Match(const char *source) : source(source), currptr(source) {}

		// TODO: This Function should be fixed.
		MatchResult matchNext() {
			DefaultType type = T_Null;
			const char *begin = currptr;
			char findchar = '\0';
			bool find = false;
			for (; *currptr; ++currptr) {
				char c = *currptr;
				if (c == '\n') ++linenum;
				if (c == ';');
				if ((!find && (c == ';')) || (find && (isspace(c) || (c == findchar) || (c == '(' || c == ')')))) {
					if (type != T_Null)
						return MatchResult(type, begin, currptr);
					findchar = '\0';
				}
				if (isspace(c))
					begin = currptr + 1;
				else if (isdigit(c)) {
					type = T_Number;
					find = true;
				}
				else if (c == '(') {
					MatchResult mr(T_LBracket, begin, ++currptr);
					return mr;
				}
				else if (c == ')') {
					MatchResult mr(T_RBracket, begin, ++currptr);
					return mr;
				}
				else if (c == '+') {
					begin = currptr;
					type = T_Identifier;
					find = true;
				}

			}
			return MatchResult(type, begin, currptr);
		}

		unsigned getCurLineNum() {
			return linenum;
		}

	private:
		unsigned linenum = 0;
		const char *source;
		const char *currptr;
	};


	const char *getTypeName(DefaultType type)
	{
		switch (type) {
		case T_Null:       return "Null";
		case T_LBracket:   return "LBracket";
		case T_RBracket:   return "RBracket";
		case T_Number:     return "Number";
		case T_Identifier: return "Identifier";
		case T_String:     return "String";
		case T_Comment:    return "Comment";
		default:              return "";
		}
	}

	void print(const MatchResult &mr)
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
	void println(const MatchResult &mr)
	{
		print(mr);
		System::Output::println();
	}
}
