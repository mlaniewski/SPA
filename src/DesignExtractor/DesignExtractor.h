#ifndef SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_
#define SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_
#include "../PKB/AST/tree.hh"
#include "../PKB/AST/ASTNode.h"
#include "../PKB/Relations/Modifies.h"
#include "../PKB/Tables/TablicaZmiennych.h"

class DesignExtractor {

private:
	void setRelacjeFollow();
	void setRelacjeParent();
	void setRelacjeModifies();
	void setRelacjeUses();
	void setRelacjeCalls();
	void setRelacjeNext();

	void setTablicaPetle();
	void setTablicaAssign();
	void setTablicaProcedur();

	void setLinieIf();
	void setLinieCall();

	void whileRecur(tree<tree_node_<ASTNode*>*>::iterator current,
			tree<tree_node_<ASTNode*>*>::iterator whileNode);
	void ifRecur(tree<tree_node_<ASTNode*>*>::iterator current,
			tree<tree_node_<ASTNode*>*>::iterator ifNode);
	void procRecur(tree<tree_node_<ASTNode*>*>::iterator current,
			int idProcedury);

public:
	DesignExtractor();
	virtual ~DesignExtractor();
	void start();

};

#endif
