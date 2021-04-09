#ifndef SRC_PKB_TABLES_LINESTABLE_H_
#define SRC_PKB_TABLES_LINESTABLE_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>
#ifndef _GLIBCXX_SET
#include <set>
#endif

using namespace std;

class LinesTable {
public:
	LinesTable();
	virtual ~LinesTable();
	int getLiczbaLinii();
	string getWywolanaNazwaProcedury(int callLine);
	vector<int> getLinieAssign();
	set<int> getOrderedAssignLines();
	map<int, string> getWszystkieLinie();
	vector<int> getLines();
	vector<int> getLinieCall();
	set<int> getOrderedCallLines();
	vector<int> getLinieWhile();
	set<int> getOrderedWhileLines();
	vector<int> getLinieIf();
	set<int> getOrderedIfLines();
	map<int, vector<int> > getLinieZawartosciWhile();
	map<int, vector<int>> getLinieZawartosciIf();
	string getLineString(int liNr1); // zwraca tekst z danej linii
	void addWhileLine(int loopLine,int stmtLine);
	void addIfLine(int ifLine, int stmtLine);
	void addCallLine(int callLine,string nazwaProcedury);
	void addAssignLine(int assignLine);
	void addLine(string text);
	void wypiszWszystkie();
	void writeIfLines();
	void writeCallLines();
	void writeWhileLines();


private:
	int linesCount;
	map<int, string> lines;
	vector<int> assignLines;
	vector<int> callLines;
	map<int,string> pkbCallLines;
	map<int, vector<int>> whileLines;
	map<int, vector<int>> ifLines;

};

#endif /* SRC_PKB_TABLES_LINESTABLE_H_ */
