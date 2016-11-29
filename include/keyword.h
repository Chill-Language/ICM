#pragma once
#include "basic.h"

namespace ICM
{
	//=======================================
	// * Class Keyword
	//=======================================
	namespace Keyword
	{
		enum KeywordID {
			p_,
			let_,
			set_,
			cpy_,
			ref_,
			if_,
			then_,
			else_,
			elsif_,
			for_,
			in_,
			to_,
			while_,
			loop_,
			break_,
			case_,
			when_,
			function_,
			defun_,
			define_,
			defstruct_,
			with_,
			list_,
			disp_,
			do_,
			call_,
		};
	}

	string to_string(Keyword::KeywordID key);

	extern SerialBijectionMap<string> GlobalIdentifierMap;
}
