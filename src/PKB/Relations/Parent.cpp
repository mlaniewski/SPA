#include "Parent.h"

Parent::Parent() {
	// TODO Auto-generated constructor stub

}

Parent::~Parent() {
	// TODO Auto-generated destructor stub
}

void Parent::addParent(int liNr1, int parentliNr) {
	lineParent[liNr1] = parentliNr;
}

void Parent::addKid(int liNr1, int kidliNr) {
	lineKids[liNr1].push_back(kidliNr);
}

bool Parent::parent(int parentliNr, int kidliNr) {
	if (lineParent.count(kidliNr) > 0){
		return lineParent.at(kidliNr) == parentliNr;
	}
	return false;
}

bool Parent::parentS(int parentliNr, int kidliNr) {
	if (parent(parentliNr, kidliNr)){
		return true;
	}

	if (lineParent.count(kidliNr) > 0) {
		return parentS(parentliNr, lineParent.at(kidliNr));
	}

	return false;
}

void Parent::wypiszWszystkie() {
	for (map<int, vector<int> >::iterator iter = lineKids.begin();
			iter != lineKids.end(); ++iter) {
		vector<int> wektorPomocniczy = (*iter).second;
		int key = (*iter).first;
		cout << key << " :";
		for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
			cout << " " << wektorPomocniczy[i];
		}
		cout << endl;
	}

/*	cout << "\nparents\n";

	for (map<int, int>::iterator iter = lineParent.begin();
			iter != lineParent.end(); ++iter) {
		int value = (*iter).second;
		int key = (*iter).first;
		cout << key << " : " << value << endl;
	}*/
}

