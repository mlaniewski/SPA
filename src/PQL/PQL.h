#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <istream>
#include <algorithm>
#include <map>
#include <iterator>
#include <stack>
#include <functional>
using namespace std;

class PQL {
private:
	string query;
public:
	PQL();
	void wpiszZapytanie();
	string getZapytanie();
	string processQuery(string query);

	void ltrim(string &str);//usuwa spacje z lewej
	void rtrim(string &str);//usuwa spacje z prawej
	void trim(string &str);//usuwa wszystkie spacje
	void usunBialeZnaki(string &str);
	string toLower(string str);


	virtual ~PQL();

};

