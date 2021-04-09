#ifndef SRC_PKB_TABLES_TABLICAPROC_H_
#define SRC_PKB_TABLES_TABLICAPROC_H_

#include<string>
#include<vector>
#include<map>
#include<set>

#include "../Relations/Calls.h"
using namespace std;

class ProcTable {
public:
	ProcTable();
	virtual ~ProcTable();
	virtual int addProc(string name, int startLine);
	virtual int getIdProcedury(string name);
	virtual int getIdProcedury(int startLine);
	virtual int getMaxIdProcedury();
	virtual int getPierwszaLiniaProcedury(int id);
	virtual int getPierwszaLiniaProcedury(string name);
	virtual string getNazwaProcedury(int id);
	virtual void writeProcLines();
	virtual void writeProcBodyLines();
	virtual vector<int> getProceduresLines();
	virtual map<int, vector<int>> getProceduresBodyLines();
	virtual vector<int> getLinieProcedury(int idProcedury);
	virtual set<int> getAllLinieProcedury(int idProcedury, Calls * calls);
	virtual void addProcBodyLine(int procStartLine, int stmtLine);

private:
	vector<string> procedures;
	vector<int> proceduresStartLines;
	map<int, vector<int>> proceduresLines;
	int idProcedury;

	set<int> procedureRecur(int idProcedury, Calls * calls, set<int> procedureLines, bool visited[]);

};

#endif /* SRC_TABLES_PROCTABLE_H_ */
