#include "PKB.h"

PKB::~PKB() {
	delete modifies;
	delete uses;
	delete parent;
	delete follows;
	delete next;
	delete varTable;
}

