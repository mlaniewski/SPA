#ifndef SRC_PQL_MATCHERPQL_H_
#define SRC_PQL_MATCHERPQL_H_

#include <vector>
#include <iostream>
#include "../PQL/Field.h"

using namespace std;

class MatcherPQL {
public:
	MatcherPQL();
	virtual ~MatcherPQL();
	vector<int> findPosition(vector<string> &elems, string token);
	bool checkTokens(string elem, string token) {
		//cout << elem.find(token) << endl;
		if (elem.find(token) > elem.length())
			return false;
		else
			return true;
	}

	bool checkVarName(string elem) {
		return (elem.find(".varName") < elem.length());
	}

	bool checkNazwaProcedury(string elem) {
		return (elem.find(".nazwaProcedury") < elem.length());
	}

	bool checkStmt_(string elem) {
		return (elem.find(".stmt#") < elem.length());
	}

	bool checkValue(string elem) {
		return (elem.find(".value") < elem.length());
	}

	bool checkBOOLEAN(string elem) {
		return (elem.find("BOOLEAN") < elem.length());
	}

	bool checkAll(string elem) {
		if (!checkVarName(elem) && !checkNazwaProcedury(elem) && !checkStmt_(elem) && !checkValue(elem) && !checkBOOLEAN(elem))
			return false;
		else
			return true;
	}

	bool checkWithAttributes(Field* field1, Field* field2)
	{
		string type1 = "error";
		if(field1->getTyp() == "procedure" && field1->czyNazwaProcedury()) type1 = "string";
		if(field1->getTyp() == "variable" && field1->czyNazwaZmiennej()) type1 = "string";
		if(field1->getTyp() == "constant") type1 = "integer";
		if((field1->getTyp() == "stmt"
			|| field1->getTyp() == "assign"
			|| field1->getTyp() == "while"
			|| field1->getTyp() == "if"
			|| field1->getTyp() == "prog_line") && field1->czyStatement()) type1 = "integer";
		if(field1->getTyp() == "string") type1 = "string";

		string type2 = "error";
		if(field2->getTyp() == "procedure" && field2->czyNazwaProcedury()) type2 = "string";
		if(field2->getTyp() == "variable" && field2->czyNazwaZmiennej()) type2 = "string";
		if(field2->getTyp() == "constant") type2 = "integer";
		if((field2->getTyp() == "stmt"
			|| field2->getTyp() == "assign"
			|| field2->getTyp() == "while"
			|| field2->getTyp() == "if"
			|| field2->getTyp() == "prog_line") && field2->czyStatement()) type2 = "integer";
		if(field2->getTyp() == "string") type2 = "string";

		if(type1 == "string" && type2 == "string")
			return true;
		else if(type1 == "integer" && type2 == "integer")
			return true;
		else
			return false;
	}

	vector<string> getTokensList()
	{
		return {"assign","stmtlst","stmt","while","variable","constant","prog_line","if","procedure"};
	}

	string checkSuchThatType(string suchThatPart) {
		if(checkTokens(suchThatPart,"Parent")) return "parent";
		if(checkTokens(suchThatPart,"Follows")) return "follows";
		if(checkTokens(suchThatPart,"Modifies")) return "modifies";
		if(checkTokens(suchThatPart,"Uses")) return "uses";

		return "";
	}

	bool isString(string elem)
	{
		int pos1 = elem.find("\"");
		int pos2 = elem.find("\"",pos1+1);
		int pos3 = elem.find("\"",pos2+1);

		if(pos1 < pos2 && pos1 + 1 != pos2 && pos3 > elem.length())
			return true;
		else
			return false;
	}

	bool isStar(string elem, int position)
	{
		if(elem.find("*",position) < elem.length() && elem.find("*",position) == position)
			return true;
		else
			return false;
	}

	bool hasTwoElem(string elem)
	{
		int pos1 = elem.find(",");
		int pos2 = elem.find(",",pos1+1);

		if(pos1 < elem.length() && pos2 > elem.length())
			return true;
		else
			return false;
	}

	bool withHasTwoElem(string elem)
	{
		int pos1 = elem.find("=");
		int pos2 = elem.find("=",pos1+1);

		if(pos1 < elem.length() && pos2 > elem.length())
			return true;
		else
			return false;
	}

	bool isBracket(string elem)
	{
		int pos1 = elem.find("(");
		int pos2 = elem.find(")");

		if(pos1 == 0 && pos2 == elem.length() - 1)
			return true;
		else
			return false;
	}

	int getPosition(string elem, string token)
	{
		return elem.find(token);
	}

	bool is_(string elem)
	{
		if(elem.find("_") < elem.length())
			return true;
		else
			return false;
	}

	bool czyLiczba(string elem)
	{
		for(int i = 0 ; i < elem.length() ; i ++)
		{
			if(!isdigit(elem[i]))
				return false;
		}
		return true;
	}
};
#endif /* SRC_PQL_MATCHERPQL_H_ */
