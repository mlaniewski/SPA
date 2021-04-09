#ifndef SRC_PKB_TABLES_TABLICAZMIENNYCH_H_
#define SRC_PKB_TABLES_TABLICAZMIENNYCH_H_

#include<string>
#include<vector>
#include <map>
#include <iostream>

using namespace std;

class VarTable {
public:
	VarTable();
	virtual ~VarTable();
	virtual int addVar(string name);
	virtual int getZmiennaId(string name);
	virtual string getNazwaZmiennej(int id);
	virtual void wypiszWszystkie();
	virtual vector<string> getAllVar();

private:
	vector<string> variables;
	int varId;
};

#endif /* SRC_TABLES_VARTABLE_H_ */
