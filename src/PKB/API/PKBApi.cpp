#include "PKBApi.h"

#include "../../globals.h"

PKBApi::PKBApi() {

}

PKBApi::~PKBApi() {
	// TODO Auto-generated destructor stub
}



int PKBApi::getLiczbaLinii(){
	return pkb -> getTablicaLiniiKodu() -> getLiczbaLinii();
}

vector<int> PKBApi::getLinieAssign(){
	return pkb -> getTablicaLiniiKodu() -> getLinieAssign();
}

map<int, string> PKBApi::getWszystkieLinie(){
	return pkb -> getTablicaLiniiKodu() -> getWszystkieLinie();
}

vector<int> PKBApi::getLinieCall(){
	return pkb -> getTablicaLiniiKodu() -> getLinieCall();
}

vector<int> PKBApi::getLinieWhile(){
	return pkb -> getTablicaLiniiKodu() -> getLinieWhile();
}

vector<int> PKBApi::getLinieIf(){
	return pkb -> getTablicaLiniiKodu() -> getLinieIf();
}

vector<int> PKBApi::getProceduresLines(){
	return pkb -> getTablicaProcedur() -> getProceduresLines();
}

map<int, vector<int>> PKBApi::getLinieZawartosciIf(){
	return pkb -> getTablicaLiniiKodu() -> getLinieZawartosciIf();
}

map<int, vector<int> > PKBApi::getLinieZawartosciWhile(){
	return pkb -> getTablicaLiniiKodu() -> getLinieZawartosciWhile();
}

string PKBApi::getLineString(int liNr1){
	return pkb -> getTablicaLiniiKodu() -> getLineString(liNr1);
}



int PKBApi::getIdProcedury(string name) {
	return pkb -> getTablicaProcedur() -> getIdProcedury(name);
}

int PKBApi::getIdProcedury(int startLine) {
	return pkb -> getTablicaProcedur() -> getIdProcedury(startLine);
}

int PKBApi::getMaxIdProcedury() {
	return pkb -> getTablicaProcedur() -> getMaxIdProcedury();
}

int PKBApi::getPierwszaLiniaProcedury(int id) {
	return pkb -> getTablicaProcedur() -> getPierwszaLiniaProcedury(id);
}

int PKBApi::getPierwszaLiniaProcedury(string name) {
	return pkb -> getTablicaProcedur() -> getPierwszaLiniaProcedury(name);
}

string PKBApi::getNazwaProcedury(int id) {
	return pkb -> getTablicaProcedur() -> getNazwaProcedury(id);
}

vector<int> PKBApi::getLinieProcedury(int idProcedury) {
	return pkb -> getTablicaProcedur() -> getLinieProcedury(idProcedury);
}

bool PKBApi::follows(int liNr, int followliNr){
	return pkb -> getFollows() -> follows(liNr, followliNr);
}

bool PKBApi::followsS(int liNr, int followliNr) {
	return pkb -> getFollows() -> followsS(liNr, followliNr);
}

bool PKBApi::modifies(int liNr1, string varName){
	return pkb -> getModifies() -> modifies(liNr1, varName);
}

bool PKBApi::parent(int parentliNr, int kidliNr){
	return pkb -> getParent() -> parent(parentliNr, kidliNr);
}

bool PKBApi::parentS(int parentliNr, int kidliNr) {
	return pkb -> getParent() -> parentS(parentliNr, kidliNr);
}
bool PKBApi::uses(int liNr1, string varName){
	return pkb -> getUses() -> uses(liNr1, varName);
}






