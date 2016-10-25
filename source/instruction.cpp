#include "instruction.h"
#include "objectdef.h"

namespace ICM
{
	namespace Instruction
	{
		//=======================================
		// * Functions
		//=======================================
		bool key_is(const AST::Element &node, KeywordID key)
		{
			if (!node.isData()) return false;
			Object *op = node.getData().get();
			return op->type == T_Keyword && op->dat<T_Keyword>() == key;
		}
		void error(const string &msg = "")
		{
			println("Error ", msg);
		}


		////
		string InstructionData::to_string() const {
			string str;
			str.append(ICM::to_string(Inst));
			str.append(getToString());
			return str;
		}
		string Instructions::CheckCall::getToString() const {
			string str = " " + ICM::to_string_code(Func) + " ";
			for (auto &e : Args)
				str.append(ICM::to_string_code(e) + " ");
			str.pop_back();
			return str;
		}

		//=======================================
		// * Class InstructionCreater
		//=======================================
		InstructionList& InstructionCreater::create() {
			InstList.push(begin);
			createSingle(*Table[1]); // TODO
			InstList.push(end);
			for (size_t i : range(0, InstList.size())) {
				println(i, "| ", InstList[i]->to_string());
			}
			return InstList;
		}
		InstructionCreater::Element InstructionCreater::getElement(Element element) {
			if (element.isRefer()) {
				createSingle(*Table[element.getRefer()]);
				element.setRefer(InstList.size() - 1);
			}
			return element;
		}
		void InstructionCreater::createSingle(const Single &single) {
			println(to_string_code(single));
			createCheckCall(single); // TODO
		}
		void InstructionCreater::createCheckCall(const Single &single) {
			Instructions::CheckCall *dat = new Instructions::CheckCall;
			dat->Func = getElement((Element&)(single[0]));
			for (size_t i : range(1, single.size()))
				dat->Args.push_back(getElement((Element&)(single[i])));
			InstList.push_back(dat);
		}

		//=======================================
		// * InstructionName
		//=======================================
#define InstName(Inst) { Inst, #Inst }
		BijectionMap<Instruction, string> InstructionName = {
			InstName(begin), InstName(end),
			InstName(sing), InstName(stor),
			InstName(call), InstName(ccal), InstName(gfid),
			InstName(let), InstName(cpy), InstName(ref), InstName(set),
			InstName(cpys), InstName(refs),
			InstName(inc), InstName(dec),
			InstName(jump), InstName(jmpf), InstName(jmpn), InstName(jmpc),
			InstName(jpsm), InstName(jpse), InstName(jpla), InstName(jple),
			InstName(sml), InstName(sme), InstName(lar), InstName(lae), InstName(equ), InstName(eqn),
			InstName(add), InstName(sub), InstName(mul), InstName(div), InstName(mod), InstName(rem),
		};
#undef InstName
	}
}
