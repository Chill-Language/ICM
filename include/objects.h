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
		class Disperse;
		class Identifier;
		class Function;
		class TypeClass;
	}

	// Types
	using ObjectPtr = shared_ptr<Objects::Object>;
	using DataList = std::vector<ObjectPtr>;
	using CallFunc = ObjectPtr(const DataList&);
	using FuncPtr = std::function<CallFunc>;
	// Functions
	template <typename T>
	T* getPointer(const ObjectPtr &op) {
		return static_cast<T*>(op.get());
	}
	template <typename T, typename... Args>
	ObjectPtr createObject(Args... args) {
		return ObjectPtr(new T(args...));
	}
	ObjectPtr createError(const string &errinfo);
	ObjectPtr adjustObjectPtr(const ObjectPtr &ptr);
	std::string to_string(const Objects::Object &obj);
	DataList::iterator begin(Objects::Disperse *disp);
	DataList::iterator end(Objects::Disperse *disp);

	namespace Objects
	{
		//=======================================
		// * Class Object
		//=======================================
		class Object
		{
		public:
			virtual ~Object() {}
			virtual string to_string() const = 0;
			virtual string to_output() const {
				return to_string();
			}
			virtual DefaultType get_type() const = 0;
			virtual Object* clone() const = 0;
			// Method
			virtual Boolean* equ(const ObjectPtr &obj) const;
			// Const
			static const DefaultType Type = T_Object;

		protected:
			bool type_equal(const ObjectPtr &obj) const {
				return this->get_type() == obj->get_type();
			}
		};
	}
}

#endif
