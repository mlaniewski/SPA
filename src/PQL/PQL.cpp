#include "PQL.h"
using namespace std;

PQL::PQL() {
	// TODO Auto-generated constructor stub
}

PQL::~PQL() {
	// TODO Auto-generated destructor stub
}

//usuwanie spacji z lewej
void PQL::ltrim(string &str) {
	str.erase(str.begin(), find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace))));
}

void PQL::rtrim(string &str) {
	str.erase(find_if(str.rbegin(), str.rend(), not1(ptr_fun<int, int>(isspace))).base(), str.end());
}

void PQL::trim(string &str) {
	ltrim(str);
	rtrim(str);
}

void PQL::usunBialeZnaki(string &str) {
	str.erase(remove_if(str.begin(), str.end(), ptr_fun<int, int>(isspace)), str.end());
}


string PQL::getZapytanie()
{
	return this->query;
}

string PQL::processQuery(string query)
{
	string processedQuery;
	processedQuery = query;
	trim(processedQuery);
	return processedQuery;
}

void PQL:: wpiszZapytanie()
{

	string query2;
	getline(cin,query);
	query+=" ";
	getline(cin,query2);
	query+=query2;

	query=processQuery(query);
}
