#pragma once
#include "ast.h"
#include "temp-getelement.h"

namespace ICM
{
	namespace Compiler
	{
		using namespace Keyword;

		class AnalysisBase
		{
		protected:
			using Element = AST::Element;
			using Node = AST::Node;
			using VecElt = vector<Element>;
			using NodeRange = RangeIterator<Node::iterator>;
			using NodeTable = vector<AST::NodePtr>;

		public:
			AnalysisBase(NodeTable &Table) : Table(Table) {}

		protected:
			NodeTable &Table;

		protected:
			// Judge
			bool isIdent(const Element &elt, const string &name) {
				return elt.isIdent() && getIdent(elt) == name;
			}
			bool isKey(const Element &elt, KeywordID key) {
				return elt.isKeyword() && elt.getKeyword() == key;
			}
			// Get
			Node& GetNode(size_t id) {
				assert(id < Table.size());
				return *Table[id];
			}
			Element& GetElement(size_t id, size_t i) {
				assert(i < GetNode(id).size());
				return GetNode(id)[i];
			}
			Node& GetRefer(const Element &elt) {
				return GetNode(elt.getRefer());
			}
			// Debug
			bool error(const string &msg = "") {
				println(msg);
				return false;
			}
			void printTable() {
				for (auto &e : rangei(Table.begin() + 1, Table.end()))
					println(ICM::to_string(*e));
			}
		};
	}
}
