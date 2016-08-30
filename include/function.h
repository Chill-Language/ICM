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
			FuncObject(const FuncPtr &func, const Signature &sign)
				: func(func), sign(sign) {}

			template <typename... Args>
			bool checkType(Args... args) const {
				return sign.checkType(args...);
			}
			ObjectPtr call(const DataList &dl) const {
				// If there was a crash, make sure that the pointer 'func' is effective.
				return func(dl);
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
		};

		//=======================================
		// * Class FuncInitObject
		//=======================================
		class FuncInitObject
		{
		public:
			FuncObject get_f() {
				return FuncObject(std::bind((&FuncInitObject::func), this, std::placeholders::_1), sign());
			}

		protected:
			virtual Signature sign() const = 0;
			virtual ObjectPtr func(const DataList &list) const = 0;
		};
	}

	//=======================================
	// * Functions
	//=======================================
	size_t getCallID(const FuncTableUnit &ftu, const DataList &dl);
	ObjectPtr checkCall(const ICM::FuncTableUnit &ftb, const ICM::DataList &dl);
}

#endif
