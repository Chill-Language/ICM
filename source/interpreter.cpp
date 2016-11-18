#include "interpreter.h"
#include "objectdef.h"

namespace ICM
{
	DataList Interpreter::createDataList(const vector<AST::Element>& args) {
		lightlist_creater<Object*> creater(args.size());
		for (auto &e : args) {
			if (e.isData()) {
				creater.push_back(new Object(e.getData()));
			}
			else if (e.isVariable()) {
				creater.push_back(e.getVariable().getData());
			}
			else if (e.isRefer()) {
				creater.push_back(TempResult[e.getRefer()]);
			}
			else {
				println("Error !!");
			}
		}
		return creater.data();
	}

	Object* Interpreter::run() {
		size_t ProgramCounter = 0;
		while (true) {
			using namespace Instruction;
			auto &Inst = InstList[ProgramCounter];
			//println(ProgramCounter, "| ", Inst->to_string());
			switch (Inst->inst()) {
			case ccal: {
				Insts::CheckCall &inst = static_cast<Insts::CheckCall&>(*Inst);
				AST::Element &func = inst.Func;
				ObjectPtr op;
				if (func.isVariable()) {
					auto &ftu = func.getVariable().getData()->dat<T_Function>().getData();
					op = checkCall(ftu, createDataList(inst.Args));
				}
				else if (func.isFunction()) {
					auto &ftu = func.getFunction();
					op = checkCall(ftu, createDataList(inst.Args));
				}
				TempResult[ProgramCounter] = op.get();
				Result = TempResult[ProgramCounter];
				break;
			}
			/*case farg: {
				Insts::FuncArgs &inst = static_cast<Insts::FuncArgs&>(*Inst);
				Global.Func.Args = createDataList(inst.Args);
				break;
			}
			case fargl: {
				Insts::FuncArgsLight &inst = *static_cast<Insts::FuncArgsLight*>(Inst);
				Global.Func.Args = inst.Args;
				break;
			}
			case fargv: {
				break;
			}
			case fsub: {
				Insts::FuncSub &inst = static_cast<Insts::FuncSub&>(*Inst);
				auto &args = Global.Func.Args;
				auto &ftu = GlobalFunctionTable[inst.Func.index];
				size_t id = getCallID(ftu, args);
				if (id != ftu.size()) {
					Global.Func.Func.index = inst.Func.index;
					Global.Func.Func.subid = id;
				}
				else {
					println("Error Unfind args match function ", ftu.getName(), ".");
					Global.Func.Func.index = 0;
					Global.Func.Func.subid = 0;
				}
				break;
			}
			case fsubr: {
				break;
			}
			case fsubv: {
				break;
			}
			case fcal: {
				auto &args = Global.Func.Args;
				auto &func = Global.Func.Func;
				if (func.index == 0) {
					println("Call Error");
					return Result;
				}
				ObjectPtr &&op = GlobalFunctionTable[func.index][func.subid].call(args);
				TempResult[ProgramCounter] = op.get();
				Result = TempResult[ProgramCounter];
				break;
			}*/
			case let:
			case cpy:
			case ref: {
				Insts::Assign &inst = static_cast<Insts::Assign&>(*Inst);
				if (inst.Data.isData())
					GlobalVariableTable[inst.VTU].setData(inst.Data.getData().clone());
				else if (inst.Data.isRefer())
					GlobalVariableTable[inst.VTU].setData(TempResult[inst.Data.getRefer()]->clone());
				break;
			}
			case stor: {
				Insts::Store &inst = static_cast<Insts::Store&>(*Inst);
				TempResult[ProgramCounter] = new Object(inst.Data.getData()); // TODO
				Result = TempResult[ProgramCounter];
				break;
			}
			case sing: {
				TempResult[ProgramCounter] = Result;
				Result = TempResult[ProgramCounter];
				break;
			}
			case jump: {
				Insts::Jump &inst = static_cast<Insts::Jump&>(*Inst);
				ProgramCounter = inst.Index;
				break;
			}
			case jmpf:
			case jmpn: {
				Insts::JumpNot &inst = static_cast<Insts::JumpNot&>(*Inst);
				Object *op = getObject(inst.Data);
				if (op->isType(T_Boolean)) {
					bool r = op->dat<T_Boolean>();
					if ((Inst->inst() == jmpf && r) || (Inst->inst() == jmpn && !r)) {
						ProgramCounter = inst.Index;
						continue;
					}
				}
				else {
					println("Error Not Boolean");
				}
				break;
			}
			case inc: {
				Insts::Inc &inst = static_cast<Insts::Inc&>(*Inst);
				GlobalVariableTable[inst.VTU].getData()->dat<T_Number>() += 1;
				break;
			}
			case jpsm:
			case jpse:
			case jpla:
			case jple: {
				Insts::JumpCompare &inst = static_cast<Insts::JumpCompare&>(*Inst);
				const auto &n1 = GlobalVariableTable[inst.VTU].getData()->dat<T_Number>();
				const auto &n2 = getObject(inst.Data)->dat<T_Number>();
				bool r;
				switch (Inst->inst()) {
				case jpsm: r = n1 < n2; break;
				case jpse: r = n1 <= n2; break;
				case jpla: r = n1 > n2; break;
				case jple: r = n1 >= n2; break;
				default:   r = false;
				}
				if (r) {
					ProgramCounter = inst.Index;
					continue;
				}
				break;
			}
			case list: {
				Insts::List &inst = static_cast<Insts::List&>(*Inst);
				// TODO
				auto v = createDataList(inst.Data);
				lightlist_creater<Object*> llc(v.size());
				for (auto p : v) {
					llc.push_back(p->clone());
				}

				TempResult[ProgramCounter] = new Objects::List(TypeBase::ListType(llc.data()));
				Result = TempResult[ProgramCounter];
				break;
			}
			case end: {
				return Result;
			}
			}
			ProgramCounter++;
		}
		return Result;
	}
	Object* Interpreter::getObject(AST::Element & element) {
		if (element.isData())
			return new Object(element.getData());
		else if (element.isRefer())
			return TempResult[element.getRefer()];
		else if (element.isVariable())
			return element.getVariable().getData();
		println("Error in getObject");
		return nullptr;
	}
}
