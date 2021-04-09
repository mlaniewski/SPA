#ifndef SRC_PKB_RELATIONS_FOLLOWS_H_
#define SRC_PKB_RELATIONS_FOLLOWS_H_


#include <map>
#include <iostream>
using namespace std;

class Follows {
public:
	Follows();
	void addNext(int liNr, int followliNr);
	void addPrev(int liNr, int prevliNr);
	bool follows(int liNr, int followliNr);
	bool followsS(int liNr, int followliNr);//Follows*
	virtual ~Follows();

	void wypiszWszystkie();
private:
	map <int, int > varNextFollowers;
	map <int, int > varPrevFollowers;
};


#endif /* SRC_PKB_RELATIONS_FOLLOWS_H_ */
