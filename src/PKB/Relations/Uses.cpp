#include "Uses.h"

#include "../../globals.h"

Uses::Uses() {

}

Uses::~Uses() {
	// TODO Auto-generated destructor stub
}

void Uses::add(int varId, int liNr1){
	varUsesLines[varId].push_back(liNr1);
}

bool Uses::uses(int liNr1, string varName){
	if (varName == "") {
		for (map<int, vector<int> >::iterator iter = varUsesLines.begin(); iter != varUsesLines.end(); ++iter) {
			vector<int> wektorPomocniczy = (*iter).second;
			for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
				if(wektorPomocniczy[i] == liNr1){
					return true;
				}
			}
		}
		return false;
	}

	int varId = pkb -> getTablicaZmiennych() -> getZmiennaId(varName);
	if (varId == -1)
		return false;
	if (varUsesLines.count(varId) > 0) {
		for (vector<int>::iterator iter = varUsesLines.at(varId).begin(); iter != varUsesLines.at(varId).end(); ++iter) {
			if ((*iter) == liNr1)
				return true;
		}
	}
	return false;
}

void Uses::wypiszWszystkie(){
	for( map<int, vector<int> >::iterator iter = varUsesLines.begin(); iter != varUsesLines.end(); ++iter ) {
	   vector<int> wektorPomocniczy = (*iter).second;
	   int key = (*iter).first;
	   cout << pkb->getTablicaZmiennych()->getNazwaZmiennej(key) << " :";
	   for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
	      cout << " " << wektorPomocniczy[i];
	   }
	   cout << endl;
	}
}

map<int,vector<int>> Uses::getAllUses()
{
	return varUsesLines;
}
