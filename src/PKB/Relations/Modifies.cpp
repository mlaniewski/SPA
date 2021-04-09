#include "Modifies.h"

#include "../../globals.h"
Modifies::Modifies() {
	// TODO Auto-generated constructor stub

}

Modifies::~Modifies() {
	// TODO Auto-generated destructor stub
}

void Modifies::add(int varId, int liNr1) {
	varModificationsLines[varId].push_back(liNr1);

}

bool Modifies::modifies(int liNr1, string varName) {
	if (varName == "") {
		for (map<int, vector<int> >::iterator iter =
				varModificationsLines.begin();
				iter != varModificationsLines.end(); ++iter) {
			vector<int> wektorPomocniczy = (*iter).second;
			for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
				if (wektorPomocniczy[i] == liNr1) {
					return true;
				}
			}
		}
		return false;
	}

	int varId = pkb->getTablicaZmiennych()->getZmiennaId(varName);
	if (varId == -1)
		return false;
	if (varModificationsLines.count(varId) > 0) {
		for (vector<int>::iterator iter =
				varModificationsLines.at(varId).begin();
				iter != varModificationsLines.at(varId).end(); ++iter) {
			if ((*iter) == liNr1)
				return true;
		}
	}
	return false;
}

void Modifies::wypiszWszystkie() {
	for (map<int, vector<int> >::iterator iter = varModificationsLines.begin();
			iter != varModificationsLines.end(); ++iter) {
		vector<int> wektorPomocniczy = (*iter).second;
		int key = (*iter).first;
		cout << pkb->getTablicaZmiennych()->getNazwaZmiennej(key) << " :";
		for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
			cout << " " << wektorPomocniczy[i];
		}
		cout << endl;
	}
}

map<int, vector<int>> Modifies::getWszystkieModifies() {
	return varModificationsLines;
}
