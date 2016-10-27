#ifndef _ICM_TYPEBASE_H_
#define _ICM_TYPEBASE_H_

#include "basic.h"
#include "object.h"
#include "number.h"
#include "keyword.h"
#include "tabledata.h"

namespace ICM
{
	namespace TypeBase
	{
		template <typename T>
		string to_string(const T &t) {
			using Common::Convert::to_string;
			using std::to_string;
			using ICM::to_string;
			return to_string(t);
		}
		template <typename T>
		string to_output(const T &t) {
			return to_string<T>(t);
		}
		template <typename T>
		string to_string_code(const T &t) {
			return to_string<T>(t);
		}

		class VoidType {
		public:
			bool operator==(const VoidType&) const { return true; }
		};
		using NumberType = Common::Number::Rational;
		using BooleanType = bool;
		using StringType = std::string;
		using SymbolType = std::string;
		using KeywordType = KeywordID;


		//=======================================
		// * Class List
		//=======================================
		class ListType
		{
			using DataList = vector<ObjectPtr>;
		public:
			ListType() {}
			explicit ListType(const lightlist<ObjectPtr> &dl) : data(dl.begin(), dl.end()) {}
			explicit ListType(const DataList &dl) : data(dl) {}
			ListType(const ListType &lt) : data(lt.data) {}

			ListType* push(const ObjectPtr &objp);
			ListType* push(const DataList &dl);
			ListType& operator+=(const ListType &dl);
			DataList::iterator begin() {
				return data.begin();
			}
			DataList::iterator end() {
				return data.end();
			}
			const DataList& getData() const {
				return data;
			}
			size_t size() const {
				return data.size();
			}
			// Method
			bool operator==(const ListType &lt) const {
				return this->data == lt.data;
			}
			string to_string() const;
			string to_output() const;

		private:
			DataList data;
		};
		template <> string to_string<ListType>(const ListType &lt);
		template <> string to_output<ListType>(const ListType &lt);

		//=======================================
		// * Class Disperse
		//=======================================
		class DisperseType
		{
			using DataList = vector<ObjectPtr>;
		public:
			DisperseType() {}
			explicit DisperseType(const lightlist<ObjectPtr> &dl) : data(dl.begin(), dl.end()) {}
			explicit DisperseType(const DataList &dl) : data(dl) {}

			DataList::iterator begin() {
				return data.begin();
			}
			DataList::iterator end() {
				return data.end();
			}
			const DataList& getData() const {
				return data;
			}
			// Method
			bool operator==(const DisperseType &lt) const {
				return this->data == lt.data;
			}
			string to_string() const;
			string to_output() const;

		private:
			DataList data;
		};
		template <> string to_string<DisperseType>(const DisperseType &dt);
		template <> string to_output<DisperseType>(const DisperseType &dt);

		//=======================================
		// * Class Error
		//=======================================
		class ErrorType
		{
		public:
			explicit ErrorType(std::string msg = "") : msg(msg) {}
			// Method
			string to_string() const {
				return "Error(" + msg + ")";
			}
			bool operator==(const ErrorType &et) const {
				return msg == et.msg;
			}

		private:
			std::string msg;
		};
		template <> string to_string<ErrorType>(const ErrorType &et);

		//=======================================
		// * Class Function
		//=======================================
		class FunctionType
		{
		public:
			FunctionType() {}
			FunctionType(size_t id) : index(id) {}
			const FuncTableUnit& getData() const {
				return DefFuncTable[index];
			}
			string to_string() const {
				return "F(" + getData().getName() + ")";
			}
			string to_output() const {
				return "F(" + getData().getName() + ")";
			}
			string to_string_code() const {
				return getData().getName();
			}
			bool operator==(const FunctionType &ft) const {
				return index == ft.index && subid == ft.subid;
			}

		public:
			size_t index;
			size_t subid = MaxValue<size_t>();
		};
		template <> string to_string<FunctionType>(const FunctionType &ft);
		template <> string to_output<FunctionType>(const FunctionType &ft);
		template <> string to_string_code<FunctionType>(const FunctionType &ft);

		//=======================================
		// * Class Identifier
		//=======================================
		class IdentifierType
		{
		public:
			explicit IdentifierType(const std::string &name = "") : name(name) {}
			IdentifierType(const std::string &name, const ObjectPtr &op) : name(name), data(op) {}
			std::string getName() const {
				return name/*.to_string()*/;
			}
			const ObjectPtr& getData() const {
				return data;
			}
			const ObjectPtr& getRealData() const;
			void setData(const ObjectPtr &op);
			void setCopy(const ObjectPtr &op);
			void setRefer(const ObjectPtr &op);
			DefaultType getValueType() const {
				return data.type();
			}
			bool operator==(const IdentifierType &it) const {
				return data == it.data;
			}
			// Method
			string to_string() const {
				return name/*.to_string()*/ + "(" + ICM::to_string(data) + ")";
			}
			string to_output() const {
				return data->to_output();
			}
			string to_string_code() const {
				return name/*.to_string()*/;
			}
			void write(File &file) const {
				file.write(data);
			}
			void read(File &file) {
				file.read(data);
			}

		private:
			string name;
			//Common::charptr name;
			ObjectPtr data;
		};
		template <> string to_string<IdentifierType>(const IdentifierType &it);
		template <> string to_output<IdentifierType>(const IdentifierType &it);
		template <> string to_string_code<IdentifierType>(const IdentifierType &it);
	}
}

#endif
