#include "prints.h"
#include "file.h"
#include <cctype>
using namespace std;
using SYSTEM;
USING Output;

#include "match.h"
using namespace ICM;

namespace ICM
{
	void putError(const char *msg)
	{
		printf("#Error:\n>  ");
		puts(msg);
	}

	struct ObjectData
	{
	public:
		template <typename T> T getData() { return (*((T*)pointer)); }
		void *pointer = nullptr;
	};

	class MatchData
	{
	public:
		MatchData() = default;
		MatchData(const Match::MatchResult &mr)
			: type(mr.getType()), addmsg(mr.getAddMsg()) {
			initValue(mr.getString());
		}
		void initValue(const std::string &str) {
			switch (type) {
			case Number:
				int value = atoi(str.c_str());
				System::Output::print("Value = ");
				System::Output::println(value);
				break;
			}
		}

		DefaultType type = Void;
		int addmsg = 0;

	};

	template <typename T>
	class BTree
	{
		class Node
		{
			T data;
			Node *left;
			Node *right;
		};

	};
}

int main(void)
{
	File file("F:/Code/Projects/Editor/ICM/run.chl", File::LOAD_TEXT);
	const char *str = file.get_textdata();
	println(str);
	println();

	auto result = ICM::Match::match(str);
	while (result.begin() != result.end())
	{
		println(result);
		ICM::MatchData matchdata(result);
		result = ICM::Match::match(result.end());
	}

	return 0;
}
