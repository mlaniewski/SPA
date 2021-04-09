#ifndef SRC_PKB_RELATIONS_CALLS_H_
#define SRC_PKB_RELATIONS_CALLS_H_

#include <map>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Calls {
public:
	Calls();
	void addCall(int id1, int id2);
	bool calls(int id1, int id2);
	bool callsS(int id1, int id2);//Calls*
	virtual ~Calls();
	vector<int> getCall(int id1);
	void wypiszWszystkie();

private:
	int maxId;
	map <int, vector<int> > callIds;

	bool callsSRecur(int id1, int id2, bool visited[]);

};

#endif /* SRC_PKB_RELATIONS_CALLS_H_ */
