#include "TablicaZmiennych.h"

VarTable::VarTable() {
	// TODO Auto-generated constructor stub
	varId = 0;
}

VarTable::~VarTable() {
	// TODO Auto-generated destructor stub
}

int VarTable::addVar(string name){
	variables.push_back(name);
	varId++;
	return varId - 1;
}

int VarTable::getZmiennaId(string name){
	for(unsigned i=0; i<variables.size(); i++){
		if(variables[i].compare(name) == 0){
			return i;
		}
	}
	return -1;
}

string VarTable::getNazwaZmiennej(int id){
	return variables[id];
}

void VarTable::wypiszWszystkie(){
	for(int i = 0; i < variables.size() ; i++){
		cout << i <<" "<<variables[i]<<endl;
	}
}

vector<string> VarTable::getAllVar() {
	return variables;
}
