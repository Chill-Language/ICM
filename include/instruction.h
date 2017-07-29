#pragma once
#include "basic.h"
#include "parser/keyword.h"
#include "runtime/typebase.h"

#include "temp-getelement2.h"

namespace ICM
{
	namespace Instruction
	{
		enum Instruction {
			begin, end,
			sing, // sing a
			stor, // stor
			list, // list ...
			nop,  // nop
			pti,  // pti ...
			// Function
			call, // call f.i args...
			ccal, // ccal f args...

			iadd,
			isub,
			imul,
			idiv,
			imod,
			ilrl,

			farg,  // farg args...   ; Ve(E) -> DL
			fargl, // fargl args...  ; DL
			fargv, // fargv args...  ; Ve(E) -> Ve(O*) -> DL
			fsub,  // fsub f, $
			fsubr, // fsubr r, $
			fsubv, // fsubv v, $
			fcal,  // fcal $, $
			// Assign
			let, // let V E
			cpy, // cpy V E
			ref, // ref V E
			set, // set V E
			cpys, // cpys E
			refs, // refs E
			//
			inc, // inc E
			dec, // dec E
			// Jump
			jump, // jump {i}
			jmpf, // jmpf b {i}
			jmpn, // jmpn b {i}
			jmpc, // jmpc b {i} {j}
			jpsm, jpse, jpla, jple, // jpxx a b {i}
			// Compare
			cpsm, cpse, cpla, cple, cpeq, cpne, // cpxx a b
			// Calc
			add, sub, mul, div, mod, rem, // xxx a b
			// Varb
			dim,  // dim a type
			rest, // rest a type
		};

		extern BijectionMap<Instruction, string> InstructionName;
	}

	string to_string(Instruction::Instruction inst);


	namespace ASTBase
	{
		struct Element;
	}

	namespace Instruction
	{
		//=======================================
		// * Struct Element
		//=======================================
		struct Element
		{
		private:
			enum ElementType {
				E_Void,
				E_Literal,
				E_Refer,
				E_Ident,
				E_Key,
			};

		public:
			Element() = default;
			Element(ElementType et, size_t id, size_t index)
				: etype((uint8_t)et), stype((uint8_t)id), index(index) {}
			Element(const Element&) = default;

			// Static Function
			static Element Literal(size_t type, size_t index) { return Element(E_Literal, type, index); }
			static Element Refer(size_t index) { return Element(E_Refer, 0, index); }
			static Element Keyword(Keyword::KeywordID key) { return Element(E_Key, key, 0); }
			static Element Identifier(size_t index) { return Element(E_Ident, 0, index); }
			static Element Identifier(IdentType type, size_t index) { return Element(E_Ident, type, index); }

			// Judge
			bool isLiteral() const { return isEltType(E_Literal); }
			bool isRefer() const { return isEltType(E_Refer); }
			bool isKeyword() const { return isEltType(E_Key); }
			bool isIdent() const { return isEltType(E_Ident); }

			bool isLiteralType(TypeUnit type) const { return isLiteral() && getSubType() == type; }
			bool isIdentType(IdentType type) const { return isIdent() && getSubType() == type; }

			// Get/Set Index
			void setIndex(size_t id) { index = id; }
			size_t getIndex() const { return index; }

			// Get/Set
			void setRefer(size_t id) { assert(isRefer()); setIndex(id); }
			size_t getRefer() const { assert(isRefer()); return getIndex(); }
			const IdentIndex& getIdentIndex() const { return reinterpret_cast<const IdentIndex&>(index); }
			TypeUnit getLiteralType() const { assert(isLiteral()); return getSubType(); }
			size_t getIndetType() const { assert(isIdent()); return getSubType(); }
			Keyword::KeywordID getKeyword() const { assert(isKeyword()); return (Keyword::KeywordID)getSubType(); }

		private:
			union {
				struct { uint8_t etype, stype; };
				uint32_t type = 0;
			};
			uint_t index = 0;

		private:
			bool isEltType(ElementType et) const { return (ElementType)this->etype == et; }
			size_t getSubType() const { return (size_t)this->stype; }
		};

		inline Element ConvertToInstElement(const ASTBase::Element &elt) {
			return *(const Element*)(&elt);
		}
		inline vector<Element> ConvertToInstElement(const vector<ASTBase::Element> &elt) {
			return *(const vector<Element>*)(&elt);
		}
	}

	string to_string(const Instruction::Element &elt);
	string to_string(const vector<Instruction::Element> &elt);

	namespace Instruction
	{
		using FuncType = TypeBase::FunctionType;

		class InstructionData
		{
		public:
			InstructionData(Instruction inst)
				: Inst(inst) {}
			Instruction inst() const { return Inst; }
			string to_string() const;

		private:
			Instruction Inst;
			virtual string getToString() const { return ""; }
		};

		namespace Insts
		{
			template <Instruction _Inst>
			struct InstDataBase : public InstructionData {
				InstDataBase() : InstructionData(_Inst) {}
			};

			struct List : public InstDataBase<list>
			{
				vector<Element> Data;

			private:
				string getToString() const {
					return ICM::to_string(Data);
				}
			};

			struct CheckCall : public InstDataBase<ccal>
			{
				vector<Element> Data;

			private:
				string getToString() const {
					return ICM::to_string(Data);
				}
			};

			struct PrintIdent : public InstDataBase<pti>
			{
				vector<Element> Args;

			private:
				string getToString() const {
					return ICM::to_string(Args);
				}
			};
			/*
			// FuncArgs
			struct FuncArgs : public InstDataBase<farg>
			{
				vector<Element> Args;

			private:
				string getToString() const { return ICM::to_string(Args); }
			};
			struct FuncArgsLight : public InstDataBase<fargl>
			{
				DataList Args;

			private:
				string getToString() const { return Convert::to_string(Args.begin(), Args.end()); }
			};
			struct FuncArgsVary : public InstDataBase<fargv>
			{
				vector<Element> Args;

			private:
				string getToString() const { return ICM::to_string(Args); }
			};
			// FuncSub
			struct FuncSub : public InstDataBase<fsub>
			{
				FuncType Func;

			private:
				string getToString() const { return ICM::to_string(Func) + ", $"; }
			};
			struct FuncSubVarb : public InstDataBase<fsubv>
			{
				Object* VFunc;

			private:
				string getToString() const { return VFunc->to_string() + ", $"; }
			};
			struct FuncSubRef : public InstDataBase<fsubr>
			{
				size_t RFunc;

			private:
				string getToString() const { return "{" + std::to_string(RFunc) + "}" + ", $"; }
			};*/

			struct FuncCall : public InstDataBase<fcal>
			{
			private:
				string getToString() const { return "$, $"; }
			};

			struct Assign : public InstructionData
			{
				Assign(Instruction inst, const IdentIndex &VTUID, const Element &elt)
					: InstructionData(inst), VTU(VTUID), Data(elt) {}
				IdentIndex VTU;
				Element Data;

			private:
				string getToString() const { return getIdentName(VTU) + ", " + ICM::to_string(Data); }
			};

			struct CopySingle : public InstDataBase<cpys>
			{
				CopySingle(const Element &Data) : Data(Data) {}
				Element Data;
			private:
				string getToString() const { return ICM::to_string(Data); }
			};

			struct Store : public InstDataBase<stor>
			{
				Store(const Element &Data) : Data(Data) {}
				Element Data;
			private:
				string getToString() const { return ICM::to_string(Data); }
			};

			struct JumpNot : public InstDataBase<jmpn>
			{
				Element Data;
				size_t Index;
			private:
				string getToString() const {
					return ICM::to_string(Data) + ", {" + std::to_string(Index) + "}";
				}
			};

			struct Jump : public InstDataBase<jump>
			{
				size_t Index;
			private:
				string getToString() const {
					return "{" + std::to_string(Index) + "}";
				}
			};

			struct Inc : public InstDataBase<inc>
			{
				Inc(const IdentIndex &VTU) : VTU(VTU) {}
				IdentIndex VTU;
			private:
				string getToString() const { return getIdentName(VTU); }
			};

			struct Compare : public InstructionData
			{
				Compare(Instruction inst, const IdentIndex &VTU, const Element &Data)
					: InstructionData(inst), VTU(VTU), Data(Data) {}
				IdentIndex VTU;
				Element Data;
			protected:
				string getToString() const { return getIdentName(VTU) + ", " + ICM::to_string(Data); }
			};
			struct JumpCompare : public Compare
			{
				JumpCompare(Instruction inst, const IdentIndex &VTU, const Element &Data)
					: Compare(inst, VTU, Data) {}
				size_t Index;
			private:
				string getToString() const { return  Compare::getToString() + ", {" + std::to_string(Index) + "}"; }
			};
		}

		class InstructionList : public vector<InstructionData*>
		{
		public:
			void push(Instruction inst) {
				push_back(new InstructionData(inst));
			}
			void push(InstructionData *instdata) {
				push_back(instdata);
			}

		private:

		};
	}
}
