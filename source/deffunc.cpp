#include "deffunc.h"
#include "objectsdef.h"
#include "tostring.h"
#include "keyword.h"
#include "tostring.h"
#include "function.h"

namespace ICM
{
	using namespace Objects;

	namespace DefFunc
	{
		ObjectPtr callDefFunc(const string &name, const DataList &dl) {
			if (DefFuncTable.find(name))
				return checkCall(DefFuncTable[name], dl);
			else
				return createError("Error in match function(" + name + ").");
		}

		//=======================================
		// * Class *
		//=======================================

		using F = Function::FuncObject;
		using FI = Function::FuncInitObject;
		using S = Function::Signature;

		namespace Calc
		{
			template <typename T>
			struct Add : public FI
			{
			private:
				S sign() const {
					return S({ T::Type }, T::Type, true); // T* -> T
				}
				ObjectPtr func(const DataList &list) const {
					T *tmp = getPointer<T>(list.front())->clone();
					for (auto i : Range<size_t>(1, list.size() - 1))
						tmp->add(getPointer<T>(list[i]));
					return ObjectPtr(tmp);
				}
			};
			template <>
			struct Add<void> : public FI
			{
			private:
				S sign() const {
					return S({}, T_Number); // Void -> N
				}
				ObjectPtr func(const DataList &list) const {
					return ObjectPtr(new Number(0));
				}
			};

			struct NumCall : public FI
			{
			public:
				using N = Number;
				using Func = std::function<void(N*, N*)>;
				NumCall(const Func &fp) : fp(fp) {}

			private:
				S sign() const {
					return S({ T_Number }, T_Number, true); // N* -> N
				}
				ObjectPtr func(const DataList &list) const {
					N *tmp = getPointer<N>(list.front())->clone();
					for (auto i : Range<size_t>(1, list.size() - 1))
						fp(tmp, getPointer<N>(list[i]));
					return ObjectPtr(tmp);
				}
				Func fp;
			};
			struct Sub : public NumCall { Sub() : NumCall([](N *num, N *obj) { num->sub(obj); }) {} };
			struct Mul : public NumCall { Mul() : NumCall([](N *num, N *obj) { num->mul(obj); }) {} };
			struct Div : public NumCall { Div() : NumCall([](N *num, N *obj) { num->div(obj); }) {} };

			struct Mod : public FI
			{
			private:
				S sign() const {
					return S({ T_Number, T_Number }, T_Number); // (N N) -> N
				}
				ObjectPtr func(const DataList &list) const {
					return ObjectPtr(getPointer<Number>(list[0])->mod(getPointer<Number>(list[1])));
				}
			};
			struct Rem : public FI
			{
			private:
				S sign() const {
					return S({ T_Number, T_Number }, T_Number); // (N N) -> N
				}
				ObjectPtr func(const DataList &list) const {
					return ObjectPtr(getPointer<Number>(list[0])->rem(getPointer<Number>(list[1])));
				}
			};

			struct Inc : public FI
			{
			private:
				S sign() const {
					return S({ T_Identifier }, T_Identifier); // I -> I
				}
				ObjectPtr func(const DataList &list) const {
					autoptr<Number> tmp(new Number(1));
					getPointer<Number>(adjustObjectPtr(list[0]))->add(tmp.get());
					return list[0];
				}
			};
			struct Dec : public FI
			{
			private:
				S sign() const {
					return S({ T_Identifier }, T_Identifier); // I -> I
				}
				ObjectPtr func(const DataList &list) const {
					autoptr<Number> tmp(new Number(1));
					getPointer<Number>(adjustObjectPtr(list[0]))->sub(tmp.get());
					return list[0];
				}
			};
		}

		namespace Comp
		{
			struct NumComp : public FI
			{
			public:
				using N = Number;
				using Func = std::function<bool(N*, N*)>;
				NumComp(const Func &fp) : fp(fp) {}

			private:
				S sign() const {
					return S({ T_Number, T_Number }, T_Boolean); // (N N) -> Bool
				}
				ObjectPtr func(const DataList &list) const {
					bool result = fp(getPointer<N>(list[0]), getPointer<N>(list[1]));
					return ObjectPtr(new Boolean(result));
				}
				Func fp;
			};
			struct NumSmallS : public NumComp { NumSmallS() : NumComp([](N *n1, N *n2) { return *n1 < *n2; }) {} };
			struct NumSmallE : public NumComp { NumSmallE() : NumComp([](N *n1, N *n2) { return *n1 <= *n2; }) {} };
			struct NumLargeL : public NumComp { NumLargeL() : NumComp([](N *n1, N *n2) { return *n1 > *n2; }) {} };
			struct NumLargeE : public NumComp { NumLargeE() : NumComp([](N *n1, N *n2) { return *n1 >= *n2; }) {} };

			struct Equ : public FI
			{
			private:
				S sign() const {
					return S({ T_Vary, T_Vary }, T_Boolean); // (Var Var) -> Boolean
				}
				ObjectPtr func(const DataList &list) const {
					return ObjectPtr(adjustObjectPtr(list[0])->equ(adjustObjectPtr(list[1])));
				}
			};
		}

		namespace Assign
		{
			struct Let : public FI
			{
			private:
				S sign() const {
					return S({ T_Identifier, T_Vary }, T_Identifier); // (I Var) -> I
				}
				ObjectPtr func(const DataList &list) const {
					Identifier *data = getPointer<Identifier>(list[0]);
					data->setValue(new ASTNode(list[1]));
					string name = getPointer<Identifier>(list[0])->getName();
					if (!AddVariableTable.find(name))
						AddVariableTable.add(name, data);
					return ObjectPtr(data);
				}
			};
			struct Cpy : public FI
			{
			private:
				S sign() const {
					return S({ T_Identifier, T_Vary }, T_Identifier); // (I Var) -> I
				}
				ObjectPtr func(const DataList &list) const {
					Identifier *data = getPointer<Identifier>(list[0]);
					data->setValue(new ASTNode(ObjectPtr(list[1]->clone())));
					string name = getPointer<Identifier>(list[0])->getName();
					if (!AddVariableTable.find(name))
						AddVariableTable.add(name, data);
					return ObjectPtr(data);
				}
			};
		}

		namespace Lists
		{
			using L = List;

			ObjectPtr list(const DataList &dl) {
				return ObjectPtr(new List(dl));
			}

			const DataList& _disp(const List *l) {
				return l->get_data();
			}

			ObjectPtr disp(const DataList &dl) {
				return ObjectPtr(new Disperse(_disp(getPointer<List>(dl[0]))));
			}

			ObjectPtr sort(const DataList &dl) {
				List *list = getPointer<List>(dl[0]);
				std::sort(list->begin(), list->end(), [](const ObjectPtr &a, const ObjectPtr &b) {
					return getPointer<Number>(a)->operator<(*getPointer<Number>(b));
				});
				return ObjectPtr(list);
			}
			// TODO
			ObjectPtr sort_f(const DataList &dl) {
				string name = getPointer<Identifier>(dl[1])->getName();

				List *list = getPointer<List>(dl[0]);
				std::sort(list->begin(), list->end(), [&](const ObjectPtr &a, const ObjectPtr &b) -> bool {
					return (bool)*getPointer<Boolean>(callDefFunc(name, DataList({ a, b })));
				});
				return ObjectPtr(list);
			}
		}

		namespace System
		{
			ObjectPtr print(const DataList &dl) {
				for (auto &op : dl)
					Common::Output::print(op->to_output());
				return Lists::list(dl);
			}
			ObjectPtr println(const DataList &dl) {
				for (auto &op : dl)
					Common::Output::print(op->to_output());
				Common::Output::println();
				return Lists::list(dl);
			}
			ObjectPtr dcall(const DataList &dl) {
				string name = getPointer<Identifier>(dl[1])->getName();
				return callDefFunc(name, DataList({ dl[0], dl[2] }));
			}
			ObjectPtr system(const DataList &dl) {
				int i = std::system(getPointer<Objects::String>(dl[0])->get_data().c_str());
				return ObjectPtr(new Number(i));
			}
			ObjectPtr exit(const DataList &dl) {
				std::exit(0);
				return nullptr;
			}
		}
	}

	void addDefFuncs(FuncTable &DefFuncTable)
	{
		using namespace ICM::DefFunc;
		using Lst = std::initializer_list<F>;
		using LST = std::initializer_list<FI*>;

		DefFuncTable.add("+", LST{
			new Calc::Add<void>(),
			new Calc::Add<Number>(),
			new Calc::Add<String>(),
			new Calc::Add<List>(),
		});
		DefFuncTable.add("-", LST{ new Calc::Sub() });
		DefFuncTable.add("*", LST{ new Calc::Mul() });
		DefFuncTable.add("/", LST{ new Calc::Div() });
		DefFuncTable.add("mod", LST{ new Calc::Mod() });
		DefFuncTable.add("rem", LST{ new Calc::Rem() });

		DefFuncTable.add("=", LST{ new Comp::Equ() });
		DefFuncTable.add("<", LST{ new Comp::NumSmallS() });
		DefFuncTable.add("<=", LST{ new Comp::NumSmallE() });
		DefFuncTable.add(">", LST{ new Comp::NumLargeL() });
		DefFuncTable.add(">=", LST{ new Comp::NumLargeE() });

		DefFuncTable.add("inc", LST{ new Calc::Inc() });
		DefFuncTable.add("dec", LST{ new Calc::Dec() });
		DefFuncTable.add("++", "inc");
		DefFuncTable.add("--", "dec");

		DefFuncTable.add("let", LST{ new Assign::Let() });
		DefFuncTable.add("cpy", LST{ new Assign::Cpy() });

		DefFuncTable.add("list", Lst{
			F(Lists::list, S({}, T_List)),               // Void -> L
			F(Lists::list, S({ T_Vary }, T_List, true)), // Var* -> L
		});
		DefFuncTable.add("disp", Lst{
			F(Lists::disp, S({ T_List }, T_Disperse)),       // L -> D
		});
		DefFuncTable.add("sort", Lst{
			F(Lists::sort, S({ T_List }, T_List)), // L -> L
			F(Lists::sort_f, S({ T_List, T_Function }, T_List)), // (L F) -> L
		});
		DefFuncTable.add("print", Lst{
			F(System::print, S({}, T_List)),               // Void -> L
			F(System::print, S({ T_Vary }, T_List, true)), // Var* -> L
		});
		DefFuncTable.add("println", Lst{
			F(System::println, S({}, T_List)),               // Void -> L
			F(System::println, S({ T_Vary }, T_List, true)), // Var* -> L
		});
		DefFuncTable.add("dcall", Lst{
			F(System::dcall, S({ T_Vary, T_Function, T_Vary }, T_Vary)), // (Var F Var) -> Var
		});
		DefFuncTable.add("exit", Lst{
			F(System::exit, S({}, T_Nil)), // Void -> Nil
		});
		DefFuncTable.add("system", Lst{
			F(System::system, S({ T_String }, T_Number)), // S -> N
		});
	}
}
