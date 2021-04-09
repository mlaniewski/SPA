#ifndef SRC_PKB_RELATIONS_NEXT_H_
#define SRC_PKB_RELATIONS_NEXT_H_

#include <map>
#include <vector>
#include <iostream>

using namespace std;

class Next {
public:
	Next();
	void addNext(int liNr1, int nextliNr);
	bool next(int liNr1, int nextliNr);
	bool nextS(int liNr1, int nextliNr);//Next*
	virtual ~Next();
	void wypiszWszystkie();

private:
	int maxId;
	map <int, vector<int> > nextLines;

	bool nextSRecur(int id1, int calleeId, bool visited[]);
};

#endif /* SRC_PKB_RELATIONS_NEXT_H_ */
