#include "basic.h"
#include "runtime/typebase.h"
#include "runtime/objectdef.h"

namespace ICM
{
	namespace TypeBase
	{
		//=======================================
		// * Class List
		//=======================================
		ListType* ListType::push(Object* op) {
			data.push_back(op);
			return this;
		}
		ListType* ListType::push(const DataList &dl) {
			data.insert(data.end(), dl.begin(), dl.end());
			return this;
		}
		ListType& ListType::operator+=(const ListType &dl) {
			data.insert(data.end(), dl.data.begin(), dl.data.end());
			return *this;
		}
		string ListType::to_string() const {
			return ICM::to_string(data);
		}
		string ListType::to_output() const {
			return Common::Convert::to_string<'[', ']'>(data.begin(), data.end(), [](const ObjectPtr &op) { return op.to_output(); });
		}

		//=======================================
		// * Class Disperse
		//=======================================
		string DisperseType::to_string() const {
			return Convert::to_string(data.begin(), data.end(), [](const ObjectPtr &op) { return ICM::to_string(op); });
		}
		string DisperseType::to_output() const {
			string str;
			for (auto &op : data)
				str.append(op->to_output());
			return str;
		}

		template<> string to_string<TypeType>(const TypeType &tt) {
			return tt.to_string();
		}
		template <> string to_string<ListType>(const ListType &lt) {
			return lt.to_string();
		}
		template <> string to_string<DisperseType>(const DisperseType &lt) {
			return lt.to_string();
		}
		template <> string to_string<ErrorType>(const ErrorType &et) {
			return et.to_string();
		}
		template <> string to_string<FunctionType>(const FunctionType &ft) {
			return ft.to_string();
		}
		template<> string to_output<TypeType>(const TypeType &tt) {
			return tt.to_output();
		}
		template <> string to_output<ListType>(const ListType &lt) {
			return lt.to_output();
		}
		template <> string to_output<DisperseType>(const DisperseType &lt) {
			return lt.to_output();
		}
		template <> string to_output<FunctionType>(const FunctionType &ft) {
			return ft.to_output();
		}
		template <> string to_string_code<FunctionType>(const FunctionType &ft) {
			return ft.to_string_code();
		}
	}
}
