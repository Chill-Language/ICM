#ifndef _ICM_OBJECTS_H_
#define _ICM_OBJECTS_H_

#include "basic.h"
#include "objectmanager.h"
#include "file.h"
#include "number.h"

namespace ICM
{
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
			virtual string to_string_code() const {
				return to_string();
			}
			virtual DefaultType getType() const {
				return Type;
			}
			virtual Object* clone() const {
				return new Object(*this);
			}
			// Method
			virtual bool equ(const ObjectPtr &obj) const {
				return this == obj.get();
			}
			virtual void write(File &file) const {}
			virtual void read(File &file) {}
			// Const
			static const DefaultType Type = T_Object;

		protected:
			bool type_equal(const ObjectPtr &obj) const {
				return this->getType() == obj->getType();
			}
		};
	}
	namespace Objects
	{
		//=======================================
		// * Declarations
		//=======================================
		class Object;
		class Error;
		class Nil;

		//=======================================
		// * Class DataObject<T, _Type>
		//=======================================
		template <typename T, DefaultType _Type>
		class DataObject : public Object
		{
		public:
			using VType = T;
			static const DefaultType Type = _Type;
		public:
			DataObject() {}
			DataObject(const T &dat)
				: data(dat) {}

			T& getData() {
				return data;
			}
			const T& getData() const {
				return data;
			}
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			// Method
			string to_string() const { // Delete
				return toString();
			}
			string to_output() const { // Delete
				return toOutput();
			}
			bool equ(const ObjectPtr &obj) const { // Delete
				return equ(obj.get());
			}
			string toString() const {
				return data.toString();
			}
			string toOutput() const {
				return data.toOutput();
			}
			DefaultType getType() const {
				return _Type;
			}
			DataObject* clone() const {
				return new DataObject(*this);
			}
			bool equ(const Object* obj) const {
				return this->data == static_cast<const DataObject*>(obj)->data;
			}
			void write(File &file) const {
				data.write(file);
			}
			void read(File &file) {
				data.read(file);
			}

		private:
			T data;
		};

		template <typename _DTy>
		class ObjectStruct
		{
		public:
			ObjectStruct() {}
			ObjectStruct(const _DTy &dat) : data(dat) {}
			operator _DTy() const { return data; }
			const _DTy& getData() const { return data; }
			_DTy& getData() { return data; }
			string toString() const {
				using Common::Convert::to_string;
				return to_string(data);
			}
			bool operator==(const ObjectStruct &os) const {
				return data == os.data;
			}
			string toOutput() const { return toString(); }
			void write(File &file) const { file.write(data); }
			void read(File &file) { file.read(data); }

		private:
			_DTy data;
		};

		using Boolean = DataObject<ObjectStruct<bool>, T_Boolean>;
		using Number = DataObject<ObjectStruct<Common::Number::Rational>, T_Number>;

		class String;
		class Symbol;
		class List;
		class Disperse;
		class Identifier;
		class Keyword;
		class Function;
		class TypeClass;
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
	vector<ObjectPtr>::iterator begin(Objects::Disperse *disp);
	vector<ObjectPtr>::iterator end(Objects::Disperse *disp);

}

#endif
