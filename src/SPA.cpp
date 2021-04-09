#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <istream>
#include <algorithm>
#include <map>
#include <iterator>
#include <stack>
#include <cstdlib>
#include <exception>
using namespace std;
#include "exceptions.h"
#include "keywords.h"
#include "PQL/tree_util.hh"
#include "PQL/PQLNode.h"
#include "PQL/PQLTree.h"
#include "QueryPreProcessor/QueryPreProcessor.h"
#include "PQL/PQL.h"
#include "QueryEvaluator/QueryEvaluator.h"
#include "keywords.h"
#include "PKB/AST/tree_util.hh"
#include "PKB/AST/ASTNode.h"
#include "PKB/AST/ASTTree.h"
#include "Parser/Matchers.h"
#include "Parser/NodeUtil.h"
#include "Parser/Syntax.h"
#include "Parser/Parser.h"
#include "globals.h"


bool debug = false;

void queryFunction() {
	cout << "Ready\n"; // potrzebne do pipe tester
	//std::abort();

	QueryPreProcessor* qpp = new QueryPreProcessor();
	PQL *pql = new PQL();



	pql->wpiszZapytanie();//funkcja do wczytywania zapytania



	vector<string> wyniki;
	try {
		qpp->parseQuery(pql->getZapytanie());

		tree<tree_node_<PQLNode>>::iterator iter;
		PQLTree* tree = qpp->getDrzewo();


		QueryEvaluator* queEva = new QueryEvaluator();
		wyniki = queEva->getWynik(tree);

		for (size_t i = 0; i < wyniki.size(); i++) {
			if(i>0) cout << ", ";
			cout << wyniki[i];
		}
		if(wyniki.size() == 0) cout<< "none";

	} catch(const char* ex) {
		vector<string> tmp;
		string query = pql->getZapytanie();
		if(query.find("BOOLEAN") <= query.length())
			wyniki.push_back("false");
		else
			wyniki = tmp;
		cerr << ex << endl;
	}

	vector<Field> entities;
	Field* f1 = new Field();
	string s1 = "stmt", s2 = "s", s3 = "variable", s4 = "v";
	f1->setTyp(s1);
	f1->setWartosc(s2);
	Field* f2 = new Field();
	f2->setTyp(s3);
	f2->setWartosc(s4);
	entities.push_back(*f1);
	entities.push_back(*f2);
}

void checkKeyWord() {
	Syntax* s = new ProcedureSyntax();
	Syntax::allSynstax[s->keyWord] = s;
	Syntax* w = new WhileSyntax();
	Syntax::allSynstax[w->keyWord] = w;
	Syntax* op = new OperandSyntax();
	Syntax::allSynstax[op->keyWord] = op;
	Syntax* callS = new CallSyntax();
	Syntax::allSynstax[callS ->keyWord] = callS ;
	Syntax* yiff = new IfSyntax();
	Syntax::allSynstax[yiff->keyWord] = yiff;
	Syntax* elseS = new ElseSyntax();
	Syntax::allSynstax[elseS->keyWord] =elseS;
	Syntax* empty = new EmptySyntax();
	Syntax::allSynstax[empty->keyWord] = empty;

	RecursiveSyntax* ass = new AssingmentSyntax();
	Syntax::allSynstax[ass->keyWord] = ass;
	RecursiveSyntax* plus = new MathSyntax("+");
	Syntax::allSynstax[plus->keyWord] = plus;
	RecursiveSyntax* minus = new MathSyntax("-");
	Syntax::allSynstax[minus->keyWord] = minus;
	RecursiveSyntax* multi = new MathSyntax("*", true);
	Syntax::allSynstax[multi->keyWord] = multi;
	RecursiveSyntax* div = new MathSyntax("/");
	Syntax::allSynstax[div->keyWord] = div;
	RecursiveSyntax* par = new ParentsisesSyntax();
	Syntax::allSynstax[par->keyWord] = par;

	ass->parsers.push_back(plus);
	ass->parsers.push_back(minus);
	ass->parsers.push_back(div);
	ass->parsers.push_back(multi);
	ass->parsers.push_back(par);

	plus->parsers.push_back(plus);
	plus->parsers.push_back(minus);
	plus->parsers.push_back(div);
	plus->parsers.push_back(multi);
	plus->parsers.push_back(par);

	minus->parsers.push_back(plus);
	minus->parsers.push_back(minus);
	minus->parsers.push_back(div);
	minus->parsers.push_back(multi);
	minus->parsers.push_back(par);

	div->parsers.push_back(plus);
	div->parsers.push_back(minus);
	div->parsers.push_back(div);
	div->parsers.push_back(multi);
	div->parsers.push_back(par);

	multi->parsers.push_back(plus);
	multi->parsers.push_back(minus);
	multi->parsers.push_back(div);
	multi->parsers.push_back(multi);
	multi->parsers.push_back(par);

	par->parsers.push_back(plus);
	par->parsers.push_back(minus);
	par->parsers.push_back(div);
	par->parsers.push_back(multi);
	par->parsers.push_back(par);
}



PKB * pkb = new PKB();
DesignExtractor * designExtractor = new DesignExtractor();


int main(int argc, char** args) {
	if (argc <= 1) {
		cout << "Nie podano odpowiednich argument�w";
		return 0;
	}
//W args[0] jest sciezka do exe.
	string action = string(args[1]);
	 {
		checkKeyWord();
		Parser parser(args[1]);
		if (!parser.fileExists) {
			cout << "Nie znaleziono pliku\n";
		} else {

			try {

							parser.parse();
					//		cout << "OK" << endl;
							pkb->setDrzewoAST(parser.root);

							designExtractor->start();


						/*	cout << "Program:\n";
										pkb->getTablicaLiniiKodu()->wypiszWszystkie();
*/

						} catch (RuntimeException &ex) {
							ex.print();
							cout << "BłąD" << endl;
						} catch (Exception &ex) {
							ex.print();
							cout << "B��D" << endl;
						}





		}
	}

		//cout << "Ready\n"; // potrzebne do pipe tester

	 queryFunction();//}


	return 0;
}
