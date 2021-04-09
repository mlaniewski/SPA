#include "DesignExtractor.h"

#include "../globals.h"

DesignExtractor::DesignExtractor() {
}

DesignExtractor::~DesignExtractor() {
	// TODO Auto-generated destructor stub
}

void DesignExtractor::start() {
	setRelacjeFollow();
	setRelacjeParent();
	setTablicaPetle();
	setTablicaAssign();
	setRelacjeModifies();
	setRelacjeUses();
	setLinieIf();
	setLinieCall();
	setTablicaProcedur();
	setRelacjeCalls();
	setRelacjeNext();
}

void DesignExtractor::setRelacjeFollow() {

	Follows * follows = pkb->getFollows();
	ASTTree * ASTtree = pkb->getDrzewoAST();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin)) {
			if (!(*begin)->data) {
				cout << "error <fillNextFollows>" << endl;
			} else {
				tmp = begin.node->next_sibling;
				if (ASTtree->czyPrawidlowe(tmp)) {
					if ((*begin)->data->liNr1 != (*tmp)->data->liNr1
							&& ((*begin)->data->type != "ELSE" && (*tmp)->data->type != "ELSE")) {
						follows->addNext((*begin)->data->liNr1, (*tmp)->data->liNr1);
						follows->addPrev((*tmp)->data->liNr1, (*begin)->data->liNr1);
					}
				}
			}
		}
		++begin;
	}
}
void DesignExtractor::setRelacjeParent() {
	Parent * parent = pkb->getParent();
	ASTTree * ASTtree = pkb->getDrzewoAST();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator prevSib;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin)) {
			if (!(*begin)->data) {
				cout << "error <fillParents>" << endl;
			} else {
				tmp = begin.node->parent;        //zmienna pomocnicza to rodzic a poczatek dziecko
				if (ASTtree->czyPrawidlowe(tmp) && (*tmp)->data->type != "PROCEDURE"
						&& (*tmp)->data->type != "PROGRAM") {
					if ((*tmp)->data->type == "ELSE" && (*begin)->data->type != "ELSE") {
						prevSib = tmp.node->prev_sibling;
						if (ASTtree->czyPrawidlowe(prevSib)) {
							parent->addParent((*begin)->data->liNr1,
									(*prevSib)->data->liNr1);
							parent->addKid((*prevSib)->data->liNr1,
									(*begin)->data->liNr1);
						}
					} else if ((*begin)->data->liNr1
							!= (*tmp)->data->liNr1 && (*begin)->data->type != "ELSE") {
						parent->addParent((*begin)->data->liNr1,
								(*tmp)->data->liNr1);
						parent->addKid((*tmp)->data->liNr1,
								(*begin)->data->liNr1);
					}

				}
			}
		}
		++begin;

	}
}
void DesignExtractor::setTablicaPetle() {
	LinesTable * linesTable = pkb->getTablicaLiniiKodu();
	ASTTree * ASTtree = pkb->getDrzewoAST();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin) && (*begin)->data->type == "WHILE") {
			if (!(*begin)->data) {
				cout << "error <fillLoops>" << endl;
			} else {
				int i = 0;
				while(i < ASTtree->getLiczbaDzieci(begin)) {
					if (i == 0) {
						tmp = begin.node->first_child;
					} else {
						tmp = tmp.node->next_sibling;
					}
					if (ASTtree->czyPrawidlowe(tmp)) {
						if ((*tmp)->data->type == "IF"
								|| (*tmp)->data->type == "ELSE"
								|| (*tmp)->data->type == "WHILE") {
							whileRecur(tmp, begin);
						} else
							linesTable->addWhileLine((*begin)->data->liNr1,
									(*tmp)->data->liNr1);
					}
					i++;
				}

			}
		}

		++begin;
	}

}
void DesignExtractor::whileRecur(tree<tree_node_<ASTNode*>*>::iterator current,
		tree<tree_node_<ASTNode*>*>::iterator whileNode) {

	tree<tree_node_<ASTNode*>*>::iterator tmp;
	ASTTree * ASTtree = pkb->getDrzewoAST();
	LinesTable * linesTable = pkb->getTablicaLiniiKodu();

	linesTable->addWhileLine((*whileNode)->data->liNr1,
			(*current)->data->liNr1);
	int i = 0;
	while(i < ASTtree->getLiczbaDzieci(current)) {
		tmp = ASTtree->getDziecko(current, i);
		if (ASTtree->czyPrawidlowe(tmp)) {

			if ((*tmp)->data->type == "IF" || (*tmp)->data->type == "ELSE"
					|| (*tmp)->data->type == "WHILE") {
				whileRecur(tmp, whileNode);

			} else {
				linesTable->addWhileLine((*whileNode)->data->liNr1,
						(*tmp)->data->liNr1);
			}

		}
		i++;
	}

}
void DesignExtractor::setTablicaAssign() {
	LinesTable * linesTable = pkb->getTablicaLiniiKodu();
	ASTTree * ASTtree = pkb->getDrzewoAST();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin) && (*begin)->data->type == "ASSIGN") {
			linesTable->addAssignLine((*begin)->data->liNr1);
		}
		++begin;

	}
}
void DesignExtractor::setRelacjeModifies() {

	Modifies * modifies = pkb->getModifies();
	ASTTree * ASTtree = pkb->getDrzewoAST();
	VarTable * varTable = pkb->getTablicaZmiennych();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin) && (*begin)->data->type == "ASSIGN") {
			//linesTable->addAssignLine((*begin)->data->liNr1);
			tmp = begin.node->first_child;
			int varId = varTable->getZmiennaId((*tmp)->data->value);
			if (varId != -1)
				modifies->add(varId, (*begin)->data->liNr1);
			else
				modifies->add(varTable->addVar((*tmp)->data->value),
						(*begin)->data->liNr1);
		}
		++begin;

	}

}

bool czyLiczba(string s) {
	unsigned i=0;
	while(i<s.size()) {
    	if(s[i] < 48 || s[i] > 57){
    		return false;
    	}
		i++;
    }
    return true;
}

void DesignExtractor::setRelacjeUses() {
	Uses * uses = pkb->getUses();
	ASTTree * ASTtree = pkb->getDrzewoAST();
	VarTable * varTable = pkb->getTablicaZmiennych();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator firstChildOfAssign;
	tree<tree_node_<ASTNode*>*>::iterator secondChildOfAssign;
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin)) {
			if ((*begin)->data->type == "ASSIGN") {       //jezeli ustawiona jest sama wartosc
				firstChildOfAssign = begin.node->first_child;
				secondChildOfAssign = firstChildOfAssign.node->next_sibling;
				if (ASTtree->czyPrawidlowe(secondChildOfAssign) && (*secondChildOfAssign)->data->type == "OPERAND") {
					int varId = varTable->getZmiennaId((*secondChildOfAssign)->data->value);
					if (varId != -1)
						uses->add(varId, (*begin)->data->liNr1);
					else if(!czyLiczba((*begin)->data->value))
						uses->add(varTable->addVar((*begin)->data->value), (*begin)->data->liNr1);
				}
			} else if ((*begin)->data->type == "WHILE") {
				int varId = varTable->getZmiennaId((*begin)->data->value);
				if (varId != -1)
					uses->add(varId, (*begin)->data->liNr1);
				else if(!czyLiczba((*begin)->data->value))
					uses->add(varTable->addVar((*begin)->data->value), (*begin)->data->liNr1);
			} else if ((*begin)->data->type == "IF") {
				int varId = varTable->getZmiennaId((*begin)->data->value);
				if (varId != -1)
					uses->add(varId, (*begin)->data->liNr1);
				else if(!czyLiczba((*begin)->data->value))
					uses->add(varTable->addVar((*begin)->data->value), (*begin)->data->liNr1);
			} else if ((*begin)->data->type == "OPERAND") {   
				tmp = begin.node->parent;
				if ((*tmp)->data->type == "EXPR") {
					int varId = varTable->getZmiennaId((*begin)->data->value);
					if (varId != -1)
						uses->add(varId, (*begin)->data->liNr1);
					else if(!czyLiczba((*begin)->data->value))
						uses->add(varTable->addVar((*begin)->data->value), (*begin)->data->liNr1);
				}
			}
		}
		++begin;
	}
}

void DesignExtractor::setLinieIf() {

	ASTTree * ASTtree = pkb->getDrzewoAST();
	LinesTable * linesTable = pkb->getTablicaLiniiKodu();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();

	tree<tree_node_<ASTNode*>*>::iterator ifstmt;
	tree<tree_node_<ASTNode*>*>::iterator elsetmt;

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin) && (*begin)->data->type == "IF") {
			if (!(*begin)->data) {
				cout << "error <set If's>" << endl;
			} else {

				ifstmt = begin;
				elsetmt = begin.node->next_sibling;

				if (ASTtree->czyPrawidlowe(ifstmt))
					for (int i = 0; i < ASTtree->getLiczbaDzieci(ifstmt); //operacje statement pod ifem
							i++) {
						if (i == 0) {
							tmp = ifstmt.node->first_child;
						} else {
							tmp = tmp.node->next_sibling;
						}
						if (ASTtree->czyPrawidlowe(tmp)) {
							if ((*tmp)->data->type == "IF"
									|| (*tmp)->data->type == "ELSE"
									|| (*tmp)->data->type == "WHILE") {
								ifRecur(tmp, begin);
							} else
								linesTable->addIfLine(
										(*begin)->data->liNr1,
										(*tmp)->data->liNr1);
						}

					}
				if (ASTtree->czyPrawidlowe(elsetmt))
					for (int i = 0; i < ASTtree->getLiczbaDzieci(elsetmt); //pod elsem
							i++) {
						if (i == 0) {
							tmp = elsetmt.node->first_child;
						} else {
							tmp = tmp.node->next_sibling;
						}
						if (ASTtree->czyPrawidlowe(tmp)) {
							if ((*tmp)->data->type == "IF"
									|| (*tmp)->data->type == "ELSE"
									|| (*tmp)->data->type == "WHILE") {
								ifRecur(tmp, begin);
							} else
								linesTable->addIfLine(
										(*begin)->data->liNr1,
										(*tmp)->data->liNr1);
						}

					}

			}
		}

		++begin;
	}
}
void DesignExtractor::ifRecur(tree<tree_node_<ASTNode*>*>::iterator current,
		tree<tree_node_<ASTNode*>*>::iterator ifNode) {

	tree<tree_node_<ASTNode*>*>::iterator tmp;
	ASTTree * ASTtree = pkb->getDrzewoAST();
	LinesTable * linesTable = pkb->getTablicaLiniiKodu();

	linesTable->addIfLine((*ifNode)->data->liNr1,
			(*current)->data->liNr1);
	int i = 0;
	while(i < ASTtree->getLiczbaDzieci(current)) {
		tmp = ASTtree->getDziecko(current, i);
		if (ASTtree->czyPrawidlowe(tmp)) {

			if ((*tmp)->data->type == "IF" || (*tmp)->data->type == "ELSE"
					|| (*tmp)->data->type == "WHILE") {
				ifRecur(tmp, ifNode);

			} else {
				linesTable->addIfLine((*ifNode)->data->liNr1,
						(*tmp)->data->liNr1);
			}

		}
		i++;
	}

}
void DesignExtractor::setLinieCall() {

	ASTTree * ASTtree = pkb->getDrzewoAST();
	LinesTable * linesTable = pkb->getTablicaLiniiKodu();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin) && (*begin)->data->type == "CALL") {
			if (!(*begin)->data) {
				cout << "error <set call's>" << endl;
			} else {
				linesTable->addCallLine((*begin)->data->liNr1,
						(*begin)->data->value);
			}
		}
		++begin;
	}

}

void DesignExtractor::setRelacjeCalls() {
	ASTTree * ASTtree = pkb->getDrzewoAST();
	ProcTable * procTable = pkb->getTablicaProcedur();
	Calls* calls = pkb->getCalls();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();
	int idProcedury = -1;

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin) && (*begin)->data) {
			if ((*begin)->data->type == "PROCEDURE") {
				idProcedury = procTable->getIdProcedury((*begin)->data->value);
			} else if (idProcedury != -1 && (*begin)->data->type == "CALL") {
				calls->addCall(idProcedury,
						procTable->getIdProcedury((*begin)->data->value));
			}
		}
		++begin;
	}
}

void DesignExtractor::setTablicaProcedur() {

	ASTTree * ASTtree = pkb->getDrzewoAST();
	ProcTable * procTable = pkb->getTablicaProcedur();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();
	tree<tree_node_<ASTNode*>*>::iterator tmp;

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin) && (*begin)->data->type == "PROCEDURE") {
			if (!(*begin)->data) {
				cout << "error <set procedures's>" << endl;
			} else {
				procTable->addProc(((*begin)->data->value),
						((*begin)->data->liNr1));

				//pobieranie dzieci
				int idProcedury = procTable->getIdProcedury((*begin)->data->liNr1);
				for (int i = 0; i < ASTtree->getLiczbaDzieci(begin); i++) {

					tmp = ASTtree->getDziecko(begin, i);
					if (ASTtree->czyPrawidlowe(tmp)) {

						if ((*tmp)->data->type == "IF"
								|| (*tmp)->data->type == "ELSE"
								|| (*tmp)->data->type == "WHILE") {
							procRecur(tmp, idProcedury);

						} else {
							procTable->addProcBodyLine(
									idProcedury,
									(*tmp)->data->liNr1);
						}

					}
				}

			}
		}
		++begin;
	}
}

void DesignExtractor::procRecur(tree<tree_node_<ASTNode*>*>::iterator current,int idProcedury) {

	tree<tree_node_<ASTNode*>*>::iterator tmp;
	ASTTree * ASTtree = pkb->getDrzewoAST();
	ProcTable * procTable = pkb->getTablicaProcedur();

	procTable->addProcBodyLine(idProcedury,
			(*current)->data->liNr1);
	int i = 0;
	while(i < ASTtree->getLiczbaDzieci(current)) {
		tmp = ASTtree->getDziecko(current, i);
		if (ASTtree->czyPrawidlowe(tmp)) {

			if ((*tmp)->data->type == "IF" || (*tmp)->data->type == "ELSE"
					|| (*tmp)->data->type == "WHILE") {
				procRecur(tmp, idProcedury);

			} else {
				procTable->addProcBodyLine(idProcedury,
						(*tmp)->data->liNr1);
			}

		}
		i++;
	}
}

void DesignExtractor::setRelacjeNext() {
	ASTTree * ASTtree = pkb->getDrzewoAST();
	Next * next = pkb->getNext();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getKorzenDrzewa();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getKoniec();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator tmp2;

	while (begin != end) {
		if (ASTtree->czyPrawidlowe(begin) && (*begin)->data) {
			if ((*begin)->data->type == "ASSIGN" || (*begin)->data->type == "CALL") {
				tmp = begin.node -> next_sibling;
				if(ASTtree->czyPrawidlowe(tmp) && (*tmp)->data) {
					next->addNext((*begin)->data->liNr1, (*tmp)->data->liNr1);
				}
				else {
					tmp = begin.node -> parent;
					if(ASTtree->czyPrawidlowe(tmp) && (*tmp)->data) {
						if((*tmp)->data->type == "WHILE") {
							next->addNext((*begin)->data->liNr1, (*tmp)->data->liNr1);
						}
						else if((*tmp)->data->type == "IF" || (*tmp)->data->type == "ELSE") {
							while(true) {
								if((*tmp)->data->type == "ELSE") {
									tmp = tmp.node -> prev_sibling;
								}
								if(!(ASTtree->czyPrawidlowe(tmp)) || !((*tmp)->data) || (*tmp)->data->type != "IF") {
									if((*tmp)->data->type == "WHILE") {
										next->addNext((*begin)->data->liNr1, (*tmp)->data->liNr1);
									}
									break;
								}
								else {
									tmp2 = tmp.node -> next_sibling;
									if(ASTtree->czyPrawidlowe(tmp2) && (*tmp2)->data) {
										if((*tmp2)->data->type != "ELSE") {
											next->addNext((*begin)->data->liNr1, (*tmp2)->data->liNr1);
											break;
										}
										else {
											tmp2 = tmp.node -> next_sibling -> next_sibling;
											if(ASTtree->czyPrawidlowe(tmp2) && (*tmp2)->data) {
												next->addNext((*begin)->data->liNr1, (*tmp2)->data->liNr1);
												break;
											}
										}
									}
								}
								tmp = tmp.node -> parent;
							}
						}
					}
				}
			}
			else if ((*begin)->data->type == "WHILE") {
				tmp = begin.node -> first_child;
				if(ASTtree->czyPrawidlowe(tmp) && (*tmp)->data) {
					next->addNext((*begin)->data->liNr1, (*tmp)->data->liNr1);
				}
				tmp = begin.node -> next_sibling;
				if(ASTtree->czyPrawidlowe(tmp) && (*tmp)->data) {
					next->addNext((*begin)->data->liNr1, (*tmp)->data->liNr1);
				}
			}
			else if ((*begin)->data->type == "IF") {
				tmp = begin.node -> first_child;
				if(ASTtree->czyPrawidlowe(tmp) && (*tmp)->data) {
					next->addNext((*begin)->data->liNr1, (*tmp)->data->liNr1);
				}
				tmp2 = begin.node -> next_sibling;

				if(ASTtree->czyPrawidlowe(tmp2) && (*tmp2)->data && (*tmp2)->data->type == "ELSE") {
					tmp = tmp2.node -> first_child;
					if(ASTtree->czyPrawidlowe(tmp) && (*tmp)->data) {
						next->addNext((*begin)->data->liNr1, (*tmp)->data->liNr1);
					}
				}
			}
		}
		++begin;
	}
}
