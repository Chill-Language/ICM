#include "basic.h"
#include "parser/parsenumber.h"

// ** Integer (int64_t [default] / uint64_t):
// * (+-) <0 [bodx]> [0-9,a-F,A-F]* (iu)
// Examples :
//   0xff = 0xffi = 255 = 255i -> int16
//   0xffu = 255u -> uint8

// ** Float (long double): 
// * (+-) <0 [bodx]> [0-9]* . <[0-9]*>
// * (+-) [0-9]* e (+-) [0-9]* <(+-) [0-9]*>
// Examples :
//   0. = 0.0
//   0x10. = 16.0
//   0x10.5 = 16.3125
//   10e1 = 100
//   10e+1 = 100
//   10e1+1 = 101
//   10e+1+1 = 101

namespace ICM
{
	namespace Parser
	{
		enum Format { f_bin = 2, f_oct = 8, f_dec = 10, f_hex = 16 };

		static int convertChar(char c)
		{
			assert(isdigit(c) || isalpha(c));

			if (c >= '0' && c <= '9')
				return c - '0';
			else if (c >= 'A' && c <= 'Z')
				return c - 'A' + 10;
			else
				return c - 'a' + 10;
		}

		static bool judgeCharNumber(char c, int base)
		{
			return (isdigit(c) || isalpha(c)) && convertChar(c) < base;
		}

		static long double convertFloatPointBehind(const char *str, int base)
		{
			int i = base;
			long double result = 0;

			while (*str) {
				int v = convertChar(*str++);
				assert(v < base);
				result += v * 1.0 / i;
				i *= base;
			}

			return result;
		}

		static NumberType getLimitTypeFloat(uint64_t &value)
		{
			long double ld = (long double&)value;
			double d = (double)ld;
			float f = (float)ld;

			if (f == ld) return nt_float;
			if (d == ld) return nt_double;
			return nt_ldouble;
		}


		static NumberType getLimitTypeInt(uint64_t &value)
		{
			int64_t i64 = (int64_t&)value;
			int32_t i32 = (int32_t)i64;
			int16_t i16 = (int16_t)i64;
			int8_t i8 = (int8_t)i64;

			if (i8 == i64) return nt_int8;
			if (i16 == i64) return nt_int16;
			if (i32 == i64) return nt_int32;
			return nt_int64;
		}


		static NumberType getLimitTypeUInt(uint64_t &value)
		{
			if (value > MaxValue<uint32_t>()) return nt_uint64;
			if (value > MaxValue<uint16_t>()) return nt_uint32;
			if (value > MaxValue<uint8_t>()) return nt_uint16;
			return nt_uint8;
		}

		static NumberType parseFloat(const string &front, const string &behind, uint64_t &value, bool minus, Format format)
		{
			long double f = (long double)std::stoull(front.c_str(), 0, format);
			long double b = convertFloatPointBehind(behind.c_str(), format);

			(long double&)value = (f + b) * (minus ? -1 : 1);
			return getLimitTypeFloat(value);
		}

		static NumberType parseFloat(const string &str, uint64_t &value, bool minus)
		{
			long double v = std::stold(str.c_str());

			(long double&)value = v * (minus ? -1 : 1);
			return getLimitTypeFloat(value);
		}

		static NumberType parseUnsigned(const string &str, uint64_t &value, Format format)
		{
			(uint64_t&)value = std::stoull(str.c_str(), 0, format);
			return getLimitTypeUInt(value);
		}

		static NumberType parseInteger(const string &str, uint64_t &value, bool minus, Format format)
		{
			string s = (minus ? "-" : "") + str;
			(int64_t&)value = std::stoll(s.c_str(), 0, format);
			return getLimitTypeInt(value);
		}

		static bool checkBehindNumber(const char *ptr, int base = 10, const char **oend = nullptr)
		{
			while (judgeCharNumber(*ptr, base)) {
				ptr++;
			}

			if (oend)
				*oend = ptr;

			return *ptr == '\0';
		}

		NumberType parseNumber(const char *ptr, uint64_t &value)
		{
			bool minus = false;

			Format format = f_dec;

			if (*ptr == '+') {
				ptr++;
			}
			else if (*ptr == '-') {
				minus = true;
				ptr++;
			}

			if (*ptr == '0') {
				ptr++;
				switch (*ptr) {
				case 'b': format = f_bin; ptr++; break;
				case 'o': format = f_oct; ptr++; break;
				case 'd': format = f_dec; ptr++; break;
				case 'x': format = f_hex; ptr++; break;
				case '.':
					// Include : 0. -> 0.0
					return checkBehindNumber(ptr + 1, format) ? parseFloat(ptr - 1, value, minus) : nt_unknown;
				default: return nt_unknown;
				}
			}

			if (!judgeCharNumber(*ptr, format))
				return nt_unknown;

			const char *end;
			if (checkBehindNumber(ptr, format, &end)) {
				return parseInteger(ptr, value, minus, format);
			}
			else {
				if (*end == 'i' || *end == 'u') {
					charptr cp(ptr, end);

					if (*end == 'i') {
						return parseInteger(cp.to_string(), value, minus, format);
					}
					else {
						if (minus) return nt_unknown;

						return parseUnsigned(cp.to_string(), value, format);
					}
				}

				if (*end == '.') {
					end++;
					charptr front(ptr, end);
					ptr = end;

					// Include the number : XXX. -> XXX.0
					if (*ptr == '\0') (void)0;

					if (!checkBehindNumber(ptr, format))
						return nt_unknown;

					return parseFloat(front.to_string(), ptr, value, minus, format);
				}

				if (*end == 'e') {
					if (format != f_dec)
						return nt_unknown;

					end++;
					if (*end == '\0')
						return nt_unknown;

					if (*end == '+' || *end == '-') {
						end++;
					}
					while (isdigit(*end)) {
						end++;
					}
					if (*end == '+' || *end == '-') {
						charptr front(ptr, end);
						long double result1;
						parseFloat(front.to_string(), (uint64_t&)result1, minus);

						minus = *end == '-';
						ptr = end + 1;
						if (!isdigit(*ptr))
							return nt_unknown;

						if (!checkBehindNumber(ptr, format))
							return nt_unknown;

						int64_t result2;
						parseInteger(ptr, (uint64_t&)result2, minus, f_dec);

						(long double&)value = result1 + (long double)result2;
						return getLimitTypeFloat(value);
					}
					if (*end != '\0')
						return nt_unknown;

					return parseFloat(ptr, value, minus);
				}
			}

			return nt_unknown;
		}
	}
}
