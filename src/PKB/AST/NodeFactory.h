#ifndef SRC_PKB_AST_NODEFACTORY_H_
#define SRC_PKB_AST_NODEFACTORY_H_
#include "ASTNode.h"

class NodeFactory {
public:

	virtual ~NodeFactory();
	static ASTNode* CreateNode(std::string type, int liNr1, std::string value);
	static NewLevelNode* CreateNewLeveLNode(std::string type, int liNr1, std::string value);
private:
	static int counter;
	NodeFactory();
};

#endif /* SRC_PKB_AST_NODEFACTORY_H_ */
