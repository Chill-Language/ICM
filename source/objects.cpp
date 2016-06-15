#include "objects.h"

namespace ICM
{
	namespace Objects
	{
		template <>
		void print(const String &s) {
			Common::print(s.get_data());
		}
	}
}