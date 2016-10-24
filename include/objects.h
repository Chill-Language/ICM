#ifndef _ICM_OBJECTS_H_
#define _ICM_OBJECTS_H_

#include "basic.h"
#include "objectenum.h"
#include "objectmanager.h"

namespace ICM
{
	//===========================================================
	// * Namespace Types
	//===========================================================
	namespace Types
	{
		//===========================================================
		// * Template TType
		//===========================================================
		using TypeName = const char* const;
		template <TypeUnit> struct TType { using Type = void; static TypeName Name; };
	}
	template <TypeUnit T> using TType = Types::TType<T>;

	struct TypeInfo
	{
		using TosFunc = std::string(const void*);
		using EquFunc = bool(const void*, const void*);

		TypeUnit index;
		const char* name;
		size_t size;

		EquFunc* equal;

		TosFunc* to_string;
		TosFunc* to_output;
		TosFunc* to_string_code;

		void* alloc() const {
			//println("Call Alloc.");
			return std::malloc(size);
		}
		void* copy(const void *src) const {
			//println("Call Copy.");
			return std::memcpy(alloc(), src, size);
		}
	};
	extern map<TypeUnit, TypeInfo> TypeInfoTable;

	namespace Objects
	{
		//=============================================
		// * Struct Object
		//---------------------------------------------
		//   Object is a struct with a type & a data.
		//=============================================
		struct Object
		{
		public:
			Object(TypeUnit type = T_Null) : type(type) {}

			string to_string() const {
				if (TypeInfoTable.find(type) != TypeInfoTable.end())
					return TypeInfoTable[type].to_string(data);
				else
					println("Error info for type(", (DefaultType)type, ").");

				return "Object";
			}
			string to_output() const {
				if (TypeInfoTable.find(type) != TypeInfoTable.end())
					return TypeInfoTable[type].to_output(data);
				else
					println("Error info for type(", (DefaultType)type, ").");

				return "Object";
			}
			string to_string_code() const {
				if (TypeInfoTable.find(type) != TypeInfoTable.end())
					return TypeInfoTable[type].to_string_code(data);
				else
					println("Error info for type(", (DefaultType)type, ").");

				return "Object";
			}
			
			Object* clone() const {
				Object *p = new Object(type);
				if (data == nullptr)
					return p;
				if (TypeInfoTable.find(type) != TypeInfoTable.end())
					p->data = TypeInfoTable[type].copy(data);
				else
					println("Error size for type(", (DefaultType)type, ").");
				return p;
			}
			void set(const Object *op) {
				if (TypeInfoTable.find(type) != TypeInfoTable.end())
					this->data = TypeInfoTable[type].copy(op->data);
				else
					println("Error size for type(", (DefaultType)type, ").");
			}
			// Method
			bool equ(const Object *obj) const {
				if (TypeInfoTable.find(type) == TypeInfoTable.end())
					println("Error size for type(", (DefaultType)type, ").");
				return this == obj || (this->type == obj->type && TypeInfoTable[type].equal(this->data,obj->data));
			}
			// Const
			static const DefaultType Type = T_Object;

		// New Change (Building):
		public:
			//template <typename T> T* get() { return _ptr<T>(); }
			//template <typename T> const T* get() const { return _ptr<T>(); }

			TypeUnit type = T_Null;
			void *data = nullptr;
			// All data should be copyable, have no shared resource.

			// Methods for data
			template <typename T> T* _ptr() { return static_cast<T*>(data); }
			template <typename T> const T* _ptr() const { return static_cast<const T*>(data); }
			template <typename T> T& _ref() { return *_ptr<T>(); }
			template <typename T> const T& _ref() const { return *_ptr<T>(); }
		};
	}

	// Types
	using DataList = lightlist<ObjectPtr>;
	//using DataList = std::vector<ObjectPtr>;
	using CallFunc = ObjectPtr(const DataList&);
	using FuncPtr = std::function<CallFunc>;
	using LDataList = Common::lightlist<Objects::Object*>;
	using LFuncPtr = std::function<void(Objects::Object*&, const LDataList &)>;
	// Functions
	template <typename T, typename... Args>
	inline ObjectPtr createObject(Args... args) {
		return ObjectPtr(new T(args...));
	}
	Objects::Object* createObject(DefaultType type);
	ObjectPtr createError(const string &errinfo);
	const ObjectPtr& adjustObjectPtr(const ObjectPtr &ptr);
	std::string to_string(const Objects::Object &obj);
	std::string to_string(const ObjectPtr &obj);
	std::string to_string(const DataList &obj);
}

#endif
