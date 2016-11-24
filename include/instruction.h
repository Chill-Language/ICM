#ifndef _ICM_INSTRUCTION_H_
#define _ICM_INSTRUCTION_H_

#include "basic.h"
#include "ast.h"
#include "keyword.h"
#include "tabledata.h"
#include "typebase.h"

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
		};

		extern BijectionMap<Instruction, string> InstructionName;
	}

	string to_string(Instruction::Instruction inst);

	namespace Instruction
	{
		using FuncType = TypeBase::FunctionType;
		using VarbType = TypeBase::VariableType;

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
				vector<AST::Element> Data;

			private:
				string getToString() const {
					return ICM::to_string(Data);
				}
			};

			struct CheckCall : public InstDataBase<ccal>
			{
				vector<AST::Element> Data;

			private:
				string getToString() const {
					return ICM::to_string(Data);
				}
			};

			struct PrintIdent : public InstDataBase<pti>
			{
				vector<AST::Element> Args;

			private:
				string getToString() const {
					return ICM::to_string(Args);
				}
			};
			/*
			// FuncArgs
			struct FuncArgs : public InstDataBase<farg>
			{
				vector<AST::Element> Args;

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
				vector<AST::Element> Args;

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
				Assign(Instruction inst, VariableTableUnit &VTU, AST::Element &elt)
					: InstructionData(inst), VTU(VTU.getID()), Data(elt) {}
				size_t VTU;
				AST::Element Data;

			private:
				string getToString() const { return GlobalVariableTable[VTU].getName() + ", " + ICM::to_string(Data); }
			};

			struct CopySingle : public InstDataBase<cpys>
			{
				CopySingle(const AST::Element &Data) : Data(Data) {}
				AST::Element Data;
			private:
				string getToString() const { return ICM::to_string(Data); }
			};

			struct Store : public InstDataBase<stor>
			{
				Store(const AST::Element &Data) : Data(Data) {}
				AST::Element Data;
			private:
				string getToString() const { return ICM::to_string(Data); }
			};

			struct JumpNot : public InstDataBase<jmpn>
			{
				AST::Element Data;
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
				Inc(size_t VTU) : VTU(VTU) {}
				size_t VTU;
			private:
				string getToString() const { return GlobalVariableTable[VTU].getName(); }
			};

			struct Compare : public InstructionData
			{
				Compare(Instruction inst, size_t VTU, const AST::Element &Data)
					: InstructionData(inst), VTU(VTU), Data(Data) {}
				size_t VTU;
				AST::Element Data;
			protected:
				string getToString() const { return GlobalVariableTable[VTU].getName() + ", " + ICM::to_string(Data); }
			};
			struct JumpCompare : public Compare
			{
				JumpCompare(Instruction inst, size_t VTU, const AST::Element &Data)
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

	namespace Compiler
	{
		ICM::Instruction::InstructionList createInstruction(vector<AST::NodePtr> &Table);
	}
}

#endif
