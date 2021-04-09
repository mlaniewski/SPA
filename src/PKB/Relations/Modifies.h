#ifndef SRC_PKB_RELATIONS_MODIFIES_H_
#define SRC_PKB_RELATIONS_MODIFIES_H_

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Modifies {
public:
	Modifies();
	void add(int varId, int liNr1);
	bool modifies(int liNr1, string varName);
	virtual ~Modifies();

	void wypiszWszystkie();
	map<int, vector<int>> getWszystkieModifies();

private:
	map <int, vector<int> > varModificationsLines;
};

#endif /* SRC_PKB_RELATIONS_MODIFIES_H_ */
