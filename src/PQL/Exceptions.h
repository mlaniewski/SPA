

#ifndef SRC_PQL_EXCEPTIONS_H_
#define SRC_PQL_EXCEPTIONS_H_

#include <iostream>
#include <exception>

using namespace std;

class Exceptions
{
public:
	void throwException() {
		throw "Zapytanie jest nieprawid�owe";
	}

	void throwSplitException() {
		throw "Zapytanie zosta�o niepoprawnie podzielone. Czesc zapytania jest nieprawid�owa.";
	}

	void throwUnexpectedTypeOfClause() {
		throw " Zapytanie ma nieprzewidzian� zmienn� w cz�ci 'such'";
	}

	void throwToMuchArguments() {
		throw "Jedna ze zmiennych ma wi�cej ni� dwa argumenty.";
	}

	void throwInvalidNumberOfArguments()
	{
		throw "Klauzula ma nieprawi�ow� liczb� argument�w.";
	}

	void throwInvalidTypeOfArguments()
	{
		throw "Klauzula ma nieprawid�owy typ argument�w.";
	}

	void throwInvalidWithStatement()
	{
		throw "Klauzula jest cz�ciowo niepoprawna.";
	}

	void throwInvalidWithStatementAttributes()
	{
		throw "Klauzula 'with' ma nieprawid�owe atrybuty 'equals' (np. string = integer, etc.).";
	}

	void throwUnexpectedSelectPart()
	{
		throw "Klauzula ma niezadeklarowan� cz�c select.";
	}
};

#endif /* SRC_PQL_EXCEPTIONS_H_ */
