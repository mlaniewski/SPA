#include "Calls.h"

Calls::Calls() {
	maxId = -1;

}

Calls::~Calls() {
	// TODO Auto-generated destructor stub
}

void Calls::addCall(int callerId, int calleeId){

	if(callerId > maxId)
		maxId = callerId;
	if(calleeId > maxId)
			maxId = calleeId;

	if(callIds.count(callerId) > 0) {
		vector<int> temp = callIds[callerId];
		for (vector<int>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
			if((*iter) == calleeId)
				return;
		}
	}
	callIds[callerId].push_back(calleeId);
}

bool Calls::calls(int callerId, int calleeId){

	if(callerId == -1 && calleeId == -1){
		return !callIds.empty();
	}

	if(callerId == -1){
		for (map<int, vector<int>>::iterator iter = callIds.begin();
						iter != callIds.end(); ++iter) {
			vector<int> x = (*iter).second;
			for (vector<int>::iterator iter = x.begin();
						iter != x.end(); ++iter) {
				if((*iter) == calleeId)
					return true;
			}
		}
		return false;
	}

	if(callIds.count(callerId) == 0)
		return false;

	if(calleeId == -1){
		vector<int> x = callIds[callerId];
		return !x.empty();
	}

	for (vector<int>::iterator iter = callIds[callerId].begin();
				iter != callIds[callerId].end(); ++iter) {
		if((*iter) == calleeId)
			return true;
	}

	return false;
}

bool Calls::callsS(int callerId, int calleeId){
	bool visited[maxId+1];
	for(int i=0; i<maxId+1; i++)
		visited[i] = false;

	return callsSRecur(callerId, calleeId, visited);
}

bool Calls::callsSRecur(int callerId, int calleeId, bool visited[]){
	if(calls(callerId, calleeId))
		return true;

	if(callIds.count(callerId) > 0){
		for (vector<int>::iterator iter = callIds[callerId].begin(); iter != callIds[callerId].end(); ++iter) {
			if(!visited[(*iter)]){
				visited[(*iter)] = true;
				return callsSRecur((*iter), calleeId, visited);
			}
		}
	}

	return false;
}

vector<int> Calls::getCall(int callerId) {
	if(callIds.count(callerId) > 0){
		return callIds[callerId];
	}
	vector<int> empty;
	return empty;
}

void Calls::wypiszWszystkie(){
	for (map<int, vector<int> >::iterator iter = callIds.begin();
				iter != callIds.end(); ++iter) {
			vector<int> wektorPomocniczy = (*iter).second;
			cout << (*iter).first << " :";
			for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
				cout << " " << wektorPomocniczy[i];
			}
			cout << endl;
		}
}
