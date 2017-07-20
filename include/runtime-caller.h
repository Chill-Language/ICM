#pragma once
#include "object.h"

template <typename... Ts> struct List;

template <typename LTs, typename T>
struct PushBack;
template <template <typename...> class LTs, typename... Ts, typename T>
struct PushBack<LTs<Ts...>, T> { using Type = LTs<Ts..., T>; };

template <typename LTs, typename T>
struct PushFront;
template <template <typename...> class LTs, typename... Ts, typename T>
struct PushFront<LTs<Ts...>, T> { using Type = LTs<T, Ts...>; };

template <typename LNs, size_t N>
struct PushBackN;
template <template <size_t...> class LNs, size_t... Ns, size_t N>
struct PushBackN<LNs<Ns...>, N> { using Type = LNs<Ns..., N>; };

template <typename LNs, size_t N>
struct PushFrontN;
template <template <size_t...> class LNs, size_t... Ns, size_t N>
struct PushFrontN<LNs<Ns...>, N> { using Type = LNs<N, Ns...>; };

namespace Runtime
{
	using FuncResult = ICM::Object*&;
	using FuncArgument = const ICM::DataList&;
	using FuncCaller = void(FuncResult, FuncArgument);

	inline void call(FuncCaller *func, FuncResult res, FuncArgument argv) {
		func(res, argv);
	}

	template <typename T>
	inline void setResult(FuncResult res, T &&data) {
		// TODO
		static T sdata;
		sdata = data;
		res = new ICM::Object(ICM::Types::CType<T>::index, &sdata);
	}

	template <typename T, size_t N>
	inline T getArgument(FuncArgument argv) {
		//ICM::println("(", typeid(T).name(), ", ", N, ")");
		return argv[N]->dat<ICM::Types::CType<T>::index>();
	}

	template <typename _TR, typename... _TAs>
	struct CallerBase
	{
		using FuncType = _TR(*) (_TAs...);

	private:
		template <typename... Pair>
		struct FuncList {
			template <FuncType f>
			inline constexpr static _TR func(FuncArgument argv) {
				return f(Pair::func(argv)...);
			}
		};

		template <typename T, size_t N>
		struct GetArgument {
			constexpr static auto func = getArgument<T, N>;
		};

		template <typename List, template <typename, size_t> class Pair, typename... Ts>
		struct GetListFunc
		{
		private:
			template <size_t NC, size_t N, typename UTs, typename... _Ts>
			struct Sub;

			template <size_t NC, size_t N, typename UTs, typename T0, typename... _Ts>
			struct Sub<NC, N, UTs, T0, _Ts...> : Sub<NC, N - 1, typename PushBack<UTs, Pair<T0, NC - N>>::Type, _Ts...> {};

			template <size_t NC, typename UTs>
			struct Sub<NC, 0, UTs> { using Type = UTs; };

		public:
			using Type = Sub<sizeof...(Ts), sizeof...(Ts), List, Ts...>;
		};

	public:
		template <FuncType f>
		inline static void call(FuncResult res, FuncArgument argv) {
			setResult<_TR>(res, GetListFunc<FuncList<>, GetArgument, _TAs...>::Type::Type::template func<f>(argv));
		}
	};

	template <typename _TR>
	struct CallerBase<_TR, void>
	{
		template <_TR(*f) (void)>
		inline static void call(FuncResult res, FuncArgument argv) {
			setResult<_TR>(res, f());
		}
	};
	
	template <>
	struct CallerBase<void, void>
	{
		template <void(*f) (void)>
		inline static void call(FuncResult res, FuncArgument argv) {
			f();
		}
	};

	template <typename _LTA, typename _TR>
	struct Caller;

	template <template <typename...> class L, typename... _TAs, typename _TR>
	struct Caller<L<_TAs...>, _TR>
	{
		using Type = typename PushFront<CallerBase<_TAs...>, _TR>::Type;
	};
}
