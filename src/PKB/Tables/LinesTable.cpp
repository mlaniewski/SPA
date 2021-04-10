#include "LinesTable.h"

LinesTable::LinesTable() {
	// TODO Auto-generated constructor stub
	linesCount = 0;
}

LinesTable::~LinesTable() {
	// TODO Auto-generated destructor stub
}

int LinesTable::getLiczbaLinii() {
	return linesCount;
}

map<int, string> LinesTable::getWszystkieLinie() {
	return lines;
}
vector<int> LinesTable::getLines() {
	vector<int> v;
	for (map<int, string>::iterator it = lines.begin(); it != lines.end();
			++it) {
		v.push_back(it->first);
	}
	return v;
}

vector<int> LinesTable::getLinieWhile() {
	vector<int> v;
	for (map<int, vector<int>>::iterator it = whileLines.begin();
			it != whileLines.end(); ++it) {
		v.push_back(it->first);
	}
	return v;
}

set<int> LinesTable::getOrderedWhileLines() {
	set<int> result;
	for (map<int, vector<int>>::iterator it = whileLines.begin();
			it != whileLines.end(); ++it) {
		result.insert(it->first);
	}
	return result;
}

vector<int> LinesTable::getLinieIf() {
	vector<int> v;
	for (map<int, vector<int>>::iterator it = ifLines.begin();
			it != ifLines.end(); ++it) {
		v.push_back(it->first);
	}
	return v;
}
set<int> LinesTable::getOrderedIfLines() {
	set<int> result;
	for (map<int, vector<int>>::iterator it = ifLines.begin();
			it != ifLines.end(); ++it) {
		result.insert(it->first);
	}
	return result;
}

map<int, vector<int>> LinesTable::getLinieZawartosciIf() {
	return ifLines;
}

vector<int> LinesTable::getLinieAssign() {
	return assignLines;
}
set<int> LinesTable::getOrderedAssignLines() {
	set<int> result;
	result.insert(assignLines.begin(), assignLines.end());
	return result;
}

map<int, vector<int>> LinesTable::getLinieZawartosciWhile() {
	return whileLines;
}

string LinesTable::getLineString(int liNr1) {
	if (lines.count(liNr1) > 0)
		return lines[liNr1];
	return "";
}

void LinesTable::addWhileLine(int loopLine, int stmtLine) {
	whileLines[loopLine].push_back(stmtLine);
}

void LinesTable::addIfLine(int ifLine, int stmtLine) {
	ifLines[ifLine].push_back(stmtLine);
}

void LinesTable::addAssignLine(int assignLine) {
	assignLines.push_back(assignLine);
}

void LinesTable::addLine(string text) {
	linesCount++;
	lines[linesCount] = text;
}

void LinesTable::wypiszWszystkie() {
	for (map<int, string>::iterator iter = lines.begin(); iter != lines.end();
			++iter) {
		cout << (*iter).first << " : " << (*iter).second << endl;
	}
}

void LinesTable::writeIfLines() {
	for (map<int, vector<int> >::iterator iter = ifLines.begin();
			iter != ifLines.end(); ++iter) {
		vector<int> wektorPomocniczy = (*iter).second;
		int key = (*iter).first;
		cout << key << " :";
		for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
			cout << " " << wektorPomocniczy[i];
		}
		cout << endl;
	}
}
void LinesTable::writeWhileLines() {
	for (map<int, vector<int> >::iterator iter = whileLines.begin();
			iter != whileLines.end(); ++iter) {
		vector<int> wektorPomocniczy = (*iter).second;
		int key = (*iter).first;
		cout << key << " :";
		for (unsigned i = 0; i < wektorPomocniczy.size(); i++) {
			cout << " " << wektorPomocniczy[i];
		}
		cout << endl;
	}
}
