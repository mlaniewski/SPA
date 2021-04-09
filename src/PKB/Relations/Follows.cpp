#include "Follows.h"

Follows::Follows() {
	// TODO Auto-generated constructor stub

}

Follows::~Follows() {
	// TODO Auto-generated destructor stub
}

void Follows::addNext(int lineNum,int followLineNum){
	varNextFollowers[lineNum] = followLineNum;
}

void Follows::addPrev(int lineNum,int prevliNr){
	varPrevFollowers[lineNum] = prevliNr;
}

bool Follows::follows(int lineNum, int followliNr){
	if(varNextFollowers.count(lineNum)>0){
		return varNextFollowers.at(lineNum) == followliNr;
	}
	return false;
}

bool Follows::followsS(int lineNum,int followliNr){
	if(follows(lineNum, followliNr))
		return true;

	if(varNextFollowers.count(lineNum)>0){
		return followsS(varNextFollowers[lineNum], followliNr);
	}

	return false;
}

void Follows::wypiszWszystkie(){
	for (map<int, int>::iterator iter = varNextFollowers.begin(); iter != varNextFollowers.end(); ++iter) {
		cout << (*iter).first << " --> " << (*iter).second <<endl;
	}

/*	cout << endl;

	for (map<int, int>::iterator iter = varPrevFollowers.begin(); iter != varPrevFollowers.end(); ++iter) {
		cout << (*iter).first << " <-- " << (*iter).second <<endl;
	}*/
}
