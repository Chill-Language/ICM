#include "deffunc.h"
#include "keyword.h"
#include "objectsdef.h"
#include "config.h"

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
		using F = ICM::Function::FuncObject;
		using FI = ICM::Function::FuncInitObject;
		using S = ICM::Function::Signature;
		using T = ICM::TypeObject;

		using ObjectLightList = Common::lightlist<Objects::Object*>;

		//=======================================
		// * Calculate
		//=======================================
		namespace Calc
		{
			template <typename T>
			struct Add : public FI
			{
			private:
				S sign() const {
					DefaultType t = T::Type;
					return S({ t }, t, true); // T* -> T
				}
				ObjectPtr func(const DataList &list) const {
					T *tmp = list.front().get<T>()->clone();
					for (auto i : Range<size_t>(1, list.size()))
						tmp->getData().operator+=(list[i].get<T>()->getData());
					return ObjectPtr(tmp);
				}
				void funcL(Objects::Object &result, const LDataList &list) const {
					T *tmp = static_cast<T*>(list.front()->clone());
					for (auto i : Range<size_t>(1, list.size()))
						tmp->getData().operator+=(static_cast<T*>(list[i])->getData());
					result = *tmp;
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
					return ObjectPtr(new Number(Number::VType(0)));
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
					N *tmp = list.front().get<N>()->clone();
					for (auto i : Range<size_t>(1, list.size()))
						fp(tmp, list[i].get<N>());
					return ObjectPtr(tmp);
				}
				Func fp;
			};
			struct Sub : public NumCall { Sub() : NumCall([](N *num, N *obj) { num->getData() -= obj->getData(); }) {} };
			struct Mul : public NumCall { Mul() : NumCall([](N *num, N *obj) { num->getData() *= obj->getData(); }) {} };
			struct Div : public NumCall { Div() : NumCall([](N *num, N *obj) { num->getData() /= obj->getData(); }) {} };

			struct Mod : public FI
			{
			private:
				S sign() const {
					return S({ T_Number, T_Number }, T_Number); // (N N) -> N
				}
				ObjectPtr func(const DataList &list) const {
					auto &r1 = list[0].get<Number>()->getData();
					auto &r2 = list[1].get<Number>()->getData();
					auto &rr = Common::Number::mod(r1, r2);
					return ObjectPtr(new Number(rr));
				}
			};
			struct Rem : public FI
			{
			private:
				S sign() const {
					return S({ T_Number, T_Number }, T_Number); // (N N) -> N
				}
				ObjectPtr func(const DataList &list) const {
					auto &r1 = list[0].get<Number>()->getData();
					auto &r2 = list[1].get<Number>()->getData();
					auto &rr = Common::Number::rem(r1, r2);
					return ObjectPtr(new Number(rr));
				}
			};

			struct Inc : public FI
			{
			private:
				S sign() const {
					return S({ T_Number }, T_Number); // N -> N
				}
				ObjectPtr func(const DataList &list) const {
					auto &r1 = list[0].get<Number>()->getData();
					r1.operator+=(1);
					return list[0];
				}
			};
			struct Dec : public FI
			{
			private:
				S sign() const {
					return S({ T_Number }, T_Number); // N -> N
				}
				ObjectPtr func(const DataList &list) const {
					auto &r1 = list[0].get<Number>()->getData();
					r1.operator-=(1);
					return list[0];
				}
			};
		}

		//=======================================
		// * Compare
		//=======================================
		namespace Comp
		{
			struct NumComp : public FI
			{
			public:
				using N = Number;
				using Func = std::function<bool(const N::VType&, const N::VType&)>;
				NumComp(const Func &fp) : fp(fp) {}

			private:
				S sign() const {
					return S({ T_Number, T_Number }, T_Boolean); // (N N) -> Bool
				}
				ObjectPtr func(const DataList &list) const {
					bool result = fp(list[0].get<N>()->getData(), list[1].get<N>()->getData());
					return ObjectPtr(new Boolean(result));
				}
				void funcL(Objects::Object* &result, const LDataList &list) const {
					funcB(result, list[0], list[1]);
				}
				void funcB(Objects::Object* &result, Objects::Object *a, Objects::Object *b) const {
					bool r = fp(static_cast<N*>(a)->getData(), static_cast<N*>(b)->getData());
					result = new Boolean(r);
				}
				Func fp;
			};
			struct NumSmallS : public NumComp { NumSmallS() : NumComp([](const N::VType &n1, const N::VType &n2) { return n1 < n2; }) {} };
			struct NumSmallE : public NumComp { NumSmallE() : NumComp([](const N::VType &n1, const N::VType &n2) { return n1 <= n2; }) {} };
			struct NumLargeL : public NumComp { NumLargeL() : NumComp([](const N::VType &n1, const N::VType &n2) { return n1 > n2; }) {} };
			struct NumLargeE : public NumComp { NumLargeE() : NumComp([](const N::VType &n1, const N::VType &n2) { return n1 >= n2; }) {} };

			struct Equ : public FI
			{
			private:
				S sign() const {
					return S({ T_Vary, T_Vary }, T_Boolean); // (Var Var) -> Boolean
				}
				ObjectPtr func(const DataList &list) const {
					auto &nn1 = adjustObjectPtr(list[0]);
					auto &nn2 = adjustObjectPtr(list[1]);
					return ObjectPtr(new Boolean(nn1->equ(nn2)));
				}
			};
		}
		/*
		//=======================================
		// * Assign
		//=======================================
		namespace Assign
		{
			class Basic : public FI
			{
			private:
				S sign() const {
					return S({ T_Identifier, T_Vary }, T_Identifier); // (I Var) -> I
				}
				ObjectPtr func(const DataList &list) const {
					Identifier *data = list[0].get<Identifier>();
					setData(data, list[1]);
					string name = list[0].get<Identifier>()->getName();
					if (!AddVariableTable.find(name))
						AddVariableTable.add(name, ObjectPtr(data));
					return ObjectPtr(data);
				}
				virtual void setData(Identifier *data, const ObjectPtr &op) const = 0;
			};
			struct Let : public Basic
			{
			private:
				void setData(Identifier *data, const ObjectPtr &op) const {
					data->setData(op);
				}
			};
			struct Cpy : public Basic
			{
			private:
				void setData(Identifier *data, const ObjectPtr &op) const {
					data->setCopy(op);
				}
			};
			struct Ref : public Basic
			{
			private:
				void setData(Identifier *data, const ObjectPtr &op) const {
					data->setRefer(op);
				}
			};
			struct CpyV : public FI
			{
			private:
				S sign() const {
					return S({ T_Vary }, T_Vary); // Var -> Var
				}
				ObjectPtr func(const DataList &list) const {
					Identifier temp;
					temp.setCopy(list[0]);
					return temp.getData();
				}
			};
		}
		*/
		//=======================================
		// * Lists
		//=======================================
		namespace Lists
		{
			using L = List;

			ObjectPtr list(const DataList &dl) {
				return ObjectPtr(new List(ListType(dl)));
			}

			const vector<ObjectPtr>& _disp(const List *l) {
				return l->getData().getData();
			}

			ObjectPtr disp(const DataList &dl) {
				return ObjectPtr(new Disperse(DisperseType(_disp(dl[0].get<List>()))));
			}

			ObjectPtr sort(const DataList &dl) {
				List *list = dl[0].get<List>();
				std::sort(list->getData().begin(), list->getData().end(), [](const ObjectPtr &a, const ObjectPtr &b) {
					return a.get<Number>()->getData() < b.get<Number>()->getData();
				});
				return ObjectPtr(list);
			}
			// TODO
			ObjectPtr sort_f(const DataList &dl) {
				auto &func = dl[1].get<Objects::Function>()->getData().get_data();
				// TODO
				size_t id = getCallID(func, DataList({ ObjectPtr(new Number(Number::VType(0))), ObjectPtr(new Number(Number::VType(0))) }));
				const auto &rf = func[id];

				List *list = dl[0].get<List>();
				Objects::Object *r;
				std::sort(list->getData().begin(), list->getData().end(), [&](const ObjectPtr &a, const ObjectPtr &b) -> bool {
					rf.callL(r, { a.get(), b.get() });
					bool rr = static_cast<Boolean*>(r)->getData();
					delete r;
					return rr;
				});
				return ObjectPtr(list);
			}

			class Mul : public FI
			{
				S sign() const {
					return S({ T_List, T_Number }, T_List); // (L N) -> L
				}
				ObjectPtr func(const DataList &list) const {
					auto &l = list[0].get<List>()->getData();
					vector<ObjectPtr> nl;
					for (size_t i : Range<size_t>(0, (size_t)list[1].get<Number>()->getData().getNum())) {
						nl.insert(nl.end(), l.begin(), l.end());
					}
					return ObjectPtr(new List(ListType(nl)));
				}
			};

			struct Foreach : public FI
			{
			private:
				S sign() const {
					return S({ T(T_Function,S({T_Vary},T_Vary,true)), T_List }, T_List, true); // (F(V*->V) L*) -> L
				}
				ObjectPtr func(const DataList &list) const {
					// TODO
					auto &func = list[0].get<Objects::Function>()->getData().get_data();
					size_t id = getCallID(func, DataList({ ObjectPtr(new Number(Number::VType(0))) }));
					const auto &rf = func[id];

					size_t minsize;
					size_t size = list.size();
					for (auto i : range(1, size))
						minsize = std::min(minsize, list[i].get<List>()->getData().size());
					vector<ObjectPtr> dls;
					Objects::Object obj;
					for (size_t i : range(0, minsize)) {
						DataList ldl(size - 1);
						for (auto id : range(1, size))
							ldl[id - 1] = list[id].get<List>()->getData().getData()[i];
						dls.push_back(rf.call(ldl));
					}
					List *result = new List(ListType(dls));
					return ObjectPtr(result);
				}
			};

			struct Size : public FI
			{
			private:
				S sign() const {
					return S({ T_List }, T_Number); // L -> N
				}
				ObjectPtr func(const DataList &list) const {
					size_t s = list[0].get<List>()->getData().getData().size();
					return ObjectPtr(new Number(Number::VType(s)));
				}
			};

			struct Swap : public FI
			{
			private:
				S sign() const {
					return S({ T_List, T_Number, T_Number }, T_List); // (L N N) -> L
				}
				ObjectPtr func(const DataList &list) const {
					auto &v = const_cast<vector<ObjectPtr>&>(list[0].get<List>()->getData().getData());
					size_t i1 = (size_t)list[1].get<Number>()->getData().getNum();
					size_t i2 = (size_t)list[2].get<Number>()->getData().getNum();
					ObjectPtr top = v[i1];
					v[i1] = v[i2];
					v[i2] = top;
					return list[0];
				}
			};

			struct At : public FI
			{
			private:
				S sign() const {
					return S({ T_List, T_Number }, T_Vary); // (L N) -> V
				}
				ObjectPtr func(const DataList &list) const {
					const ObjectPtr &op = list[0].get<List>()->getData().getData()[(size_t)list[1].get<Number>()->getData().getNum()];
					return op;
				}
			};

			struct Set : public FI
			{
			private:
				S sign() const {
					return S({ T_List, T_Number, T_Vary }, T_Vary); // (L N V) -> V
				}
				ObjectPtr func(const DataList &list) const {
					const ObjectPtr &op = list[0].get<List>()->getData().getData()[(size_t)list[1].get<Number>()->getData().getNum()];
					ObjectPtr &rop = const_cast<ObjectPtr&>(op);
					rop = list[2];
					return rop;
				}
			};
		}

		//=======================================
		// * System
		//=======================================
		namespace System
		{
			ObjectPtr call(const DataList &dl) {
				return checkCall(dl[0].get<Objects::Function>()->getData().get_data(), DataList(dl.begin() + 1, dl.end()));
			}
			ObjectPtr p(const DataList &dl) {
				for (auto &op : dl)
					Common::Output::println(op.to_string());
				return Lists::list(dl);
			}
			ObjectPtr print(const DataList &dl) {
				for (auto &op : dl)
					Common::Output::print(op->to_output());
				return Lists::list(dl);
			}
			ObjectPtr println(const DataList &dl) {
				print(dl);
				Common::Output::println();
				return Lists::list(dl);
			}
			ObjectPtr dcall(const DataList &dl) {
				auto &ftu = dl[1].get<Objects::Function>()->getData().get_data();
				return checkCall(ftu, DataList({ dl[0], dl[2] }));
			}
			ObjectPtr system(const DataList &dl) {
				int i = std::system(dl[0].get<Objects::String>()->getData().c_str());
				return ObjectPtr(new Number(Number::VType(i)));
			}
			ObjectPtr exit(const DataList &dl) {
				std::exit(0);
				return ObjectPtr(new Nil());
			}
			ObjectPtr exitv(const DataList &dl) {
				// TODO
				std::exit((int)dl[0].get<Number>()->getData().getNum());
				return ObjectPtr(new Nil());
			}
		}

		//=======================================
		// * Config
		//=======================================
		namespace Config
		{
			struct SetBool : public FI
			{
			private:
				S sign() const {
					return S({ T_Boolean }, T_Nil); // Bool -> Nil
				}
				ObjectPtr func(const DataList &list) const {
					bool value = list[0].get<Boolean>()->getData();
					setValue(value);
					return ObjectPtr(new Nil());
				}
				virtual void setValue(bool value) const = 0;
			};
			struct DebugMode : public SetBool
			{
			private:
				virtual void setValue(bool value) const {
					GlobalConfig.SetDebugMode(value);
				}
			};
			struct PrintIntervalTime : public SetBool
			{
			private:
				virtual void setValue(bool value) const {
					GlobalConfig.PrintIntervalTime = value;
				}
			};
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
		DefFuncTable.add("*", LST{ new Calc::Mul(), new Lists::Mul() });
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

		//DefFuncTable.add("let", LST{ new Assign::Let() });
		//DefFuncTable.add("cpy", LST{ new Assign::Cpy(), new Assign::CpyV() });
		//DefFuncTable.add("ref", LST{ new Assign::Ref() });

		DefFuncTable.add("list", Lst{
			F(Lists::list, S({}, T_List)),               // Void -> L
			F(Lists::list, S({ T_Vary }, T_List, true)), // Var* -> L
		});
		DefFuncTable.add("disp", Lst{
			F(Lists::disp, S({ T_List }, T_Disperse)),       // L -> D
		});
		DefFuncTable.add("sort", Lst{
			F(Lists::sort, S({ T_List }, T_List)), // L -> L
			F(Lists::sort_f, S({ T_List, T(T_Function,S({T_Number,T_Number},T_Number)) }, T_List)), // (L F) -> L
		});
		DefFuncTable.add("foreach", LST{ new Lists::Foreach() });
		DefFuncTable.add("size", LST{ new Lists::Size() });
		DefFuncTable.add("swap", LST{ new Lists::Swap() });
		DefFuncTable.add("at", LST{ new Lists::At() });
		DefFuncTable.add("set", LST{ new Lists::Set() });
		DefFuncTable.add("call", Lst{
			F(System::call, S({ T_Function }, T_Vary)),    // F -> V
			F(System::call, S({ T_Function, T_Vary }, T_Vary, true)),    // (F V*) -> V
		});
		DefFuncTable.add("print", Lst{
			F(System::print, S({}, T_List)),               // Void -> L
			F(System::print, S({ T_Vary }, T_List, true)), // Var* -> L
		});
		DefFuncTable.add("println", Lst{
			F(System::println, S({}, T_List)),               // Void -> L
			F(System::println, S({ T_Vary }, T_List, true)), // Var* -> L
		});
		//DefFuncTable.add("p", Lst{
		//	F(System::p, S({}, T_List)),               // Void -> L
		//	F(System::p, S({ T_Vary }, T_List, true)), // Var* -> L
		//});
		DefFuncTable.add("dcall", Lst{
			F(System::dcall, S({ T_Vary, T_Function, T_Vary }, T_Vary)), // (Var F Var) -> Var
		});
		DefFuncTable.add("exit", Lst{
			F(System::exit, S({}, T_Nil)),            // Void -> N
			F(System::exitv, S({ T_Number }, T_Nil)), // N -> N
		});
		DefFuncTable.add("system", Lst{
			F(System::system, S({ T_String }, T_Number)), // S -> N
		});
		DefFuncTable.add("Config.SetDebugMode", LST{ new DefFunc::Config::DebugMode() });
		DefFuncTable.add("Config.SetPrintIntervalTime", LST{ new DefFunc::Config::PrintIntervalTime() });
	}
}
