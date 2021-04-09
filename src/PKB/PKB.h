#ifndef SRC_PKB_PKB_H_
#define SRC_PKB_PKB_H_

#include <iostream>
#include "AST/tree_util.hh"

#include "AST/ASTNode.h"
#include "AST/ASTTree.h"
#include "Relations/Calls.h"
#include "Relations/Follows.h"
#include "Relations/Modifies.h"
#include "Relations/Next.h"
#include "Relations/Parent.h"
#include "Relations/Uses.h"
#include "Relations/Uses.h"
#include "Tables/LinesTable.h"
#include "Tables/TablicaProc.h"
#include "Tables/TablicaZmiennych.h"

class PKB {
public:

	PKB(){
		modifies = new Modifies();
		uses = new Uses();
		parent = new Parent();
		follows = new Follows();
		calls = new Calls();
		next = new Next();
		varTable = new VarTable();
		lineTable = new LinesTable();
		procTable = new ProcTable();
	}

	virtual ~PKB();

	Follows* getFollows() {
		return follows;
	}

	Modifies* getModifies() {
		return modifies;
	}

	Parent* getParent() {
		return parent;
	}

	Uses* getUses() {
		return uses;
	}

	Calls* getCalls() {
		return calls;
	}

	Next* getNext() {
		return next;
	}

	ASTTree* getDrzewoAST(){
		return tree;
	}

	VarTable* getTablicaZmiennych(){
		return varTable;
	}

	LinesTable* getTablicaLiniiKodu(){
		return lineTable;
	}

	ProcTable* getTablicaProcedur(){
		return procTable;
	}

	void setDrzewoAST(ASTTree * tree2){
		tree = tree2;
	}
private:
	Modifies* modifies = NULL;
	Uses* uses = NULL;
	Parent* parent = NULL;
	Follows* follows = NULL;
	Calls* calls = NULL;
	Next* next = NULL;
	ASTTree * tree = NULL;
	VarTable * varTable = NULL;
	LinesTable * lineTable = NULL;
	ProcTable * procTable = NULL;
};

#endif /* SRC_PKB_PKB_H_ */
