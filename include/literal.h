#pragma once
#include "basic.h"
#include "bytepool.h"

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
				StringType(char *data)
					:data(data) {}
				char *data = nullptr;
			};

			using Boolean = bool;
			using Number = int_t;
			using String = std::string;
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

		extern ElementPool GlobalElementPool;
	}


}
