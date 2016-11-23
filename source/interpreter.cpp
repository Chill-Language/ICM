#include "interpreter.h"
#include "objectdef.h"

namespace ICM
{
	class Interpreter
	{
	public:
		Interpreter(Instruction::InstructionList &InstList)
			: InstList(InstList), TempResult(InstList.size(), &Static.Null) {}

		Object* getObject(const AST::Element &element) {
			if (element.isData()) {
				return new Object(element.getData());
			}
			else if (element.isVariable()) {
				return element.getVariable().getData();
			}
			else if (element.isRefer()) {
				return TempResult[element.getRefer()];
			}
			else if (element.isFunction()) {
				return new Objects::Function(element.getFunction().getID());
			}
			else {
				println("Error in getObject.");
				return nullptr;
			}
		}
		DataList createDataList(const vector<AST::Element> &args) {
			lightlist_creater<Object*> creater(args.size());
			for (auto &e : args) {
				Object *op = getObject(e);
				if (op)
					creater.push_back(op);
			}
			return creater.data();
		}
		Object* run() {
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
					else if (func.isRefer()) {
						Object *fp = TempResult[func.getRefer()];
						if (fp->type == T_Function) {
							auto &ftu = fp->dat<T_Function>().getData();
							op = checkCall(ftu, createDataList(inst.Args));
						}
						else {
							println("'", fp->to_string(), "' is not Function.");
						}
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
						GlobalVariableTable[inst.VTU].setData(inst.Data.getData());
					else if (inst.Data.isRefer())
						GlobalVariableTable[inst.VTU].setData(TempResult[inst.Data.getRefer()]);
					else if (inst.Data.isVariable()) {
						switch (Inst->inst()) {
						case let:
							GlobalVariableTable[inst.VTU].setData(inst.Data.getVariable().getData());
							break;
						case cpy:
							GlobalVariableTable[inst.VTU].setData(inst.Data.getVariable().getData()->clone());
							break;
						case ref:
							GlobalVariableTable[inst.VTU].setData(inst.Data.getVariable().getData()); // TODO
							break;
						}
					}
					else if (inst.Data.isFunction()) {
						GlobalVariableTable[inst.VTU].setData(new Objects::Function(inst.Data.getFunction().getID()));
					}
					else
						println("Error in Assign.");
					break;
				}
				case cpys: {
					Insts::CopySingle &inst = static_cast<Insts::CopySingle&>(*Inst);
					TempResult[ProgramCounter] = getObject(inst.Data)->clone(); // TODO
					Result = TempResult[ProgramCounter];
					break;
				}
				case stor: {
					Insts::Store &inst = static_cast<Insts::Store&>(*Inst);
					TempResult[ProgramCounter] = getObject(inst.Data); // TODO
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
					continue;
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
						return nullptr;
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
				case pti: {
					Insts::PrintIdent &inst = static_cast<Insts::PrintIdent&>(*Inst);
					for (AST::Element &e : inst.Args) {
						if (e.isVariable())
							print(e.getVariable().getName(), "(");
						Object *op = getObject(e);
						if (op)
							print(getObject(e)->to_string());
						else
							print("Null");
						if (e.isVariable())
							print(")");
						println();
					}
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

	private:
		Instruction::InstructionList& InstList;
		Object* Result = &Static.Nil;
		vector<Object*> TempResult;

		struct {
			struct {
				Types::Function Func;
				DataList Args;
			} Func;
		} Global;
	};

	Object * Run(Instruction::InstructionList & instlist) {
		Interpreter interpreter(instlist);
		return interpreter.run();
	}

}
