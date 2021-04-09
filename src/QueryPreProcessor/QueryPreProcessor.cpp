#include "QueryPreProcessor.h"

#include "../PQL/MatcherPQL.h"

using namespace std;

QueryPreProcessor::QueryPreProcessor() {
	matcher = new MatcherPQL();
	exc = new Exceptions();
}

QueryPreProcessor::~QueryPreProcessor() {
	delete matcher;
	delete exc;
}

void QueryPreProcessor::parseQuery(string query) {
	vector<string> first_split = split(query, ';'); // kod do dzielenia zapytania na czesci, przed i po sredniku
	vector<string> result_part;
	vector<string> query_part;


	MatcherPQL* m = new MatcherPQL();
	size_t i = 0;
	while(i < first_split.size()) {
		if(!m->checkTokens(first_split[i], "Select"))
			result_part.push_back(first_split[i]);
		else
			query_part.push_back(first_split[i]);
		i++;
	}
	delete m;


	if(query_part.size() == 0) {
		exc->throwException();
	}


	vector<Field> fields = makeFields(result_part);
	setFields(fields);


	makeTree(query_part);

}
