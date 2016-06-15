#ifndef _ICM_OBJECTS_H_
#define _ICM_OBJECTS_H_

#include <string>
#include <vector>
#include "type.h"
#include "memory.h"
#include "prints.h"
#define self (*this)

namespace ICM
{
	namespace Objects
	{
		template <typename T> using DataList = std::vector<T>;

		class Number
		{
		public:
			Number(int dat = 0) : data(dat) {}
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

		class String
		{
		public:
			String(const std::string &dat = "")
			{
				data = (Common::Memory::copyOf<char>(dat.c_str(), dat.length() + 1));
				//Common::println();
			}
			String(const String &dat)
			{
				data = (Common::Memory::copyOf<char>(dat.data, std::string(dat.data).length() + 1));
			}
			~String() {

				if (data)
					delete[] data;
			}
			String& add(const String &b) {
				std::string str = std::string(self.data) + std::string(b.data);
				data = (Common::Memory::copyOf<char>(str.c_str(), str.length() + 1));
				//self.data += b.data;
				return self;
			}
			std::string to_string() const {
				return '"' + std::string(data) + '"';
			}
			DefaultType get_type() const {
				return T_String;
			}

		private:
			char *data = nullptr;
		};

		// Default Function
		template <typename T>
		T sum(const DataList<T> &list) {
			T result;
			for (auto &l : list)
				result.add(l);
			return result;
		}
	}
}

#endif