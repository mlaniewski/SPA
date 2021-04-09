#ifndef SRC_PKB_RELATIONS_USES_H_
#define SRC_PKB_RELATIONS_USES_H_


#include <map>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Uses {
public:
	Uses();
	void add(int varId, int liNr1);
	bool uses(int liNr1, string varName);
	void wypiszWszystkie();
	virtual ~Uses();
	virtual map<int,vector<int>> getAllUses();

private:
	map <int, vector<int> > varUsesLines;
};

#endif /* SRC_PKB_RELATIONS_USES_H_ */
