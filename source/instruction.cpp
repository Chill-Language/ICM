#include "basic.h"
#include "instruction.h"
#include "objectdef.h"
#include "analysisbase.h"
#include "literal.h"

namespace ICM
{
	//=======================================
	// * To String
	//=======================================
	string Instruction::InstructionData::to_string() const {
		string str;
		str.append(ICM::to_string(Inst));
		const string &s = getToString();
		if (!s.empty()) {
			str.push_back(' ');
			str.append(s);
		}
		return str;
	}

	namespace Instruction
	{
		//=======================================
		// * InstructionName
		//=======================================
#define InstName(Inst) { Inst, #Inst }
		BijectionMap<ICM::Instruction::Instruction, string> InstructionName = {
			InstName(begin), InstName(end),
			InstName(sing), InstName(stor), InstName(nop), InstName(list), InstName(pti),
			InstName(call), InstName(ccal),
			InstName(farg), InstName(fargl), InstName(fargv), InstName(fsub), InstName(fcal),
			InstName(let), InstName(cpy), InstName(ref), InstName(set),
			InstName(cpys), InstName(refs),
			InstName(inc), InstName(dec),
			InstName(jump), InstName(jmpf), InstName(jmpn), InstName(jmpc),
			InstName(jpsm), InstName(jpse), InstName(jpla), InstName(jple),
			InstName(cpsm), InstName(cpse), InstName(cpla), InstName(cple), InstName(cpeq), InstName(cpne),
			InstName(add), InstName(sub), InstName(mul), InstName(div), InstName(mod), InstName(rem),
			InstName(dim), InstName(rest),
		};
#undef InstName
	}

	namespace Compiler
	{
		bool PrintCompilingProcess2 = false;

		using namespace ICM::Instruction;

		class InstructionCreater : private AnalysisBase
		{
		public:
			explicit InstructionCreater(vector<AST::NodePtr> &Table) : AnalysisBase(Table) {}
			//=======================================
			// * Class InstructionCreater
			//=======================================
			ICM::Instruction::InstructionList& create() {
				InstList.clear();
				InstList.push(begin);
				createNode(GetNode(1), GetElement(0, 0)); // TODO
				InstList.push(end);
				if (PrintCompilingProcess2) {
					for (size_t i : range(0, InstList.size())) {
						println(i, "| ", InstList[i]->to_string());
					}
				}
				return InstList;
			}

		private:
			ICM::Instruction::InstructionList InstList;
		
		private:
			size_t CurrInstID() {
				return InstList.size() - 1;
			}
			size_t NextInstID() {
				return InstList.size();
			}
		private:
			bool createNode(Node &node, Element &refelt) {
				if (PrintCompilingProcess2)
					println(to_string(node));
				assert(node.front().isKeyword());

				switch (node[0].getKeyword()) {
				case call_:     return createNodeCall(node, refelt);
				case do_:       return createNodeDo(node, refelt);
				case if_:       return createNodeIf(node, refelt);
				case while_:    return createNodeWhile(node, refelt);
				case loop_:     return createNodeLoop(node, refelt);
				case for_:      return createNodeFor(node, refelt);
				case list_:     return createNodeList(node, refelt);
				case let_:	    
				case cpy_:	    
				case ref_:      return createNodeLSRC(node, refelt);
				case p_:        return createNodePrintIdent(node, refelt);
				case dim_:      return createNodeDim(node, refelt);
				case restrict_: return createNodeRestrict(node, refelt);
				case function_: return false;
				case define_:   return false;
				case defun_:    return false;
				default:        return false;
				}
			}

			// (call I(x) args...)
			bool createNodeCall(Node &node, Element &refelt) {
				vector<Element> Data;
				Element &front = node[1];
				for (Element &e : rangei(node.begin() + 1, node.end())) {
					if (e.isRefer()) {
						createNode(GetRefer(e), e);
					}
					Data.push_back(e);
				}
				Insts::CheckCall *p = new Insts::CheckCall();
				p->Data = ConvertToInstElement(Data);

				InstList.push(p);
				refelt.setRefer(CurrInstID());
				return true;
			}

			// (do ...)
			bool createNodeDo(Node &node, Element &refelt) {
				assert(node[0].getKeyword() == do_);
				for (Element &e : rangei(node.begin() + 1, node.end())) {
					if (e.isRefer()) {
						createNode(GetRefer(e), e);
					}
					else if (e.isLiteral() || e.isIdentType(I_Function) || e.isIdentType(I_DyVarb)) {
						InstList.push(new Insts::Store(ConvertToInstElement(e)));
					}
					else if (isKey(e, break_)) {
						if (LoopBreakIDs.empty()) {
							println("Error with break.");
						}
						else {
							Insts::Jump *p = new Insts::Jump();
							size_t id = LoopBreakIDs.top();
							LoopBreakJumpPtrs.push_back(&p->Index);
							InstList.push_back(p);
						}
					}
				}
				if (node.size() == 1) {
					InstList.push(nop);
				}
				refelt.setRefer(CurrInstID());
				return true;
			}

			// (if BE0 BEk BEn R{0} R{k} R{n} R{n+1})
			bool createNodeIf(Node &node, Element &refelt) {
				size_t count = node.size() / 2 - 1;
				vector<Insts::Jump*> recordJ;
				for (size_t i : range(1, 1 + count)) {
					Element &be = node[i];
					Element &de = node[i + count];

					createReferNode(be);
					auto *inst = new Insts::JumpNot();
					inst->Data = ConvertToInstElement(be);
					InstList.push(inst);
					createReferNode(de);
					auto *jinst = new Insts::Jump();
					recordJ.push_back(jinst);
					InstList.push_back(jinst);
					inst->Index = NextInstID();
				}
				createReferNode(node.back());
				InstList.push(sing);
				size_t index = CurrInstID();
				for (auto *p : recordJ) {
					p->Index = index;
				}
				refelt.setRefer(index);
				return true;
			}

			stack<size_t> LoopBreakIDs;
			vector<size_t*> LoopBreakJumpPtrs;
			void adjustLoopBreakJump(size_t i) {
				for (size_t *pi : LoopBreakJumpPtrs) {
					*pi = i;
				}
				LoopBreakJumpPtrs.clear();
			}

			// (while E R{n})
			bool createNodeWhile(Node &node, Element &refelt) {
				LoopBreakIDs.push(node.getIndex());
				Element &Bexp = node[1];
				Element &Rdo = node[2];
				size_t Bid = NextInstID();
				createReferNode(Bexp);
				auto *jpendinst = new Insts::JumpNot();
				jpendinst->Data = ConvertToInstElement(Bexp);
				InstList.push_back(jpendinst);
				createReferNode(Rdo);
				Insts::Jump *jmp = new Insts::Jump();
				jmp->Index = Bid;
				InstList.push_back(jmp);
				jpendinst->Index = NextInstID();
				InstList.push(nop);

				refelt.setRefer(CurrInstID());
				adjustLoopBreakJump(refelt.getRefer());
				LoopBreakIDs.pop();
				return true;
			}
			// (loop R{n})
			bool createNodeLoop(Node &node, Element &refelt) {
				LoopBreakIDs.push(node.getIndex());
				Element &Rdo = node[1];
				size_t index = NextInstID();
				createReferNode(Rdo);
				auto *jpinst = new Insts::Jump();
				InstList.push(jpinst);
				jpinst->Index = index;
				InstList.push(nop);

				refelt.setRefer(CurrInstID());
				adjustLoopBreakJump(refelt.getRefer());
				LoopBreakIDs.pop();
				return true;
			}
			// (for I x y R{n})
			bool createNodeFor(Node &node, Element &refelt) {
				LoopBreakIDs.push(node.getIndex());
				Element &I = node[1];
				Element &vb = node[2];
				Element &ve = node[3];
				Element &Rdo = node[4];

				size_t id = getIdentID(I);
				setDyVarbData(id, new Objects::Number(0)); // TODO

				createReferNode(vb);
				createReferNode(ve);

				InstList.push(new Insts::Assign(let, id, ConvertToInstElement(vb)));
				size_t index = NextInstID();
				createReferNode(Rdo);

				auto *jpinst = new Insts::JumpCompare(jpse, id, ConvertToInstElement(ve));
				InstList.push(new Insts::Inc(id));
				jpinst->Index = index;
				InstList.push(jpinst);
				InstList.push(nop);

				refelt.setRefer(CurrInstID());
				adjustLoopBreakJump(refelt.getRefer());
				LoopBreakIDs.pop();
				return true;
			}
			// (let/set/ref/cpy I x y R{n}), (ref/cpy I)
			bool createNodeLSRC(Node &node, Element &refelt) {
				if (node.size() == 3) {
					assert(node[1].isIdentType(I_DyVarb));
					Element &ident = node[1];
					Element &value = node[2];
					size_t ident_id = getIdentID(ident);
					ICM::Instruction::Instruction inst;
					switch (node[0].getKeyword()) {
					case Keyword::let_: inst = let; break;
					case Keyword::cpy_: inst = cpy; break;
					case Keyword::ref_: inst = ref; break;
					default: break;
					}
					createReferNode(value);
					InstList.push(new Insts::Assign(inst, ident_id, ConvertToInstElement(value)));
				}
				else {
					Element &value = node[1];
					createReferNode(value);
					InstList.push(new Insts::CopySingle(ConvertToInstElement(value)));
					refelt.setRefer(CurrInstID());
				}
				return true;
			}
			bool createNodeDim(Node &node, Element &refelt) {
				// TODO
				assert(node.size() == 3);
				assert(node[1].isIdentType(I_StVarb));
				assert(node[2].isIdentType(I_Type));
				Element &ident = node[1];
				Element &type = node[2];
				size_t ident_id = getIdentID(ident);
				InstList.push(new Insts::Assign(dim, ident_id, ConvertToInstElement(type)));
				return true;
			}
			bool createNodeRestrict(Node &node, Element &refelt) {
				// TODO
				assert(node.size() == 3);
				assert(node[1].isIdentType(I_DyVarb));
				assert(node[2].isIdentType(I_Type));
				Element &ident = node[1];
				Element &type = node[2];
				size_t ident_id = getIdentID(ident);
				InstList.push(new Insts::Assign(rest, ident_id, ConvertToInstElement(type)));
				return true;
			}
			// (list ...)
			bool createNodeList(Node &node, Element &refelt) {
				Insts::List *inst = new Insts::List();
				for (auto &e : rangei(node.begin() + 1, node.end())) {
					if (e.isRefer())
						createNode(GetRefer(e), e);
					inst->Data.push_back(ConvertToInstElement(e));
				}
				InstList.push(inst);
				refelt.setRefer(CurrInstID());
				return true;
			}
			// (p ...)
			bool createNodePrintIdent(Node &node, Element &refelt) {
				Insts::PrintIdent *inst = new Insts::PrintIdent();
				for (auto &e : rangei(node.begin() + 1, node.end())) {
					if (e.isRefer())
						createNode(GetRefer(e), e);
					inst->Args.push_back(ConvertToInstElement(e));
				}
				InstList.push(inst);
				refelt.setRefer(CurrInstID());
				return true;
			}

			void createReferNode(AST::Element &element) {
				if (element.isRefer())
					createNode(GetRefer(element), element);
			}
		};

		InstructionList createInstruction(vector<AST::NodePtr>& Table) {
			InstructionCreater instcreater(Table);
			return instcreater.create();
		}
	}
}
