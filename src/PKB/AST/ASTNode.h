#ifndef SRC_PKB_AST_ASTNode_H_
#define SRC_PKB_AST_ASTNode_H_
#include <string>
class ASTNode {
public:
	std::string value = "";
	std::string type = "";
	int id;
	int liNr1;

	//parser
	int closed = 0;
	bool newLevel = false;
	ASTNode() {

	}

	ASTNode(int id, std::string type, int liNr1, std::string value = "") {  //dla wezlow takich jak variable lub call
		this->value = value;
		this->type = type;
		this->liNr1 = liNr1;
		this->id = id;
	}
};

class NewLevelNode: public ASTNode {
public:
	NewLevelNode(int id, std::string type, int liNr1, std::string value = "") :
			ASTNode(id, type, liNr1, value) {
		newLevel = true;
	}
};

#endif
