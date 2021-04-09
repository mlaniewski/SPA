#ifndef _STDLIB_H_
#include <stdlib.h>
#endif
#include <iostream>
#include <sstream>
#include "Field.h"

Field::Field() {
	this->nazwaProcedury = false;
	this->nazwaZmiennej = false;
	this->wartosc = false;
	this->statement = false;
}

Field::Field(string type, string value) {
	this->type = type;
	this->value = value;
	this->nazwaProcedury = false;
	this->nazwaZmiennej = false;
	this->wartosc = false;
	this->statement = false;
}

Field::Field(string type, string value, bool nazwaProcedury, bool varName, bool val,
		bool stmt) {
	this->type = type;
	this->value = value;
	this->nazwaProcedury = nazwaProcedury;
	this->nazwaZmiennej = varName;
	this->wartosc = val;
	this->statement = stmt;
}

Field::~Field() {
	// TODO Auto-generated destructor stub
}

bool Field::czyStatement() {
	return statement;
}

void Field::setStatement(bool stmt) {
	this->statement = stmt;
}

string& Field::getTyp() {
	return type;
}

void Field::setTyp(string& type) {
	this->type = type;
}

bool Field::getWartosc1() {
	return wartosc;
}

void Field::setWartosc1(bool val) {
	this->wartosc = val;
}

string& Field::getWartosc() {
	return value;
}

int Field::stringNaInt() {
	return std::stoi(value);
}

void Field::setWartosc(string& value) {
	this->value = value;
}

bool Field::czyNazwaProcedury() {
	return nazwaProcedury;
}

void Field::setNazwaProcedury(bool nazwaProcedury) {
	this->nazwaProcedury = nazwaProcedury;
}

bool Field::czyNazwaZmiennej() {
	return nazwaZmiennej;
}

void Field::setNazwaZmiennej(bool varName) {
	this->nazwaZmiennej = varName;
}

string Field::printField()
{
	stringstream fieldText;
	fieldText << "Field: [" << this->type << " " << this->value << "] {nazwaProcedury: " << this->nazwaProcedury << ", varName: " << this->nazwaZmiennej << ", stmt#: " << this->statement << ", value: " << this->wartosc << "}" << endl;
	return fieldText.str();
}
