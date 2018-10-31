#pragma once
#include "basic.h"

namespace ICM
{
	namespace Compiler
	{
		// * Literal
		// Boolean
		// Number
		// String
		// Symbol
		namespace Literal
		{
			struct StringType
			{
				StringType()
					: data(nullptr) {}
				StringType(char *data)
					: data(data) {}
				char *data = nullptr;
				const char* c_str() const {
					return data;
				}
				StringType& operator+=(const StringType& st) {
					string s1(data), s2(st.data);
					string *s = new string(s1 + s2); // TODO
					data = (char*)(s->c_str());      // TODO
					return *this;
				}
				bool operator==(const StringType& st) const {
					string s1(data), s2(st.data);
					return s1 == s2;
				}
			};

			using Boolean = bool;
			using Number = int_t;
			using String = StringType;
			using Symbol = uint_t;
			using Int = int_t;
			using UInt = uint_t;

		}

		class ElementPool : public BytePool
		{
		public:
			size_t setBoolean(Literal::Boolean v) {
				return insert(v);
			}
			size_t setNumber(Literal::Number&& v) {
				return insert(v);
			}
			size_t setString(const char *s, size_t len) {
				return insert(s, len);
			}
			size_t setInt(Literal::Int v) {
				return insert(v);
			}
			size_t setUInt(Literal::UInt v) {
				return insert(v);
			}
		};

		extern SerialBijectionMap<string> GlobalIdentNameMap;
	}

	inline string to_string(const Compiler::Literal::StringType &st) {
		return string(st.data);
	}
}
