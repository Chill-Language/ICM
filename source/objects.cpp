#include "objects.h"

namespace ICM
{
	namespace Objects
	{
		template <>
		void print(const String &s) {
			Common::Output::print(s.get_data());
		}
		template <>
			void print(const Number &s) {
			Common::Output::print(s.to_string());
		}
		std::string to_string(const Object &obj) {
			return obj.to_string();
		}
	}
}