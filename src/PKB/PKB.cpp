#include "PKB.h"

PKB::~PKB() {
	delete modifies;
	delete uses;
	delete parent;
	delete follows;
	delete calls;
	delete next;
	delete varTable;
}

