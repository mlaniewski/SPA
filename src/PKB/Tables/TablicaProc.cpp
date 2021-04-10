#include "TablicaProc.h"

#include <iostream>
ProcTable::ProcTable() {
	idProcedury = 0;
}

ProcTable::~ProcTable() {
	// TODO Auto-generated destructor stub
}

int ProcTable::addProc(string name, int startLine) {
	procedures.push_back(name);
	proceduresStartLines.push_back(startLine);
	idProcedury++;
	return idProcedury - 1;
}
void ProcTable::addProcBodyLine(int idProcedury, int stmtLine) {
	proceduresLines[idProcedury].push_back(stmtLine);
}
map<int, vector<int>> ProcTable::getProceduresBodyLines() {
	return proceduresLines;
}

int ProcTable::getIdProcedury(string name) {
	for (unsigned i = 0; i < procedures.size(); i++) {
		if (procedures[i].compare(name) == 0) {
			return i;
		}
	}
	return -1;
}

int ProcTable::getIdProcedury(int startLine) {
	for (unsigned i = 0; i < proceduresStartLines.size(); i++) {
		if (proceduresStartLines[i] == startLine) {
			return i;
		}
	}
	return -1;
}

int ProcTable::getMaxIdProcedury() {
	return idProcedury - 1;
}

int ProcTable::getPierwszaLiniaProcedury(int id) {
	return proceduresStartLines[id];
}

int ProcTable::getPierwszaLiniaProcedury(string name) {
	return proceduresStartLines[getIdProcedury(name)];
}

string ProcTable::getNazwaProcedury(int id) {
	return procedures[id];
}
void ProcTable::writeProcLines() {
	for (vector<string>::iterator iter = procedures.begin();
			iter != procedures.end(); ++iter) {
		cout << (*iter) << " ";
	}
	cout << endl;
	for (vector<int>::iterator iter = proceduresStartLines.begin();
			iter != proceduresStartLines.end(); ++iter) {
		cout << (*iter) << " ";
	}
	cout << endl;
}
void ProcTable::writeProcBodyLines() {
	for (map<int, vector<int> >::iterator iter = proceduresLines.begin();
			iter != proceduresLines.end(); ++iter) {
		vector<int> wektorPomocniczy = (*iter).second;
		int key = (*iter).first;
		cout << key << " :";
		for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
			cout << " " << wektorPomocniczy[i];
		}
		cout << endl;
	}

}

vector<int> ProcTable::getProceduresLines() {
	return proceduresStartLines;
}

vector<int> ProcTable::getLinieProcedury(int procId) {
	if (proceduresLines.count(procId) > 0)
		return proceduresLines[procId];

	vector<int> empty;
	return empty;
}
