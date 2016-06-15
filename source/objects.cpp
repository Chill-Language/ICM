#include "objects.h"

namespace ICM
{
	namespace Objects
	{
		template <>
		void print(const String &s) {
			Common::print(s.get_data());
		}
		std::string to_string(const Object &obj) {
			return obj.to_string();
		}
	}
}