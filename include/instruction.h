#ifndef _ICM_INSTRUCTION_H_
#define _ICM_INSTRUCTION_H_

#include "basic.h"
#include "ast.h"
#include "keyword.h"
#include "tabledata.h"

namespace ICM
{
	namespace Instruction
	{
		enum Instruction {
			begin, end,
			sing, // sing a
			stor, // stor
			// Function
			call, // call f.i args...
			ccal, // ccal f args...
			gfid, // gfid f args...
			// Assign
			let, // let V e
			cpy, // cpy V e
			ref, // ref V e
			set, // set V e
			cpys, // cpys e
			refs, // refs e
			//
			inc, // inc e
			dec, // dec e
			// Jump
			jump, // jump {i}
			jmpf, // jmpf b {i}
			jmpn, // jmpn b {i}
			jmpc, // jmpc b {i} {j}
			jpsm, jpse, jpla, jple, // jpxx a b {i}
			// Compare
			sml, sme, lar, lae, equ, eqn, // xxx a b
			// Calc
			add, sub, mul, div, mod, rem, // xxx a b
		};

		extern BijectionMap<Instruction, string> InstructionName;
	}

	string to_string(Instruction::Instruction inst);

	namespace Instruction
	{
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
		namespace Instructions
		{
			struct CheckCall : public InstructionData
			{
				CheckCall() : InstructionData(ccal) {}

				AST::Element Func;
				vector<AST::Element> Args;

			private:
				string getToString() const;
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

		class InstructionCreater
		{
			using Single = AST::Node;
			using Element = AST::Element;
		public:
			explicit InstructionCreater(vector<AST::NodePtr> &table) : Table(table) {}
			InstructionList& create();
			void createCheckCall(const Single &single);
			void createSingle(const Single &single);
			Element getElement(Element element);

		private:
			InstructionList InstList;
			vector<AST::NodePtr> Table;
		};
	}
}

#endif
