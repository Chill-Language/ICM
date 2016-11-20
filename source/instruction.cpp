#include "instruction.h"
#include "objectdef.h"
#include "analysisbase.h"

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
			InstName(sing), InstName(stor),
			InstName(call), InstName(ccal),
			InstName(farg), InstName(fargl), InstName(fargv), InstName(fsub), InstName(fcal),
			InstName(let), InstName(cpy), InstName(ref), InstName(set),
			InstName(cpys), InstName(refs),
			InstName(inc), InstName(dec),
			InstName(jump), InstName(jmpf), InstName(jmpn), InstName(jmpc),
			InstName(jpsm), InstName(jpse), InstName(jpla), InstName(jple),
			InstName(cpsm), InstName(cpse), InstName(cpla), InstName(cple), InstName(cpeq), InstName(cpne),
			InstName(add), InstName(sub), InstName(mul), InstName(div), InstName(mod), InstName(rem),
		};
#undef InstName
	}

	namespace Compiler
	{
		extern bool PrintCompilingProcess;

		using namespace ICM::Instruction;

		class InstructionCreater : public AnalysisBase
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
				if (PrintCompilingProcess) {
					for (size_t i : range(0, InstList.size())) {
						println(i, "| ", InstList[i]->to_string());
					}
				}
				return InstList;
			}

		private:
			ICM::Instruction::InstructionList InstList;

		private:
			void createNode(Node &node, Element &refelt) {
				if (PrintCompilingProcess)
					println(to_string(node));
				assert(node.front().isKeyword());

				switch (node[0].getKeyword()) {
				case call_:
					createNodeCall(node, refelt);
					break;
				case do_:
					createNodeDo(node, refelt);
					break;
				case if_:
					createNodeIf(node, refelt);
					break;
				case for_:
					createNodeFor(node, refelt);
					break;
				case let_:
				case cpy_:
				case ref_:
					createNodeLSRC(node, refelt);
					break;
				case list_:
					createNodeList(node, refelt);
					break;
				}
			}

			// (I(x) args...)
			void createNodeCall(Node &node, Element &refelt) {
				vector<Element> Args;
				Element &front = setIdentifier(node[1]);
				createArgs(rangei(node.begin() + 2, node.end()), [&](Element &e) {
					Args.push_back(e);
				});
				Insts::CheckCall *p = new Insts::CheckCall();
				p->Args = Args;
				p->Func = front;

				InstList.push(p);
				refelt.setRefer(InstList.size() - 1);
			}

			// (do ...)
			void createNodeDo(Node &node, Element &refelt) {
				assert(node[0].getKeyword() == do_);
				for (Element &e : rangei(node.begin() + 1, node.end())) {
					if (e.isIdentifier()) {
						setIdentifier(e);
					}
					else if (e.isRefer()) {
						createNode(GetRefer(e), e);
					}
					else if (e.isData()) {
						InstList.push(new Insts::Store(e));
					}
				}
				if (node.size() == 1) {
					InstList.push(new Insts::Store(Element::Data(T_Nil, 0)));
				}
				refelt.setRefer(InstList.size() - 1);
			}

			/*void createNodeCall(Node &node, Element &refelt) {
			InstructionData *pfsub, *pfarg;
			Element &front = setIdentifier(node[0]);
			if (front.isFunction()) {
			// (F(x) args...)
			auto *inst = new Insts::FuncSub();
			inst->Func = front.getFunction().getID();
			pfsub = inst;
			}
			else if (front.isVariable()) {
			// (V(x) args...)
			auto *inst = new Insts::FuncSubVarb();
			inst->VFunc = front.getVariable().getData();
			pfsub = inst;
			}
			else {
			error();
			return;
			}
			vector<Element> Args;
			bool alldata = true;
			createArgs(rangei(node.begin() + 1, node.end()), [&](Element &e) {
			if (!e.isData() && !e.isVariable()) {
			alldata = false;
			}
			Args.push_back(e);
			});
			if (alldata) {
			auto *inst = new Insts::FuncArgsLight();
			inst->Args = createDataList(Args);
			pfarg = inst;
			}
			else {
			auto *inst = new Insts::FuncArgs();
			inst->Args = Args;
			pfarg = inst;
			}
			InstList.push(pfarg);
			InstList.push(pfsub);
			InstList.push(new Insts::FuncCall());
			refelt.setRefer(InstList.size() - 1);
			}*/
			// (if BE0 BEk BEn R{0} R{k} R{n} R{n+1})
			void createNodeIf(Node &node, Element &refelt) {
				size_t count = node.size() / 2 - 1;
				vector<Insts::Jump*> record;
				for (size_t i : range(1, 1 + count)) {
					Element &be = node[i];
					Element &de = node[i + count];

					createReferNode(be);
					auto *inst = new Insts::JumpNot();
					inst->Data = be;
					InstList.push(inst);
					createReferNode(de);
					inst->Index = InstList.size();
					auto *jinst = new Insts::Jump();
					record.push_back(jinst);
					InstList.push_back(jinst);
				}
				createReferNode(node.back());
				InstList.push(sing);
				size_t index = InstList.size() - 1;
				for (auto *p : record) {
					p->Index = index;
				}
				refelt.setRefer(index);
			}
			// (for I x y R{n})
			void createNodeFor(Node &node, Element &refelt) {
				Element &I = node[1];
				Element &vb = node[2];
				Element &ve = node[3];
				Element &Rdo = node[4];

				setIdentifier(I);
				size_t id = I.getVariable().getID();
				GlobalVariableTable[id].setData(Objects::Number(0));
				InstList.push(new Insts::Assign(let, GlobalVariableTable[id], vb));
				size_t index = InstList.size();
				createReferNode(Rdo);
				if (ve.isIdentifier())
					setIdentifier(ve);

				Element ele;
				if (ve.isVariable())
					ele = Element::Data(ve.getVariable().getData()->type, GlobalElementObjectPool.insert(ve.getVariable().getData()));
				else {
					createReferNode(ve);
					ele = ve;
				}

				auto *jpinst = new Insts::JumpCompare(jpse, id, ve);
				InstList.push(new Insts::Inc(id));
				jpinst->Index = index;
				InstList.push(jpinst);
			}
			void createNodeLSRC(Node &node, Element &refelt) {
				if (node.size() == 3) {
					assert(node[1].isIdentifier());
					Element &ident = node[1];
					Element &value = node[2];
					const string &name = ident.getIdentifier();
					VariableTableUnit& vtu = setIdentifier(ident).getVariable();
					ICM::Instruction::Instruction inst;
					switch (node[0].getKeyword()) {
					case Keyword::let_: inst = let; break;
					case Keyword::cpy_: inst = cpy; break;
					case Keyword::ref_: inst = ref; break;
					default: break;
					}
					createReferNode(value);
					InstList.push(new Insts::Assign(inst, vtu, value));
				}
				else {
					Element &value = node[1];
					createReferNode(value);
					InstList.push(new Insts::CopySingle(value));
					refelt.setRefer(InstList.size() - 1);
				}
			}
			void createNodeList(Node &node, Element &refelt) {
				Insts::List *inst = new Insts::List();
				for (auto &e : rangei(node.begin() + 1, node.end())) {
					if (e.isRefer())
						createNode(GetRefer(e), e);
					inst->Data.push_back(e);
				}
				InstList.push(inst);
				refelt.setRefer(InstList.size() - 1);
			}

			template <typename Func>
			void createArgs(const RangeIterator<vector<Element>::iterator> &r, Func func) {
				for (auto &e : r) {
					if (e.isIdentifier()) {
						setIdentifier(e);
					}
					else if (e.isRefer()) {
						createNode(GetRefer(e), e);
					}
					func(e);
				}
			}

			DataList createDataList(const vector<Element> &args) {
				lightlist_creater<Object*> creater(args.size());
				for (auto &e : args) {
					if (e.isData()) {
						creater.push_back(new Object(e.getData()));
					}
					else if (e.isVariable()) {
						creater.push_back(e.getVariable().getData());
					}
					else {
						error();
					}
				}
				return creater.data();
			}
			void createReferNode(AST::Element &element) {
				if (element.isRefer())
					createNode(GetRefer(element), element);
				else if (element.isIdentifier())
					setIdentifier(element);
			}

			Element getElement(Element element) {
				if (element.isRefer()) {
					createNode(GetRefer(element), element);
					element.setRefer(InstList.size() - 1);
				}
				return element;
			}
			Element& setIdentifier(Element &element) {
				assert(element.isIdentifier());
				const string &name = element.getIdentifier();
				size_t index;
				if ((index = GlobalVariableTable.find(name))) {
					element = Element::Variable(index);
				}
				else if ((index = GlobalFunctionTable.find(name))) {
					element = Element::Function(index);
				}
				else {
					auto &vtu = GlobalVariableTable.add(name, Objects::Nil());
					element = Element::Variable(vtu.getID());
				}
				return element;
			}

			enum class Space
			{
				Global,
				Function,
			};
			Space CurrentSpace = Space::Global;

		};

		InstructionList createInstruction(vector<AST::NodePtr>& Table) {
			InstructionCreater instcreater(Table);
			return instcreater.create();
		}

		//=======================================
		// * Class InstructionCreater
		//=======================================

		}
}
