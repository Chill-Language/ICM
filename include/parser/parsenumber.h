#pragma once

namespace ICM
{
	namespace Parser
	{
		enum NumberType {
			nt_unknown,
			nt_int8,
			nt_int16,
			nt_int32,
			nt_int64,
			nt_uint8,
			nt_uint16,
			nt_uint32,
			nt_uint64,
			nt_float,
			nt_double,
			nt_ldouble,
		};

		NumberType parseNumber(const char *ptr, uint64_t &value);
	}
}
