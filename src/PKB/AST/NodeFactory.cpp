#include "NodeFactory.h"

NodeFactory::NodeFactory() {
	// TODO Auto-generated constructor stub

}

int NodeFactory::counter = 0;

NodeFactory::~NodeFactory() {
	// TODO Auto-generated destructor stub
}
ASTNode* NodeFactory::CreateNode(std::string type, int liNr1, std::string value){
	return new ASTNode(++counter,type,liNr1,value);
}

NewLevelNode* NodeFactory:: CreateNewLeveLNode(std::string type, int liNr1, std::string value ){
	return new NewLevelNode(++counter,type,liNr1,value);
}
