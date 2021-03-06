#include "basic.h"
#include "interpreter.h"
#include "runtime/objectdef.h"
#include "temp-getelement2.h"
#include <ctime>

namespace ICM
{
	class Interpreter
	{
	public:
		Interpreter(Instruction::InstructionList &InstList, const Compiler::ElementPool &EP)
			: InstList(InstList), TempResult(InstList.size(), &Static.Null), EP(EP) {}

		Object* getObject(const Instruction::Element &element) {
			if (element.isLiteral()) {
				return createObjectFromLiteral(element, EP);
			}
			else if (element.isRefer()) {
				return TempResult[element.getRefer()];
			}
			else if (element.isIdent()) {
				return getIdentData(element);
			}
			else {
				println("Error in getObject.");
				return nullptr;
			}
		}
		DataList createDataList(vector<Instruction::Element> &args) {
			return createDataList(rangei(args.begin(), args.end()));
		}
		DataList createDataList(const RangeIterator<vector<Instruction::Element>::iterator> &ri) {
			lightlist_creater<Object*> creater(ri.size());
			for (auto &e : ri) {
				Object *op = getObject(e);
				if (op)
					creater.push_back(op);
			}
			return creater.data();
		}
		DataList createDispCallList(vector<Instruction::Element> &el, const FuncTableUnit* &ftup) {
			list<Object*> datalist;
			for (auto &e : el) {
				Object *op = getObject(e);
				if (op) {
					if (op->type == T_Disperse) {
						auto &ve = op->dat<T_Disperse>().getData();
						datalist.insert(datalist.end(), ve.begin(), ve.end());
					}
					else {
						datalist.push_back(op);
					}
				}
			}
			Object *fp = datalist.front();
			datalist.pop_front();
			if (fp->type != T_Function) {
				println("'", fp->to_string(), "' is not Function.");
				return DataList();
			}
			ftup = &fp->dat<T_Function>().getData();
			return DataList(datalist.begin(), datalist.size());
		}
		Object* CheckCall(vector<Instruction::Element> &Data) {
			const FuncTableUnit *ftup;

			Instruction::Element &front = Data.front();
			if (front.isIdentType(I_StFunc)) {
				ftup = &getStFunc(front);
			}
			else if (front.isIdentType(I_DyVarb) || front.isRefer()) {
				Object *fp = getObject(front);
				if (fp->type == T_Disperse) {
				}
				else if (fp->type != T_Function) {
					println("'", fp->to_string(), "' is not Function.");
					return nullptr;
				}
				else {
					ftup = &fp->dat<T_Function>().getData();
				}
			}
			else {
				println("Error in CheckCall.");
				return nullptr;
			}

			DataList dl = createDispCallList(Data, ftup);
			ObjectPtr op = checkCall(*ftup, dl);
			return op.get();
		}

		Object* run() {
			size_t ProgramCounter = 0;
			std::srand(static_cast<unsigned>(std::time(0)));
			while (true) {
				using namespace Instruction;
				auto &Inst = InstList[ProgramCounter];
				//println(ProgramCounter, "| ", Inst->to_string());
				switch (Inst->inst()) {
				case ccal: {
					Insts::CheckCall &inst = static_cast<Insts::CheckCall&>(*Inst);
					TempResult[ProgramCounter] = CheckCall(inst.Data);
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
					if (inst.Data.isLiteral()) {
						setDyVarbData(inst.VTU, createObjectFromLiteral(inst.Data, EP));
					}
					else if (inst.Data.isRefer()) {
						setDyVarbData(inst.VTU, TempResult[inst.Data.getRefer()]);
					}
					else if (inst.Data.isIdentType(I_DyVarb)) {
						switch (Inst->inst()) {
						case let:
							setDyVarbData(inst.VTU, getDyVarbData(inst.Data));
							break;
						case cpy:
							setDyVarbData(inst.VTU, getDyVarbData(inst.Data)->clone());
							break;
						case ref:
							setDyVarbData(inst.VTU, getDyVarbData(inst.Data)); // TODO
							break;
						}
					}
					else if (inst.Data.isIdent()) {
						setDyVarbData(inst.VTU, getIdentData(inst.Data));
					}
					else
						println("Error in Assign.");
					break;
				}
				case dim: {
					Insts::Assign &inst = static_cast<Insts::Assign&>(*Inst);
					break;
				}
				case rest: {
					Insts::Assign &inst = static_cast<Insts::Assign&>(*Inst);
					//(restrict a (type 5))
					Object *data = getObject(inst.Data);
					setDyVarbRestType(inst.VTU, data->dat<T_Type>().get());
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
					getDyVarbData(inst.VTU)->dat<T_Number>() += 1;
					break;
				}
				case jpsm:
				case jpse:
				case jpla:
				case jple: {
					Insts::JumpCompare &inst = static_cast<Insts::JumpCompare&>(*Inst);
					const auto &n1 = getDyVarbData(inst.VTU)->dat<T_Number>();
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
					for (Element &e : inst.Args) {
						if (e.isIdentType(I_DyVarb))
							print(getIdentName(e), "(");
						Object *op = getObject(e);
						if (op)
							print(getObject(e)->to_string());
						else
							print("Null");
						if (e.isIdentType(I_DyVarb))
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
		const Compiler::ElementPool &EP;

		struct {
			struct {
				Types::Function Func;
				DataList Args;
			} Func;
		} Global;
	};

	Object * Run(Instruction::InstructionList & instlist, const Compiler::ElementPool &EP) {
		Interpreter interpreter(instlist, EP);
		return interpreter.run();
	}

}
