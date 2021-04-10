#ifndef SRC_PKB_API_PKBAPI_H_
#define SRC_PKB_API_PKBAPI_H_

#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

class PKBApi {
public:
	PKBApi();
	virtual ~PKBApi();

	int getLiczbaLinii();
	vector<int> getLinieAssign();
	map<int, string> getWszystkieLinie();
	vector<int> getLinieWhile();
	vector<int> getLinieIf();
	map<int, vector<int> > getLinieZawartosciWhile();
	map<int, vector<int>> getLinieZawartosciIf();
	vector<int> getProceduresLines();
	string getLineString(int liNr1);

	int getIdProcedury(string name);
	int getIdProcedury(int startLine);
	int getMaxIdProcedury();
	int getPierwszaLiniaProcedury(int id);
	int getPierwszaLiniaProcedury(string name);
	vector<int> getLinieProcedury(int idProcedury);
	set<int> getAllLinieProcedury(int idProcedury);
	string getNazwaProcedury(int id);


	bool follows(int liNr, int followliNr);
	bool followsS(int liNr, int followliNr);
	bool modifies(int liNr1, string varName);
	bool parent(int parentliNr, int kidliNr);
	bool parentS(int parentliNr, int kidliNr);
	bool uses(int liNr1, string varName);



};

#endif /* SRC_PKB_API_PKBAPI_H_ */
