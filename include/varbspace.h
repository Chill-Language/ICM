#pragma once
#include "basic.h"
#include "object.h"

namespace ICM
{
	namespace Runtime
	{
		class SpaceManager
		{
			void create(TypeUnit type) {
				TypeInfo &info = TypeInfoTable.at(type);
				info.alloc();
			}
			void free() {

			}
			void get() {

			}
			void set() {

			}
		};

		class VariableManager
		{
		public:
			VariableManager() {}
		private:
			class DyVarbManager
			{
				void create() {

				}
				void free() {

				}
				void get() {

				}
				void set() {

				}
			};

			class StVarbManager
			{
				void create() {

				}
				void free() {

				}
				void get() {

				}
				void set() {

				}
			};

		private:
			DyVarbManager DyVarb;
			StVarbManager StVarb;
		};
	}
}
