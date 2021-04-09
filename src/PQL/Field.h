#ifndef SRC_TREE_FIELD_H_
#define SRC_TREE_FIELD_H_

#include <string>

using namespace std;

class Field {
public:
	Field();
	Field(string type, string value);
	Field(string type, string value, bool nazwaProcedury, bool nazwaZmiennej, bool wartosc, bool statement);
	virtual ~Field();
	bool czyStatement();
	void setStatement(bool statement);
	string& getTyp();
	void setTyp(string& type);
	bool getWartosc1();
	void setWartosc1(bool wartosc);
	string& getWartosc();
	int stringNaInt();
	void setWartosc(string& value);
	bool czyNazwaProcedury();
	void setNazwaProcedury(bool nazwaProcedury);
	bool czyNazwaZmiennej();
	void setNazwaZmiennej(bool nazwaZmiennej);
	string printField();


private:
	string type;  //typy procedure,assign, variable, prog_line, string,any, constant,stmt#
	string value;
	//wszystko jest na false
	bool nazwaProcedury;
	bool nazwaZmiennej;
	bool wartosc;
	bool statement;
};

#endif /* SRC_TREE_FIELD_H_ */
