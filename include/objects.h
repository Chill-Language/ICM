#ifndef _ICM_OBJECTS_H_
#define _ICM_OBJECTS_H_

#include "basic.h"

namespace ICM
{
	namespace Objects
	{
		//=======================================
		// * Declarations
		//=======================================
		class Object;
		class Error;
		class Nil;
		class Boolean;
		class Number;
		class String;
		class List;
		class Identifier;
	}

	// Types
	using ObjectPtr = autoptr<Objects::Object>;
	using DataList = std::vector<ObjectPtr>;
	using CallFunc = ObjectPtr(const DataList&);
	using FuncPtr = std::function<CallFunc>;
	// Functions
	template <typename T>
	T* getPointer(const ObjectPtr &op) {
		return static_cast<T*>(op.get());
	}
	ObjectPtr createError(const string &errinfo);
	std::string to_string(const Objects::Object &obj);

	namespace Objects
	{
		//=======================================
		// * Class Object
		//=======================================
		class Object
		{
		public:
			virtual ~Object() {}
			virtual string to_string() const {
				return "Object";
			}
			virtual string to_output() const {
				return to_string();
			}
			virtual DefaultType get_type() const {
				return T_Object;
			}
			virtual Object* clone() const {
				return new Object(*this);
			}
			// Method
			virtual Boolean* equ(const ObjectPtr &obj) const;

		protected:
			bool type_equal(const ObjectPtr &obj) const {
				return this->get_type() == obj->get_type();
			}
		};
	}
}

#endif
