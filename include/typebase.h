#pragma once
#include "basic.h"
#include "object.h"
#include "number.h"
#include "parser/keyword.h"
#include "tabledata.h"
#include "parser/literal.h"

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

		using NumberType = Compiler::Literal::Number;
		using BooleanType = Compiler::Literal::Boolean;
		using StringType = Compiler::Literal::String;
		using SymbolType = std::string;
		using KeywordType = ICM::Keyword::KeywordID;

		//=======================================
		// * Class TypeType
		//=======================================
		class TypeType
		{
		public:
			TypeType(TypeUnit type = T_Null) : data(type) {}
			string to_string() const {
				return TypeInfoTable.at(data).name;
			}
			string to_output() const {
				return TypeInfoTable.at(data).name;
			}
			bool operator==(const TypeType &tt) const {
				return data == tt.data;
			}
			TypeUnit get() const {
				return data;
			}

		private:
			TypeUnit data;
		};
		template <> string to_string<TypeType>(const TypeType &lt);
		template <> string to_output<TypeType>(const TypeType &lt);


		//=======================================
		// * Class ListType
		//=======================================
		class ListType
		{
			using DataList = vector<Object*>;
		public:
			ListType() {}
			explicit ListType(const lightlist<Object*> &dl) : data(dl.begin(), dl.end()) {}
			explicit ListType(const DataList &dl) : data(dl) {}
			ListType(const ListType &lt) : data(lt.data) {}

			ListType* push(Object* objp);
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
			size_t capacity() const {
				return data.capacity();
			}
			void reserve(size_t length) {
				data.reserve(length);
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
		// * Class DisperseType
		//=======================================
		class DisperseType
		{
			using DataList = vector<Object*>;
		public:
			DisperseType() {}
			explicit DisperseType(const lightlist<Object*> &dl) : data(dl.begin(), dl.end()) {}
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
		// * Struct FunctionType
		//=======================================
		struct FunctionType
		{
		public:
			FunctionType() {}
			FunctionType(size_t id) : index(id) {}
			const FuncTableUnit& getData() const {
				return GlobalFunctionTable[index];
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
	}
	string to_string(const TypeBase::FunctionType &ft);
}
