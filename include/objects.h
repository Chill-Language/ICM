#ifndef _ICM_OBJECTS_H_
#define _ICM_OBJECTS_H_

#include <string>
#include <vector>
#include "type.h"
#include "memory.h"
#include "charptr.h"
#include "prints.h"

namespace ICM
{
	namespace Objects
	{
		template <typename T> using DataList = std::vector<T>;

		class Object
		{
		public:
			virtual std::string to_string() const {
				return "Object";
			}
			virtual DefaultType get_type() const {
				return T_Nil;
			}
		};

		class Number : public Object
		{
		public:
			explicit Number(int dat = 0) : data(dat) {}
			Number& add(const Number &b) {
				self.data += b.data;
				return self;
			}
			Number& sub(const Number &b) {
				self.data -= b.data;
				return self;
			}
			std::string to_string() const {
				return std::to_string(data);
			}
			DefaultType get_type() const {
				return T_Number;
			}

		private:
			int data;
		};

		class String : public Object
		{
		public:
			explicit String(const std::string &dat = "") : data(dat) {}
			String(const String &dat) : data(dat.data) {}
			String& add(const String &b) {
				data = Common::charptr(self.data.to_string() + b.data.to_string());
				return self;
			}
			std::string to_string() const {
				return '"' + data.to_string() + '"';
			}
			std::string get_data() const {
				return data.to_string();
			}
			DefaultType get_type() const {
				return T_String;
			}

		private:
			Common::charptr data;
		};

		// Default Function
		template <typename T>
		T sum(const DataList<T> &list) {
			T result;
			for (auto &l : list)
				result.add(l);
			return result;
		}
		template <typename T>
		void print(const T &t);

		std::string to_string(const Object &obj);
	}
}

#endif