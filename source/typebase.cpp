#include "typebase.h"
#include "objectdef.h"

namespace ICM
{
	namespace TypeBase
	{
		//=======================================
		// * Class List
		//=======================================
		ListType* ListType::push(const ObjectPtr &op) {
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
				str.append(op.to_output());
			return str;
		}

		//=======================================
		// * Class Identifier
		//=======================================

		void IdentifierType::setData(const ObjectPtr &op) {
			if (op->type == T_Identifier)
				data = op->get<T_Identifier>()->getData();
			else
				data = op;
		}
		void IdentifierType::setCopy(const ObjectPtr &op) {
			if (op.isType(T_Identifier))
				setCopy(op->get<T_Identifier>()->getData());
			else
				data = ObjectPtr(op->clone());
		}
		void IdentifierType::setRefer(const ObjectPtr &op) {
			if (op->type == T_Identifier) {
				const ObjectPtr &sop = op->get<T_Identifier>()->getData();
				const ObjectPtr &refop = sop.isType(T_Identifier) ? sop : op;
				const ObjectPtr &refopdata = refop->get<T_Identifier>()->getData();
				if (data.get() != refopdata.get())
					data = refop;
				else
					data = refopdata;
			}
			else
				data = op;
		}

		const ObjectPtr& IdentifierType::getRealData() const {
			if (data.isType(T_Identifier))
				return data->get<T_Identifier>()->getData();
			else
				return data;
		}

		/*ObjectPtr GetElt(const ObjectPtr &op) {
		if (op.isType(T_Variable))
		return GetElt(op.get<Variable>()->getData());
		else if (op.isType(T_Reference))
		return GetElt(op.get<Reference>()->getData());
		else
		return op;
		}
		ObjectPtr GetRef(const ObjectPtr &op) {
		if (op.isType(T_Variable))
		return ObjectPtr(new Reference("R", op));
		if (op.isType(T_Reference))
		return op;
		else
		return ObjectPtr(new Reference("R", op));
		}*/
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
		template <> string to_string<IdentifierType>(const IdentifierType &ft) {
			return ft.to_string();
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
		template <> string to_output<IdentifierType>(const IdentifierType &it) {
			return it.to_output();
		}
		template <> string to_string_code<FunctionType>(const FunctionType &ft) {
			return ft.to_string_code();
		}
		template <> string to_string_code<IdentifierType>(const IdentifierType &it) {
			return it.to_string_code();
		}
	}
}
