#include "transform.h"
#include "analysisbase.h"
#include "objectdef.h"

namespace ICM
{
	namespace Compiler
	{
		bool PrintCompilingProcess = !true;

		class PreliminaryCompile : private AnalysisBase
		{
		public:
			PreliminaryCompile(NodeTable &Table) : AnalysisBase(Table) {}

			void start() {
				if (PrintCompilingProcess)
					println("PreliminaryCompile");

				compileSub(GetNode(1), GetNode(0)[0]);

				if (PrintCompilingProcess) {
					println("-->");
					printTable();
				}
			}

		private:
			bool checkBoolExp(const Element &elt) {
				if (elt.isIdentifier() || elt.isRefer() || elt.isBoolean())
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
			Element& adjustElement(Element &elt) {
				if (elt.isRefer())
					compileSub(GetRefer(elt), elt);
				return elt;
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
				compileDo(*p, e);
			}

		private:
			//
			void compileSub(Node &node, Element &refelt) {
				if (PrintCompilingProcess)
					println(to_string(node));
				if (node[0].isKeyword())
					compileKeyword(node, refelt);
				else if (node[0].isIdentifier() || node[0].isRefer())
					compileCall(node, refelt);
				else
					println("Error in compileSub.");
			}
			// call ...
			void compileCall(Node &node, Element &refelt) {
				checkCallList(rangei(node.begin(), node.end()));
				node.push_front(Element::Keyword(call_));
			}
			void checkCallList(const RangeIterator<Node::iterator> &ri) {
				for (auto &e : ri) {
					if (e.isKeyword()) {
						if (isKey(e, list_)) {
							e = Element::Function(GlobalFunctionTable["list"].getID());
						}
						else {
							println("Error : DoList has Keyword.");
						}
					}
					adjustElement(e);
				}
			}
			void compileKeyword(Node &node, Element &refelt) {
				switch (node[0].getKeyword()) {
				case if_:
					compileIf(node, refelt);
					break;
				case for_:
					compileFor(node, refelt);
					break;
				case while_:
					compileWhile(node, refelt);
					break;
				case loop_:
					compileLoop(node, refelt);
					break;
				case do_:
					compileDo(node, refelt);
					break;
				case list_:
					compileList(node, refelt);
					break;
				case disp_:
					compileDisp(node, refelt);
					break;
				case let_:
				case set_:
				case ref_:
				case cpy_:
					compileLSRC(node, refelt);
					break;
				case p_:
					compilePrintIdent(node, refelt);
					break;
				case call_:
					compileCallK(node, refelt);
					break;
				default:
					println("Error with unkonwn Keyword.");
				}
			}
			void compileDo(Node &node, Element &refelt) {
				for (auto &e : rangei(node.begin() + 1, node.end())) {
					adjustElement(e);
				}
			}
			void compileCallK(Node &node, Element &refelt) {
				checkCallList(rangei(node.begin() + 1, node.end()));
			}
			void compileList(Node &node, Element &refelt) {
				for (auto &e : rangei(node.begin() + 1, node.end())) {
					adjustElement(e);
				}
			}
			void compilePrintIdent(Node &node, Element &refelt) {
				checkCallList(rangei(node.begin() + 1, node.end()));
			}
			void compileDisp(Node &node, Element &refelt) {
				if (node.size() == 2) {
					Element &e = node[1];
					if (e.isIdentifier() || e.isRefer()) {
						//refelt = Element(e).setDisp();
						if (e.isRefer()) {
							compileSub(GetRefer(e), e);
						}
					}
					else {
						println("Error in compile disp.");
						return;
					}
				}
				else {
					println("Error in compile disp.");
					return;
				}
			}
			// (if E0 E1... elsif E2 E3... else E4 E5...)
			// --> (if E0 E2 E4 R{do E1...} R{do E3...} R{do E5...})
			void compileIf(Node &node, Element &refelt) {
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
			}
			void compileIfSub(const NodeRange &nr, VecElt &condList, vector<VecElt> &dolists) {
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
						if (e.getKeyword() == else_) {
							VecElt elsedolist = createDoList(rangei(iter + 1, ie));
							dolists.push_back(elsedolist);
							return;
						}
						else if (e.getKeyword() == elsif_) {
							compileIfSub(rangei(iter + 1, ie), condList, dolists);
							return;
						}
					}
					dolist.push_back(e);
				}
			}
			// (loop E...)
			// --> (loop R{do E...})
			void compileLoop(Node &node, Element &refelt) {
				if (node.size() == 1) {
					println("dolist will not be blank.");
					return;
				}
				VecElt dolist = createDoList(rangei(node.begin() + 1, node.end()));
				resetNode(node);
				setDoNode(node, dolist);
			}
			// (while E0 E...)
			// --> (while E0 R{do E...})
			void compileWhile(Node &node, Element &refelt) {
				if (node.size() == 1) {
					println("none boolean exp.");
					return;
				}
				else if (node.size() == 2) {
					println("dolist will not be blank.");
					return;
				}
				Element &bexp = node[1];
				checkBoolExp(adjustElement(bexp));
				VecElt dolist = createDoList(rangei(node.begin() + 2, node.end()));
				resetNode(node);
				node.push_back(bexp);
				setDoNode(node, dolist);
			}
			// (for I in E0 to E1 E...)
			// --> (for I E0 E1 R{do E...})
			void compileFor(Node &node, Element &refelt) {
				if (node.size() < 6) {
					println("Syntex error for 'for'.");
					return;
				}
				if (!node[1].isIdentifier()) {
					println("for var must be Identifier.");
					return;
				}
				if (!isKey(node[2], in_) || !isKey(node[4], to_)) {
					println("Syntex error for 'for'.");
					return;
				}
				//if (!(node[3].isData() || node[3].isRefer())
				//	|| !(node[5].isData() || node[5].isRefer())) {
				//	println("Syntex error for 'for'.");
				//	return;
				//}
				if (node.size() == 6) {
					println("dolist will not be blank.");
					return;
				}
				Element I = node[1];
				Element E0 = adjustElement(node[3]);
				Element E1 = adjustElement(node[5]);
				VecElt dolist = createDoList(rangei(node.begin() + 6, node.end()));
				resetNode(node);
				node.push_back(I);
				node.push_back(E0);
				node.push_back(E1);
				setDoNode(node, dolist);
			}
			// (let/set/ref/cpy I E) or (ref/cpy E)
			void compileLSRC(Node &node, Element &refelt) {
				KeywordID key = node.front().getKeyword();

				if (node.size() == 3) {
					if (node[1].isIdentifier())
						adjustElement(node[2]);
					else
						println("var must be Identifier.");
				}
				else if (node.size() == 2) {
					if (key == ref_ || key == cpy_)
						adjustElement(node[1]);
					else
						println("Syntex error in '", key, "'.");
				}
				else {
					println("Syntex error in '", key, "'.");
				}
			}
		};

		class RoughAnalysis : private AnalysisBase
		{
		public:
			RoughAnalysis(NodeTable &Table) : AnalysisBase(Table) {}


			struct DispNodeRecord
			{
				DispNodeRecord() : DispNodeRecord(0, 0, 0) {}
				DispNodeRecord(Node *node, Node *refnode, size_t refpost)
					: node(node), refnode(refnode), refpost(refpost) {}

				Node *node;
				Node *refnode;
				size_t refpost;
			};
			DispNodeRecord dispNode;

			void start() {
				if (PrintCompilingProcess)
					println("RoughAnalysis");
				while (analysisNode(GetNode(0), 0));

				if (PrintCompilingProcess) {
					println("-->");
					printTable();
				}
			}


			bool analysisNode(Node &refnode, size_t refpost) {
				Element &refelt = refnode[refpost];
				Node &node = GetRefer(refelt);
				if (PrintCompilingProcess)
					println(to_string(node));
				if (isKey(node[0], disp_)) {
					dispNode = DispNodeRecord(&node, &refnode, refpost);
				}
				else if (isKey(node[0], list_)) {
					DispNodeRecord &tnr = dispNode;
					if (tnr.node == &refnode) {
						setRefNodeNew(node, *tnr.refnode, tnr.refpost);
						dispNode = DispNodeRecord();
						return true;
					}
				}
				for (size_t i = 0; i < node.size(); i++) {
					if (node[i].isRefer()) {
						if (analysisNode(node, i))
							return true;
					}
				}
				return false;
			}
			void setRefNodeNew(Node &node, Node &refnode, size_t refpost) {
				refnode.erase(refnode.begin() + refpost);
				refnode.insert(refnode.begin() + refpost, node.begin() + 1, node.end());
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
				for (auto &e : node) {
					if (e.isIdentifier())
						setIdentifier(e);
					else if (e.isRefer())
						setIdentSub(GetRefer(e));
				}
			}

		private:
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
		};

		void transform(vector<AST::NodePtr> &Table) {
			PreliminaryCompile(Table).start();
			RoughAnalysis(Table).start();
			IdentifierAnalysis(Table).start();
		};
	}
}
