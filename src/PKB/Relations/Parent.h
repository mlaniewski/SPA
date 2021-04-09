#ifndef SRC_PKB_RELATIONS_PARENT_H_
#define SRC_PKB_RELATIONS_PARENT_H_

#include <map>
#include <string>
#include <iostream>

#include "../Tables/LinesTable.h"
using namespace std;

class Parent {
public:
	Parent();
	void addParent(int liNr1, int parentliNr);
	void addKid(int liNr1, int kidliNr);
	bool parent(int parentliNr, int kidliNr);
	bool parentS(int parentliNr, int kidliNr);//Parent*
	virtual ~Parent();
	void wypiszWszystkie();

private:
	map <int, int> lineParent;
	map <int, vector<int> > lineKids;
};

#endif /* SRC_PKB_RELATIONS_PARENT_H_ */
