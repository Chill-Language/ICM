#pragma once
#include "parser/literal.h"

namespace ICM
{
	namespace Compiler
	{
		namespace Literal
		{
			namespace NumberCalc
			{
				template <typename _T1, typename _T2>
				struct ResultType;

				template <typename _T1, typename _T2>
				typename ResultType<_T1, _T2>::Type add(_T1 x, _T2 y) { return x + y; }

				template <typename _T1, typename _T2>
				typename ResultType<_T1, _T2>::Type sub(_T1 x, _T2 y) { return x - y; }

				template <typename _T1, typename _T2>
				typename ResultType<_T1, _T2>::Type mul(_T1 x, _T2 y) { return x * y; }

				template <typename _T1, typename _T2>
				typename ResultType<_T1, _T2>::Type div(_T1 x, _T2 y) { return x / y; }

				template <> struct ResultType<Int32, Int32> { using Type = Int32; };
				template <> struct ResultType<UInt32, UInt32> { using Type = UInt32; };

				template <> struct ResultType<Int64, Int64> { using Type = Int64; };
				template <> struct ResultType<UInt64, UInt64> { using Type = UInt64; };

				template <> struct ResultType<LDouble, LDouble> { using Type = LDouble; };

				template <> struct ResultType<Int32, UInt32> { using Type = UInt32; };
				template <> struct ResultType<UInt32, Int32> { using Type = UInt32; };
				template <> struct ResultType<Int64, UInt64> { using Type = UInt64; };
				template <> struct ResultType<UInt64, Int64> { using Type = UInt64; };

				template <> struct ResultType<Int32, LDouble> { using Type = LDouble; };
				template <> struct ResultType<UInt32, LDouble> { using Type = LDouble; };
				template <> struct ResultType<LDouble, Int32> { using Type = LDouble; };
				template <> struct ResultType<LDouble, UInt32> { using Type = LDouble; };
				template <> struct ResultType<LDouble, Int64> { using Type = LDouble; };
				template <> struct ResultType<LDouble, UInt64> { using Type = LDouble; };
			}
		}
	}
}
