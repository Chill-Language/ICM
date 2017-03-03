#include "basic.h"
#include "transform.h"
#include "analysisbase.h"
#include "objectdef.h"

#include "temp-getelement2.h"

namespace ICM
{
	namespace Compiler
	{
		bool PrintCompilingProcess = false;

		class PreliminaryCompile : private AnalysisBase
		{
		public:
			PreliminaryCompile(NodeTable &Table) : AnalysisBase(Table) {}

			void start() {
				if (PrintCompilingProcess)
					println("PreliminaryCompile");

				compileSub(GetNode(1), GetElement(0, 0));

				if (PrintCompilingProcess) {
					println("-->");
					printTable();
				}
			}

		private:
			Element& adjustElement(Element &elt) {
				if (elt.isRefer())
					compileSub(GetRefer(elt), elt);
				return elt;
			}
			bool adjustNode(Node &node, size_t begin = 0) {
				for (Element &e : rangei(node.begin() + begin, node.end())) {
					adjustElement(e);
				}
				return true;
			}
			bool checkBoolExp(const Element &elt) {
				if (elt.isIdent() || elt.isRefer() || elt.isLiteralType(T_Boolean))
					return true;
				println("Error : BoolExp has Non Boolean Value.");
				return false;
			}
			VecElt createDoList(const NodeRange &nr) {
				VecElt dolist;
				dolist.reserve(nr.size() + 1);
				dolist.push_back(Element::Keyword(do_));
				dolist.insert(dolist.end(), nr.begin(), nr.end());
				return dolist;
			}
			void resetNode(Node &node) {
				Element front = node.front();
				node.clear();
				node.push_back(front);
			}
			void setDoNode(Node &node, VecElt &dolist) {
				size_t id = Table.size();
				Node *p = new Node(id, std::move(dolist));
				Table.push_back(std::move(std::unique_ptr<Node>(p)));
				Element e = Element::Refer(id);
				node.push_back(e);
				adjustNode(*p, 1);
			}

		private:
			//
			bool compileSub(Node &node, Element &refelt) {
				if (PrintCompilingProcess)
					println(to_string(node));
				if (node[0].isKeyword())
					return compileKeyword(node, refelt);
				else if (node[0].isIdent() || node[0].isRefer())
					return compileCall(node, refelt);
				else
					return error("Error in compileSub.");
			}
			// call ...
			bool compileCall(Node &node, Element &refelt) {
				adjustNode(node);
				node.push_front(Element::Keyword(call_));
				return true;
			}
			bool compileKeyword(Node &node, Element &refelt) {
				switch (node[0].getKeyword()) {
				case if_:       return compileIf(node, refelt);
				case ife_:      return compileIfe(node, refelt);
				case for_:      return compileFor(node, refelt);
				case while_:    return compileWhile(node, refelt);
				case loop_:     return compileLoop(node, refelt);
				case do_:       return adjustNode(node, 1);
				case list_:     return adjustNode(node, 1);
				case p_:        return adjustNode(node, 1);
				case call_:     return adjustNode(node, 1);
				case disp_:     return compileDisp(node, refelt);
				case let_:
				case set_:
				case ref_:
				case cpy_:      return compileLSRC(node, refelt);
				case dim_:
				case restrict_: return compileRestrictDim(node, refelt);
				case define_:   return compileDefine(node, refelt);
				default:        return error("Error with unkonwn Keyword.");
				}
			}
			// (disp I|R)
			// --> (disp I|R)
			bool compileDisp(Node &node, Element &refelt) {
				if (node.size() == 2) {
					Element &e = node[1];
					if (e.isIdent() || e.isRefer()) {
						if (e.isRefer()) {
							compileSub(GetRefer(e), e);
						}
						return true;
					}
				}
				return error("Syntax error with disp.");
			}
			// (if E0 E1... elsif E2 E3... else E4 E5...)
			// --> (if E0 E2 E4 R{do E1...} R{do E3...} R{do E5...})
			bool compileIf(Node &node, Element &refelt) {
				VecElt condlist;
				vector<VecElt> dolists;
				compileIfSub(rangei(node.begin() + 1, node.end()), condlist, dolists);
				if (dolists.size() == condlist.size()) {
					dolists.push_back(VecElt{ Element::Keyword(do_) });
				}
				resetNode(node);
				node.insert(node.end(), condlist.begin(), condlist.end());
				for (auto &dolist : dolists) {
					setDoNode(node, dolist);
				}
				return true;
			}
			bool compileIfSub(const NodeRange &nr, VecElt &condList, vector<VecElt> &dolists) {
				Element &bexp = *nr.begin();
				checkBoolExp(adjustElement(bexp));
				condList.push_back(bexp);
				dolists.push_back(VecElt{ Element::Keyword(do_) });
				VecElt &dolist = dolists.back();

				auto ib = nr.begin() + 1;
				auto ie = nr.end();
				for (auto iter = ib; iter != ie; ++iter) {
					Element &e = *iter;
					if (e.isKeyword()) {
						if (e.getKeyword() == elsif_) {
							return compileIfSub(rangei(iter + 1, ie), condList, dolists);
						}
						else if (e.getKeyword() == else_) {
							VecElt elsedolist = createDoList(rangei(iter + 1, ie));
							dolists.push_back(elsedolist);
							return true;
						}
					}
					dolist.push_back(e);
				}
				return true;
			}
			// (? BE E1 E2)
			// --> (? BE E1 E2)
			bool compileIfe(Node &node, Element &refelt) {
				if (node.size() == 4) {
					Element &bexp = node[1];
					checkBoolExp(adjustElement(bexp));
					adjustElement(node[2]);
					adjustElement(node[3]);
					return true;
				}
				else
					return error("Syntax error in '?'.");
			}
			// (loop E...)
			// --> (loop R{do E...})
			bool compileLoop(Node &node, Element &refelt) {
				if (node.size() == 1)
					return error("dolist will not be blank.");
				VecElt dolist = createDoList(rangei(node.begin() + 1, node.end()));
				resetNode(node);
				setDoNode(node, dolist);
				return true;
			}
			// (while E0 E...)
			// --> (while E0 R{do E...})
			bool compileWhile(Node &node, Element &refelt) {
				if (node.size() == 1)
					return error("none boolean exp.");
				else if (node.size() == 2)
					return error("dolist will not be blank.");
				Element &bexp = node[1];
				checkBoolExp(adjustElement(bexp));
				VecElt dolist = createDoList(rangei(node.begin() + 2, node.end()));
				resetNode(node);
				node.push_back(bexp);
				setDoNode(node, dolist);
				return true;
			}
			// (for I in E0 to E1 E...)
			// --> (for I E0 E1 R{do E...})
			bool compileFor(Node &node, Element &refelt) {
				// Check
				if (node.size() < 6)
					return error("Syntax error for 'for'.");
				else if (node.size() == 6)
					return error("dolist will not be blank.");
				else if (!node[1].isIdent())
					return error("for var must be Identifier.");
				else if (!isKey(node[2], in_) || !isKey(node[4], to_))
					return error("Syntax error for 'for'.");
				// Compile
				Element I = node[1];
				Element E0 = adjustElement(node[3]);
				Element E1 = adjustElement(node[5]);
				VecElt dolist = createDoList(rangei(node.begin() + 6, node.end()));
				resetNode(node);
				node.push_back(I);
				node.push_back(E0);
				node.push_back(E1);
				setDoNode(node, dolist);
				return true;
			}
			// (let/set/ref/cpy I E) or (ref/cpy E)
			bool compileLSRC(Node &node, Element &refelt) {
				KeywordID key = node.front().getKeyword();

				if (node.size() == 3) {
					if (node[1].isIdent()) {
						adjustElement(node[2]);
						return true;
					}
					else
						return error("var must be Identifier.");
				}
				else if (node.size() == 2) {
					if (key == ref_ || key == cpy_) {
						adjustElement(node[1]);
						return true;
					}
					else
						return error("Syntax error in '" + ICM::to_string(key) + "'.");
				}
				else
					return error("Syntax error in '" + ICM::to_string(key) + "'.");
			}
			// (restrict/dim I E)
			bool compileRestrictDim(Node &node, Element &refelt) {
				KeywordID key = node.front().getKeyword();

				if (node.size() == 3) {
					if (node[1].isIdent()) {
						adjustElement(node[2]);
						return true;
					}
					else
						return error("var must be Identifier.");
				}
				else
					return error("Syntax error in '" + ICM::to_string(key) + "'.");
			}
			// (define I E)
			bool compileDefine(Node &node, Element &refelt) {
				KeywordID key = node.front().getKeyword();

				if (node.size() == 3) {
					if (node[1].isIdent()) {
						adjustElement(node[2]);
						return true;
					}
					else
						return error("var must be Identifier.");
				}
				else
					return error("Syntax error in '" + ICM::to_string(key) + "'.");
			}
		};

		class CompiletimeEvaluate : public AnalysisBase
		{
		public:
			CompiletimeEvaluate(NodeTable &Table) : AnalysisBase(Table) {}

			bool eval(Element &element, Object &result) {

			}
			bool isIdentDefined(Element &element) {
				assert(element.isIdent());

			}

		private:
			Object* call(Function::FuncObject &func, DataList &list) {
				return func.call(list).get();
			}
		};

		class IdentifierAnalysis : public AnalysisBase
		{
		public:
			IdentifierAnalysis(NodeTable &Table) : AnalysisBase(Table) {}

			void start() {
				if (PrintCompilingProcess)
					println("IdentifierAnalysis");

				setIdentSub(GetNode(1));

				if (PrintCompilingProcess) {
					println("-->");
					printTable();
				}
			}

			void setIdentSub(Node &node) {
				if (PrintCompilingProcess)
					println(to_string(node));
				// define
				if (node[0].isKeyword()) {
					if (node[0].getKeyword() == define_) {
						println("Making Define...");
						Element &ident = node[1];
						IdentSpaceIndex sid = getCurrentIdentSpaceIndex();
						IdentIndex ii = { sid };

						println(ident);
						setIdent(ident, I_Data, ii);
					}
					else if (node[0].getKeyword() == module_) {
						println("Making Module...");
						println(node[1]);
					}
				}
				// other
				bool change = false;
				for (size_t i : range(0, node.size())) {
					Element &e = node[i];
					if (e.isIdent())
						setIdentifier(e);
					else if (e.isRefer())
						setIdentSub(GetRefer(e));
					else if (e.isKeyword() && i != 0)
						setKeyword(e);
					else if (isKey(e, disp_)) {
						setKeyword(e);
						change = true;
					}
				}
				if (change)
					node.push_front(Element::Keyword(call_));
			}

		private:
			bool isIdentDefined(const IdentKey &key, IdentIndex &iid) {
				IdentBasicIndex index = findFromIdentTable(iid.space_index, key);
				if (index != getIdentTableSize(iid.space_index)) {
					iid.ident_index = index;
					return true;
				}
				return false;
			}
			void setIdentifier(Element &element) {
				const IdentKey &key = element.getIndex();
				IdentIndex ii(getCurrentIdentSpaceIndex());
				if (isIdentDefined(key, ii)) {
					IdentTableUnit &itu = getFromIdentTable(ii);
					setIdent(element, itu.type, ii);
				}
				else {
					ii.ident_index = insertFromIdentTable(ii.space_index, key, I_DyVarb);
					setIdent(element, I_DyVarb, ii);
				}
			}
			void setKeyword(Element &element) {
				if (isKey(element, list_)) {
					setIdent(element, I_StFunc, getGlobalFunctionIdentIndex("list"));
				}
				else if (isKey(element, disp_)) {
					setIdent(element, I_StFunc, getGlobalFunctionIdentIndex("disp"));
				}
			}
			void setIdent(ASTBase::Element &elt, IdentType type, const IdentIndex &index) {
				elt = ASTBase::Element::Identifier(type, ConvertIdentIndexToSizeT(index));
			}
			IdentIndex getGlobalFunctionIdentIndex(const string &name) {
				return { 0, findFromIdentTable(0, GlobalIdentNameMap[name]) };
			}
		};

		void transform(vector<AST::NodePtr> &Table) {
			PreliminaryCompile(Table).start();
			IdentifierAnalysis(Table).start();
		};
	}
}
