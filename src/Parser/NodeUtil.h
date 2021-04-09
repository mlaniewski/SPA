#ifndef SRC_PARSER_NODEUTIL_H_
#define SRC_PARSER_NODEUTIL_H_

class NodeUtil {
	public:

	static ASTTree* createCallNode(string name, int lineNumber) {
		ASTNode* node = new ASTNode(0, NodeName::CALL, lineNumber, name);
		return createTree(node);
	}

	static ASTTree* createAssignmentNode(int lineNumber) {
		ASTNode* node = new ASTNode(0, NodeName::ASSIGN, lineNumber);
		return createTree(node);
	}

	static ASTTree* createProcedureNode(string name, int lineNumber) {
		ASTNode* node = new NewLevelNode(0, NodeName::PROCEDURE, lineNumber, name);
		return createTree(node);
	}

	static tree_node_<ASTNode*>* createProgramNode() {
		ASTNode* node = new NewLevelNode(0, NodeName::PROGRAM, 0);
		return createTreeNode(node);
	}

	static ASTTree* createOperandNode(string name, int lineNumber) {
		ASTNode* node = new ASTNode(0, NodeName::OPERAND, lineNumber, name);
		return createTree(node);
	}

	static ASTTree* createWhileNode(string name, int lineNumber) {
		ASTNode* node = new NewLevelNode(0, NodeName::WHILE, lineNumber, name);
		return createTree(node);
	}

	static ASTTree* createIfNode(string name, int lineNumber) {
		ASTNode* node = new NewLevelNode(0, NodeName::IF, lineNumber, name);
		return createTree(node);
	}
	static ASTTree* createElseNode(int lineNumber) {
		ASTNode* node = new NewLevelNode(0, NodeName::ELSE, lineNumber);
		return createTree(node);
	}

	static ASTTree* createExpressionNode(string name, int lineNumber) {
		ASTNode* node = new ASTNode(0, NodeName::EXPR, lineNumber, name);
		return createTree(node);
	}

	static tree<tree_node_<ASTNode*>*>::iterator appendChild(ASTTree* t, tree<tree_node_<ASTNode*>*>::iterator parent, ASTTree* child) {
		return t->appendSubTree(parent, child);
	}

	static void appendChild(ASTTree* t, ASTTree* child) {
		t->appendSubTree(t->getKorzenDrzewa(), child);
	}

	static ASTTree* createFakeOpenNode() {
		ASTNode* node = new ASTNode(0, NodeName::FAKE_OPEN, 0, NodeName::FAKE_OPEN);
		return createTree(node);
	}

	static ASTTree* createFakeCloseNode(int closed) {
		ASTNode* node = new ASTNode(0, NodeName::FAKE_CLOSE, 0, NodeName::FAKE_CLOSE);
		node->closed = closed;
		return createTree(node);
	}

	static tree<tree_node_<ASTNode*>*>::iterator appendChild(ASTTree* t, tree<tree_node_<ASTNode*>*>::iterator parent,
			tree_node_<ASTNode*>* child) {
		return t->appendChild(parent, child);
	}

private:
	NodeUtil() {
	}

	static ASTTree* createTree(ASTNode* node) {
		ASTTree* tree = new ASTTree();
		tree_node_<ASTNode*>* outer = createTreeNode(node);
		tree->appendRoot(outer);
		return tree;
	}

	static tree_node_<ASTNode*>* createTreeNode(ASTNode* node) {
		return new tree_node_<ASTNode*>(node);
	}

};

#endif
