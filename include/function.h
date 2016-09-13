#ifndef _ICM_FUNCTION_H_
#define _ICM_FUNCTION_H_

#include "ast.h"
#include "objects.h"
#include "lightlist.h"
#include "typeobject.h"

namespace ICM
{
	//=======================================
	// * Namespace Function
	//=======================================
	namespace Function
	{
		//=======================================
		// * Class Signature
		//=======================================
		class Signature
		{
		public:
			using List = lightlist<shared_ptr<TypeObject>>;
			using InitList1 = std::initializer_list<TypeObject>;

			Signature() = default;
			Signature(const InitList1 &intype, const TypeObject& outtype, bool last_is_args = false);

			const List& getInType() const { return InType; }
			const TypeObject& getOutType() const { return *OutType; }
			bool isLastArgs() const { return last_is_args; }

			bool checkType(const DataList &list, DataList *dlp = nullptr) const;
			bool checkType(const Signature &sign) const;
			bool checkType(const vector<TypeObject> &argT) const;

			string to_string() const;

		private:
			List InType;
			shared_ptr<TypeObject> OutType;
			bool last_is_args;
		};

		//=======================================
		// * Class FuncObject
		//=======================================
		class FuncObject
		{
		public:
			FuncObject() = default;
			FuncObject(const FuncPtr &func, const Signature &sign, const LFuncPtr &Lfunc = nullptr)
				: func(func), sign(sign), Lfunc(Lfunc) {}

			template <typename... Args>
			bool checkType(Args... args) const {
				return sign.checkType(args...);
			}
			ObjectPtr call(const DataList &dl) const {
				// If there was a crash, make sure that the pointer 'func' is effective.
				return func(dl);
			}
			void callL(Objects::Object* &ro, const LDataList &l) const {
				Lfunc(ro, l);
			}

			string to_string() const {
				return sign.to_string();
			}
			const Signature& getSign() const {
				return sign;
			}

		private:
			FuncPtr func;
			Signature sign;
			LFuncPtr Lfunc;
		};

		//=======================================
		// * Class FuncInitObject
		//=======================================
		class FuncInitObject
		{
			using LFP = Objects::Object*(const Common::lightlist<Objects::Object*> &); // TODO
		public:
			FuncObject get_f() {
				const auto &f1 = std::bind(&FuncInitObject::func, this, std::placeholders::_1);
				const auto &f2 = std::bind(&FuncInitObject::funcL, this, std::placeholders::_1, std::placeholders::_2);
				return FuncObject(f1, sign(), f2);
			}

		protected:
			virtual Signature sign() const = 0;
			virtual ObjectPtr func(const DataList &list) const = 0;
			virtual void funcL(Objects::Object* &, const LDataList &) const {
				println("Error : Not find funcL.");
			}
		};
	}

	//=======================================
	// * Functions
	//=======================================
	size_t getCallID(const FuncTableUnit &ftu, const DataList &dl);
	ObjectPtr checkCall(const ICM::FuncTableUnit &ftb, const ICM::DataList &dl);
}

#endif
