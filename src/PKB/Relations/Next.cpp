#include "Next.h"

Next::Next() {
	maxId = -1;
}

Next::~Next() {
	// TODO Auto-generated destructor stub
}

void Next::addNext(int liNr1, int nextliNr) {

	if(liNr1 > maxId)
		maxId = liNr1;
	if(nextliNr > maxId)
		maxId = nextliNr;

	if(nextLines.count(liNr1) > 0) { //warunek niedopuszczajacy do powtorzen
		vector<int> temp = nextLines[liNr1];
		for (vector<int>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
			if((*iter) == nextliNr)
				return;
		}
	}
	nextLines[liNr1].push_back(nextliNr);
}

bool Next::next(int liNr1, int nextliNr) {
	if(nextLines.count(liNr1) == 0)
		return false;

	for (vector<int>::iterator iter = nextLines[liNr1].begin();
				iter != nextLines[liNr1].end(); ++iter) {
		if((*iter) == nextliNr)
			return true;
	}

	return false;
}


bool Next::nextS(int liNr1, int nextliNr) {
	bool visited[maxId+1];
	for(int i=0; i<maxId+1; i++)
		visited[i] = false;

	return nextSRecur(liNr1, nextliNr, visited);
}

bool Next::nextSRecur(int liNr1, int nextliNr, bool visited[]) {
	if(next(liNr1, nextliNr))
		return true;

	if(nextLines.count(liNr1) > 0) {
		for (vector<int>::iterator iter = nextLines[liNr1].begin(); iter != nextLines[liNr1].end(); ++iter) {
			if(!visited[(*iter)]) {
				visited[(*iter)] = true;
				return nextSRecur((*iter), nextliNr, visited);
			}
		}
	}
	return false;
}


void Next::wypiszWszystkie() {
	for (map<int, vector<int> >::iterator iter = nextLines.begin(); iter != nextLines.end(); ++iter) {
		vector<int> wektorPomocniczy = (*iter).second;
		cout << (*iter).first << " :";
		for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
			cout << " " << wektorPomocniczy[i];
		}
		cout << endl;
	}
}

