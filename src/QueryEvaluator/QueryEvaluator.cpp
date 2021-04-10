#include "QueryEvaluator.h"

#include <set>
#include <iterator>
#include <map>

#include "../globals.h"
#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif
#ifndef SRC_PQL_PQLTREE_H_
#include "../PQL/PQLTree.h"
#endif

namespace std {

QueryEvaluator::QueryEvaluator() {
	// Automatyczne generowanie konstruktora

}

QueryEvaluator::~QueryEvaluator() {
	// Automatyczne generowanie destruktora
}

vector<string> QueryEvaluator::getWynik(PQLTree *Tree) {
	vector<string> result;
	vector<int> lines;
	set<int> setLines;
	string selectValue;
	tree<tree_node_<PQLNode*>*>::iterator begin = Tree->Tree->begin();
	tree<tree_node_<PQLNode*>*>::iterator end = Tree->Tree->end();
	bool isModifies = false;
	bool isUses = false;

	pierwszyUses = true;

	//WYSZUKIWANIE ODPOWIEDZI

	while (begin != end) {

			if (((*begin)->data->getType()) == "resultNode") {
				string s = (*begin)->data->getField1()->getTyp();
				wynikType = s;
				if (s == "assign") {
					lines = pkb->getTablicaLiniiKodu()->getLinieAssign();
					selectValue = (*begin)->data->getField1()->getWartosc();
				} else if (s == "while") {
					lines = pkb->getTablicaLiniiKodu()->getLinieWhile();
					selectValue = (*begin)->data->getField1()->getWartosc();
				} else if (s == "variable") {
					lines = pkb->getTablicaLiniiKodu()->getLines();
					selectValue = (*begin)->data->getField1()->getWartosc();
				} else if (s == "prog_line") {
					lines = pkb->getTablicaLiniiKodu()->getLines();
					selectValue = (*begin)->data->getField1()->getWartosc();
				} else if (s == "procedure") {
					lines = pkbApi->getProceduresLines();
					selectValue = (*begin)->data->getField1()->getWartosc();
				} else if (s == "stmt" || s == "boolean") {
					vector<int> tmp2;
					tmp2 = pkb->getTablicaLiniiKodu()->getLinieAssign();
					setLines.insert(tmp2.begin(),tmp2.end());
					tmp2 = pkb->getTablicaLiniiKodu()->getLinieWhile();
					setLines.insert(tmp2.begin(),tmp2.end());
					tmp2 = pkb->getTablicaLiniiKodu()->getLinieCall();
					setLines.insert(tmp2.begin(),tmp2.end());
					tmp2 = pkb->getTablicaLiniiKodu()->getLinieIf();
					setLines.insert(tmp2.begin(),tmp2.end());
					std::copy(setLines.begin(), setLines.end(), std::inserter( lines, lines.end() ) );
					selectValue = (*begin)->data->getField1()->getWartosc();
				} else if (s == "if") {
					lines = pkb->getTablicaLiniiKodu()->getLinieIf();
					selectValue = (*begin)->data->getField1()->getWartosc();
				}
			}

			if (((*begin)->data->getType()) == "suchNode") {
				//Zweryfikowanie wystapienia relacji Modifies
				if ((*begin)->data->getNodeType() == "modifies") {
					lines = getModifiesWynik((*begin)->data->getField1(),
							(*begin)->data->getField2(), lines, selectValue);
					isModifies = true;
				}
				//Zweryfikowanie wystąpienia relacji Parent lub Parent*
				if ((*begin)->data->getNodeType() == "parent") {
					//Pareznt z *
					if ((*begin)->data->isStar()) {
						lines = getParentSWynik((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
					}
					//Parent bez *
					else {
						lines = getParentWynik((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
					}
				}
				//Zweryfikowanie wystąpienia relacji Follows lub Follows*
				if ((*begin)->data->getNodeType() == "follows") {
					//Follows *
					if ((*begin)->data->isStar()) {
						lines = getFollowsSWynik((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
					}
					//Follows bez *
					else {
						lines = getFollowsWynik((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
					}
				}
				//Zweryfikowanie wystąpienia relacji Uses lub Uses*
				if ((*begin)->data->getNodeType() == "uses") {
					//Uses z *
					if ((*begin)->data->isStar()) {
					}
					//Uses bez *
					else {
						lines = getUsesWynik((*begin)->data->getField1(), (*begin)->data->getField2(), lines, selectValue);
						isUses = true;
					}
				}
				// Zweryfikowanie wystąpienia relacji Calls lub Calls*
				if ((*begin)->data->getNodeType() == "calls") {
					//Calls z *
					if ((*begin)->data->isStar()) {
						lines = getCallSWynik((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
					} 
					//Calls bez *
					else {
						lines = getCallWynik((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
					}
				}

				if ((*begin)->data->getNodeType() == "affects" || (*begin)->data->getNodeType() == "next") {
					vector<int> empty;
					lines = empty;
				}
			}

			if(((*begin)->data->getType()) == "withNode") {
				getWithWynik((*begin)->data->getField1(), (*begin)->data->getField2(), lines);
			}
		++begin;
	}

	result.clear();


	if(!lines.empty())
	{
		if(wynikType == "boolean")
		{
			result.push_back("true");
		}
		else
		{
			for(size_t i = 0 ; i < lines.size() ; i++)
			{
 				if(wynikType == "procedure")
				{
					string name = pkbApi -> getNazwaProcedury(pkbApi -> getIdProcedury(lines[i]));
					if(find(result.begin(), result.end(), name) >= result.begin())
						result.push_back(name);
				}
 				else if(wynikType == "variable")
				{
					if(isUses && !isModifies) // Czy występuje tylko relacja -> Uses
					{
						for (set<std::pair<string,int>>::iterator it = usesPairs.begin(); it != usesPairs.end(); ++it) {
							if((*it).second == lines[i] && find(result.begin(),result.end(),(*it).first) == result.end())
							{
								result.push_back((*it).first);
							}
						}
					}

					map<int,vector<int>> varUsesLines = pkb -> getUses() -> getAllUses();
					map<int,vector<int>> varModifiesLines = pkb -> getModifies() -> getWszystkieModifies();
					vector<int> variableIds;
					if(withMap.count(selectValue) > 0) variableIds = withMap[selectValue];

					if(!isUses && isModifies) // Czy występuje tylko relacja -> Modifies
					{

						for (map<int, vector<int>>::iterator it = varModifiesLines.begin(); it != varModifiesLines.end(); ++it)
						{
							vector<int> tmp = (*it).second;
							if(find(tmp.begin(), tmp.end(), lines[i]) != tmp.end())
							{
								string name = pkb -> getTablicaZmiennych() -> getNazwaZmiennej((*it).first);
								if(find(result.begin(), result.end(), name) == result.end() && (find(variableIds.begin(),variableIds.end(), (*it).first) != variableIds.end() || variableIds.empty()))
									result.push_back(name);
							}
						}
					}
					else if(isUses && isModifies) // Wystapienie obu relacji (Uses i Modifies)
					{
						vector<string> resultUses;
						for (map<int, vector<int>>::iterator it = varUsesLines.begin(); it != varUsesLines.end(); ++it)
						{
							vector<int> tmp = (*it).second;
							if(find(tmp.begin(), tmp.end(), lines[i]) != tmp.end())
							{
								string name = pkb -> getTablicaZmiennych() -> getNazwaZmiennej((*it).first);
								if(find(resultUses.begin(), resultUses.end(), name) == resultUses.end() && (find(variableIds.begin(),variableIds.end(), (*it).first) != variableIds.end() || variableIds.empty()))
									resultUses.push_back(name);
							}
						}

						vector<string> resultModifies;
						for (map<int, vector<int>>::iterator it = varModifiesLines.begin(); it != varModifiesLines.end(); ++it)
						{
							vector<int> tmp = (*it).second;
							if(find(tmp.begin(), tmp.end(), lines[i]) != tmp.end())
							{
								string name = pkb -> getTablicaZmiennych() -> getNazwaZmiennej((*it).first);
								if(find(resultModifies.begin(), resultModifies.end(), name) == resultModifies.end() && (find(variableIds.begin(),variableIds.end(), (*it).first) != variableIds.end() || variableIds.empty()))
									resultModifies.push_back(name);
							}
						}

						for(size_t iU = 0 ; iU < resultUses.size() ; iU++)
						{
							for(size_t iM = 0 ; iM < resultModifies.size() ; iM++)
							{
								if(resultUses[iU] == resultModifies[iM] && find(result.begin(), result.end(), resultUses[iU]) == result.end())
								{
									result.push_back(resultUses[iU]);
								}
							}
						}
					}

				}
				else
				{
					stringstream ss;
					ss << lines[i];
					if(find(result.begin(), result.end(), ss.str()) >= result.begin())
						result.push_back(ss.str());
				}
			}
		}
	}
	else
	{
		if(wynikType == "boolean")
		{
			result.push_back("false");
		}
		else
		{
			//result.push_back("NONE");
		}
	}

	return result;
}

vector<int> QueryEvaluator::getModifiesWynik(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<int> setLines2;

	if(field1->getTyp() == "constant" && field2->getTyp() != "constant")
	{
		  int param1 = field1->stringNaInt();
		  vector<int> a = pkb->getTablicaLiniiKodu()->getLinieCall();
		  if(find(a.begin(),a.end(),param1) != a.end())
		  {
		   string name = pkb->getTablicaLiniiKodu()->getWywolanaNazwaProcedury(param1);
		   vector<int> b = pkb->getTablicaProcedur()->getLinieProcedury(pkb->getTablicaProcedur()->getIdProcedury(name));
		   setLines1.insert(b.begin(),b.end());
		  }
		  else if(pkb->getTablicaProcedur()->getMaxIdProcedury() >= param1)
		  {
		   string name = pkb->getTablicaProcedur()->getNazwaProcedury(param1);
		   vector<int> b = pkb->getTablicaProcedur()->getLinieProcedury(pkb->getTablicaProcedur()->getIdProcedury(name));
		   setLines1.insert(b.begin(),b.end());
		  }
		  else
		  setLines1.insert(param1);

		if(field2->getTyp() == "variable" || field2->getTyp() == "any")
		{

			vector<string> tmp = pkb->getTablicaZmiennych()->getAllVar();
			size_t i = 0;
			while(i < tmp.size()) {
				setLines2.insert(pkb->getTablicaZmiennych()->getZmiennaId(tmp[i]));
				i++;
			}
		}
		else if(field2->getTyp() == "string")
		{
			int param2 = pkb->getTablicaZmiennych()->getZmiennaId(field2->getWartosc());
			if(param2 != -1)
				setLines2.insert(param2);
		}
	}
	else if(field1->getTyp() != "constant" && field2->getTyp() != "constant")
	{
		//Zweryfikowanie atrybutu z pola 1
		if(field1->getTyp() == "if")
		{
			map<int,vector<int>> ifLinesStart = pkb->getTablicaLiniiKodu()->getLinieZawartosciIf();

			for (map<int, vector<int>>::iterator it = ifLinesStart.begin(); it != ifLinesStart.end(); ++it) {
				vector<int> ifLines = (*it).second;
				for(size_t i = 0 ; i < ifLines.size() ; i++)
				{
					setLines1.insert(ifLines[i]);
				}
			}
		}
		else if(field1->getTyp() == "while")
		{
			map<int,vector<int>> whileLinesStart = pkb->getTablicaLiniiKodu()->getLinieZawartosciWhile();

			for (map<int, vector<int>>::iterator it = whileLinesStart.begin(); it != whileLinesStart.end(); ++it) {
				vector<int> whileLines = (*it).second;
				for(size_t i = 0 ; i < whileLines.size() ; i++)
				{
					setLines1.insert(whileLines[i]);
				}
			}
		}
		else if(field1->getTyp() == "assign")
		{
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
		else if(field1->getTyp() == "procedure")
		{
			map<int,vector<int>> procLinesStart = pkb->getTablicaProcedur()->getProceduresBodyLines();

			for (map<int, vector<int>>::iterator it = procLinesStart.begin(); it != procLinesStart.end(); ++it)
			{
				vector<int> procLines = (*it).second;
				int procParam = pkb->getTablicaProcedur()->getPierwszaLiniaProcedury((*it).first);
				setLines1.insert(procParam);
				for(size_t i = 0 ; i < procLines.size() ; i++)
				{
					setLines1.insert(procLines[i]);
				}
			}
		}
		else if(field1->getTyp() == "call")
		{
			vector<int> callLinesStart = pkb->getTablicaLiniiKodu()->getLinieCall();
			for(size_t i = 0 ; i < callLinesStart.size() ; i++)
			{
				string nazwaProcedury = pkb->getTablicaLiniiKodu()->getWywolanaNazwaProcedury(callLinesStart[i]);
				int idProcedury = pkb->getTablicaProcedur()->getIdProcedury(nazwaProcedury);
				if(idProcedury != -1)
				{
					vector<int> callLines = pkb->getTablicaProcedur()->getLinieProcedury(idProcedury);

					for(size_t j = 0 ; j < callLines.size() ; j++)
					{
						setLines1.insert(callLines[j]);
					}
				}
			}
		}
		else if(field1->getTyp() == "string")
		{
			int idProcedury = pkb->getTablicaProcedur()->getIdProcedury(field1->getWartosc());
			if(idProcedury != -1)
			{
				vector<int> procLines = pkb->getTablicaProcedur()->getLinieProcedury(idProcedury);
				size_t j = 0;
				while(j < procLines.size()) {
					setLines1.insert(procLines[j]);
					j++;
				}
			}
		}
		else if(field1->getTyp() == "stmt")
		{
			vector<int> allLines = pkb->getTablicaLiniiKodu()->getLines();
			size_t j = 0;
			while(j < allLines.size()) {
				setLines1.insert(allLines[j]);
				j++;
			}
		}

		//Zweryfikowanie atrybutu z pola 2
		if(field2->getTyp() == "variable" || field2->getTyp() == "any")
		{
			vector<string> tmp = pkb->getTablicaZmiennych()->getAllVar();
			size_t i = 0;
			while(i < tmp.size()) {
				setLines2.insert(pkb->getTablicaZmiennych()->getZmiennaId(tmp[i]));
				i++;
			}
		}
		else if(field2->getTyp() == "string")
		{
			int param2 = pkb->getTablicaZmiennych()->getZmiennaId(field2->getWartosc());
			if(param2 != -1)
				setLines2.insert(param2);
		}
	}

	//Biorąc pod uwagę części zapytanie z "with" następuje skrócenie listy parametru 1
	if(field1->getTyp() != "constant" && field1->getTyp() != "any")
		setLines1 = cutSetLines(field1->getWartosc(), setLines1);
	//Biorąc pod uwagę części zapytanie z "with" następuje skrócenie listy parametru 2
	if(field2->getTyp() != "constant" && field2->getTyp() != "any")
		setLines2 = cutSetLines(field2->getWartosc(), setLines2);


	vector<int> resultPart;
	//Sprawdzenie czy wszystkie parametry były dobre przy parsowaniu lub walidacji. Jeżeli nie to zwracana jest pusta lista
	if (!setLines1.empty() && !setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if (pkb->getModifies()->modifies(*l1, pkb->getTablicaZmiennych()->getNazwaZmiennej(*l2)) == true) {
					if (selectValue == field1->getWartosc() && selectValue == field2->getWartosc() && selectValue != "boolean") {
						//Jeżeli oba parametry śa takie same, a nie są to constant, znaczy to że nie ma odpowiedzi
						return resultPart;
					} else if (selectValue == field1->getWartosc() && selectValue != "boolean") {
						//Jeśli parametr pierwszy jest tym, którego szukamy to wybieramy z listy pierwszej
						if(find(resultPart.begin(),resultPart.end(),*l1) >= resultPart.end())
						{
							resultPart.push_back(*l1);
						}
					} else if (selectValue == field2->getWartosc() && selectValue != "boolean") {
						//Jeśli parametr drugi jest tym, którego szukamy to wybieramy z listy drugiej
						if(find(resultPart.begin(),resultPart.end(),*l1) >= resultPart.end())
						{
							resultPart.push_back(*l1);
						}
					} else {
						cout<<"wlazi"<<endl;
						//Jeśli żaden parametr nie jest szukany to zwracane są wszystkie wartości
						return lines;
					}
				}
			}
		}
	}

	return resultPart;
}

vector<int> QueryEvaluator::getParentWynik(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<int> setLines2;
	//Pierwszy parametr (Field1) sprawdzany w relacji Parent
	if (field1->getTyp() == "constant" && field2->getTyp() != "constant") {
		int param = field1->stringNaInt();
		setLines1.insert(param);
		if (field2->getTyp() == "stmt" || field2->getTyp() == "any") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			set<int> tmp = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines2.insert(tmp.begin(), tmp.end());
			tmp = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines2.insert(tmp.begin(), tmp.end());
			tmp = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines2.insert(tmp.begin(), tmp.end());
		} else if (field2->getTyp() == "while") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field2->getTyp() == "if") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field2->getTyp() == "call") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field2->getTyp() == "assign") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
	} else if (field1->getTyp() != "constant"
			&& field2->getTyp() == "constant") {
		int param = field2->stringNaInt();
		setLines2.insert(param);
		if (field1->getTyp() == "stmt" || field1->getTyp() == "any") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			set<int> tmp = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines1.insert(tmp.begin(), tmp.end());
		} else if (field1->getTyp() == "while") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field1->getTyp() == "if") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		}
	} else if (field1->getTyp() == "constant"
			&& field2->getTyp() == "constant") {
		int param1 = field1->stringNaInt();
		int param2 = field1->stringNaInt();
		if (pkb->getParent()->parent(param1, param2) == true) {
			return lines;
		}
	} else {
		if (field1->getTyp() == "stmt" || field1->getTyp() == "any") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			set<int> tmp = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines1.insert(tmp.begin(), tmp.end());
		} else if (field1->getTyp() == "while") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field1->getTyp() == "if") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		}
		if (field2->getTyp() == "stmt" || field2->getTyp() == "prog_line"
				|| field2->getTyp() == "any") {
			set<int> tmp = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			setLines2.insert(tmp.begin(), tmp.end());
			tmp = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines2.insert(tmp.begin(), tmp.end());
			tmp = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines2.insert(tmp.begin(), tmp.end());
		} else if (field2->getTyp() == "while") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field2->getTyp() == "if") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field2->getTyp() == "call") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field2->getTyp() == "assign") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
	}

	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru pierwszego
	if(field1->getTyp() != "constant" && field1->getTyp() != "any")
		setLines1 = cutSetLines(field1->getWartosc(), setLines1);
	
	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru drugiego
	if(field2->getTyp() != "constant" && field2->getTyp() != "any")
		setLines2 = cutSetLines(field2->getWartosc(), setLines2);

	vector<int> resultPart;
	//Dla pobranych parametrow setLines1 oraz setLines2 sprawdzane są zależności, a następnie porównanie ich z wartościami na lines (szukana wartość)
	if (!setLines1.empty() && !setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if (pkb->getParent()->parent(*l1, *l2) == true) {
					if (selectValue == field1->getWartosc() && selectValue == field2->getWartosc() && selectValue != "boolean") {
						//Jeżeli oba parametry śa takie same, a nie są to constant, znaczy to że nie ma odpowiedzi 
						return resultPart;
					} else if (selectValue == field1->getWartosc() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
						//Jeśli parametr pierwszy jest tym, którego szukamy to wybieramy z listy pierwszej
						if(find(resultPart.begin(),resultPart.end(),*l1) >= resultPart.end())
							resultPart.push_back(*l1);
					} else if (selectValue == field2->getWartosc() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
						//Jeśli parametr drugi jest tym, którego szukamy to wybieramy z listy drugiej
						if(find(resultPart.begin(),resultPart.end(),*l2) >= resultPart.end())
							resultPart.push_back(*l2);
					} else {
						//Jeśli żaden parametr nie jest szukany to zwracane są wszystkie wartości
						return lines;
					}
				}
			}
		}
	}
	return resultPart;
}

vector<int> QueryEvaluator::getParentSWynik(Field* field1, Field* field2, vector<int> lines, string selectValue) {
		set<int> setLines1;
		set<int> setLines2;
		//Sprawdzanie pierwszego parametru (Filed1) w relacji Parent
		if (field1->getTyp() == "constant" && field2->getTyp() != "constant") {
			int param = field1->stringNaInt();
			setLines1.insert(param);
			if (field2->getTyp() == "stmt" || field2->getTyp() == "any") {
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
				set<int> tmp = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
				setLines2.insert(tmp.begin(), tmp.end());
				tmp = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
				setLines2.insert(tmp.begin(), tmp.end());
				tmp = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
				setLines2.insert(tmp.begin(), tmp.end());
			} else if (field2->getTyp() == "while") {
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			} else if (field2->getTyp() == "if") {
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			} else if (field2->getTyp() == "call") {
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			} else if (field2->getTyp() == "assign") {
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			}
		} else if (field1->getTyp() != "constant"
				&& field2->getTyp() == "constant") {
			int param = field2->stringNaInt();
			setLines2.insert(param);
			if (field1->getTyp() == "stmt" || field1->getTyp() == "any") {
				setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
				set<int> tmp = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
				setLines1.insert(tmp.begin(), tmp.end());
			} else if (field1->getTyp() == "while") {
				setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			} else if (field1->getTyp() == "if") {
				setLines1 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			}
		} else if (field1->getTyp() == "constant"
				&& field2->getTyp() == "constant") {
			int param1 = field1->stringNaInt();
			int param2 = field1->stringNaInt();
			if (pkb->getParent()->parent(param1, param2) == true) {
				return lines;
			}
		} else {
			if (field1->getTyp() == "stmt" || field1->getTyp() == "any") {
				setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
				set<int> tmp = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
				setLines1.insert(tmp.begin(), tmp.end());
			} else if (field1->getTyp() == "while") {
				setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			} else if (field1->getTyp() == "if") {
				setLines1 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			}
			if (field2->getTyp() == "stmt" || field2->getTyp() == "prog_line"
					|| field2->getTyp() == "any") {
				set<int> tmp = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
				setLines2.insert(tmp.begin(), tmp.end());
				tmp = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
				setLines2.insert(tmp.begin(), tmp.end());
				tmp = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
				setLines2.insert(tmp.begin(), tmp.end());
			} else if (field2->getTyp() == "while") {
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			} else if (field2->getTyp() == "if") {
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			} else if (field2->getTyp() == "call") {
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			} else if (field2->getTyp() == "assign") {
				setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			}
		}

		//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru pierwszego
		if(field1->getTyp() != "constant" && field1->getTyp() != "any")
			setLines1 = cutSetLines(field1->getWartosc(), setLines1);
		//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru drugiego
		if(field2->getTyp() != "constant" && field2->getTyp() != "any")
			setLines2 = cutSetLines(field2->getWartosc(), setLines2);

		vector<int> resultPart;
		//Dla pobranych parametrow setLines1 oraz setLines2 sprawdzane są zależności, a następnie porównanie ich z wartościami na lines (szukana wartość)
		if (!setLines1.empty() && !setLines2.empty()) {
			for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
				for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
					if (pkb->getParent()->parentS(*l1, *l2) == true) {
						if (selectValue == field1->getWartosc() && selectValue == field2->getWartosc() && selectValue != "boolean") {
							//Jeżeli oba parametry śa takie same, a nie są to constant, znaczy to że nie ma odpowiedzi 
							return resultPart;
						} else if (selectValue == field1->getWartosc() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
							//Jeśli parametr pierwszy jest tym, którego szukamy to wybieramy z listy pierwszej
							if(find(resultPart.begin(),resultPart.end(),*l1) >= resultPart.end())
								resultPart.push_back(*l1);
						} else if (selectValue == field2->getWartosc() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
							//Jeśli parametr drugi jest tym, którego szukamy to wybieramy z listy drugiej
							if(find(resultPart.begin(),resultPart.end(),*l2) >= resultPart.end())
								resultPart.push_back(*l2);
						} else {
							//Jeśli żaden parametr nie jest szukany to zwracane są wszystkie wartości
							return lines;
						}
					}
				}
			}
		}
		lines = resultPart;
		return lines;
}

vector<int> QueryEvaluator::getFollowsWynik(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<int> setLines2;

	//Sprawdzanie pierwszego parametru (Filed1) w relacji Follows
	if (field1->getTyp() == "constant" && field2->getTyp() != "constant") {
		int param = field1->stringNaInt();
		//Wartość constant jest dodawana do listy pierwszej
		setLines1.insert(param);
		//Sprawdzanie czym jest drugi parametr i ewentualne pobranie listy
		if (field2->getTyp() == "stmt" || field2->getTyp() == "any") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines2.insert(pom.begin(), pom.end());
		} else if (field2->getTyp() == "while") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field2->getTyp() == "if") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field2->getTyp() == "call") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field2->getTyp() == "assign") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
	} else if (field1->getTyp() != "constant"
			&& field2->getTyp() == "constant") {
		int param = field2->stringNaInt();
		//Wartość constant jest dodawana do listy drugiej
		setLines2.insert(param);
		//Sprawdzanie czym jest pierwszy parametr i ewentualne pobranie listy
		if (field1->getTyp() == "stmt" || field1->getTyp() == "any") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines1.insert(pom.begin(), pom.end());
		} else if (field1->getTyp() == "while") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field1->getTyp() == "if") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field1->getTyp() == "call") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field1->getTyp() == "assign") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
	} else if (field1->getTyp() == "constant" && field2->getTyp() == "constant") {
		int param1 = field1->stringNaInt();
		int param2 = field2->stringNaInt();
		if (pkb->getFollows()->follows(param1, param2) == true) {
			return lines;
		}
	} else {
		//Dla pierwszego parametru nastepuje pobranie z listy
		//Any jest to dowolna wartośc z dostępnych czyli stmt
		if (field1->getTyp() == "stmt" || field1->getTyp() == "any") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines1.insert(pom.begin(), pom.end());
		} else if (field1->getTyp() == "while") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field1->getTyp() == "if") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field1->getTyp() == "call") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field1->getTyp() == "assign") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
		//Pobranie listy dla drugiego parametru
		if (field2->getTyp() == "stmt" || field2->getTyp() == "any") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines2.insert(pom.begin(), pom.end());
		} else if (field2->getTyp() == "while") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field2->getTyp() == "if") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field2->getTyp() == "call") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field2->getTyp() == "assign") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
	}

	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru pierwszego
	if(field1->getTyp() != "constant" && field1->getTyp() != "any")
		setLines1 = cutSetLines(field1->getWartosc(), setLines1);
	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru drugiego
	if(field2->getTyp() != "constant" && field2->getTyp() != "any")
		setLines2 = cutSetLines(field2->getWartosc(), setLines2);

	vector<int> resultPart;
	//Sprawdzenie cz wszystkie parametry były dobre, jesli nie sa to zwracana jest pusta lista 
	if (!setLines1.empty() && !setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if (pkb->getFollows()->follows(*l1, *l2) == true) {
					if (selectValue == field1->getWartosc() && selectValue == field2->getWartosc() && selectValue != "boolean") {
						//Jeżeli oba parametry śa takie same, a nie są to constant, znaczy to że nie ma odpowiedzi
						return resultPart;
					} else if (selectValue == field1->getWartosc() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
						//Jeśli parametr pierwszy jest tym, którego szukamy to wybieramy z listy pierwszej
						if(find(resultPart.begin(),resultPart.end(),*l1) >= resultPart.end())
							resultPart.push_back(*l1);
					} else if (selectValue == field2->getWartosc() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
						//Jeśli parametr drugi jest tym, którego szukamy to wybieramy z listy drugiej
						if(find(resultPart.begin(),resultPart.end(),*l2) >= resultPart.end())
							resultPart.push_back(*l2);
					} else {
						//Jeśli żaden parametr nie jest szukany to zwracane są wszystkie wartości
						return lines;
					}
				}
			}
		}
	}

	return resultPart;
}

vector<int> QueryEvaluator::getFollowsSWynik(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<int> setLines2;

	//Sprawdzanie pierwszego parametru (Filed1) w relacji Follows
	if (field1->getTyp() == "constant" && field2->getTyp() != "constant") {
		int param = field1->stringNaInt();
		//Do listy pierwszej dodawana jest wartość constant
		setLines1.insert(param);
		//Sprawdzanie czym jest drugi parametr i ewentualne pobranie listy
		if (field2->getTyp() == "stmt" || field2->getTyp() == "any") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines2.insert(pom.begin(), pom.end());
		} else if (field2->getTyp() == "while") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field2->getTyp() == "if") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field2->getTyp() == "call") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field2->getTyp() == "assign") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
	} else if (field1->getTyp() != "constant"
			&& field2->getTyp() == "constant") {
		int param = field2->stringNaInt();
		//Do listy drugiej dodawana jest wartość constant
		setLines2.insert(param);
		//Sprawdzanie czym jest pierwszy parametr i ewentualne pobranie listy
		if (field1->getTyp() == "stmt" || field1->getTyp() == "any") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines1.insert(pom.begin(), pom.end());
		} else if (field1->getTyp() == "while") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field1->getTyp() == "if") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field1->getTyp() == "call") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field1->getTyp() == "assign") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
	} else if (field1->getTyp() == "constant" && field2->getTyp() == "constant") {
		int param1 = field1->stringNaInt();
		int param2 = field2->stringNaInt();
		if (pkb->getFollows()->follows(param1, param2) == true) {
			return lines;
		}
	} else {
		//Dla pierwszego parametru nastepuje pobranie z listy
		//Any jest to dowolna wartośc z dostępnych czyli stmt
		if (field1->getTyp() == "stmt" || field1->getTyp() == "any") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines1.insert(pom.begin(), pom.end());
		} else if (field1->getTyp() == "while") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field1->getTyp() == "if") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field1->getTyp() == "call") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field1->getTyp() == "assign") {
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
		// Pobranie listy dla drugiego parametru
		if (field2->getTyp() == "stmt" || field2->getTyp() == "any") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
			set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
			setLines2.insert(pom.begin(), pom.end());
		} else if (field2->getTyp() == "while") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		} else if (field2->getTyp() == "if") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		} else if (field2->getTyp() == "call") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		} else if (field2->getTyp() == "assign") {
			setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
	}

	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru pierwszego
	if(field1->getTyp() != "constant" && field1->getTyp() != "any")
		setLines1 = cutSetLines(field1->getWartosc(), setLines1);
	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru drugiego
	if(field2->getTyp() != "constant" && field2->getTyp() != "any")
		setLines2 = cutSetLines(field2->getWartosc(), setLines2);

	vector<int> resultPart;
	//Sprawdzenie cz wszystkie parametry były dobre, jesli nie sa to zwracana jest pusta lista
	if (!setLines1.empty() && !setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if (pkb->getFollows()->followsS(*l1, *l2) == true) {
					if (selectValue == field1->getWartosc() && selectValue == field2->getWartosc() && selectValue != "boolean") {
						//Jeżeli oba parametry śa takie same, a nie są to constant, znaczy to że nie ma odpowiedzi
						return resultPart;
					} else if (selectValue == field1->getWartosc() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
						//Jeśli parametr pierwszy jest tym, którego szukamy to wybieramy z listy pierwszej
						if(find(resultPart.begin(),resultPart.end(),*l1) >= resultPart.end())
							resultPart.push_back(*l1);
					} else if (selectValue == field2->getWartosc() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
						//Jeśli parametr drugi jest tym, którego szukamy to wybieramy z listy drugiej
						if(find(resultPart.begin(),resultPart.end(),*l2) >= resultPart.end())
							resultPart.push_back(*l2);
					} else {
						//Jeśli żaden parametr nie jest szukany to zwracane są wszystkie wartościs
						return lines;
					}
				}
			}
		}
	}

	return resultPart;
}

vector<int> QueryEvaluator::getUsesWynik(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<int> setLines2;

	if(field1->getTyp() == "constant" && field2->getTyp() != "constant")
	{
		int param1 = field1->stringNaInt();
		vector<int> a = pkb->getTablicaLiniiKodu()->getLinieCall();
		if(find(a.begin(),a.end(),param1) != a.end())
		{
			string name = pkb->getTablicaLiniiKodu()->getWywolanaNazwaProcedury(param1);
			vector<int> b = pkb->getTablicaProcedur()->getLinieProcedury(pkb->getTablicaProcedur()->getIdProcedury(name));
			setLines1.insert(b.begin(),b.end());
		}
		else if(pkb->getTablicaProcedur()->getMaxIdProcedury() >= param1)
		{
			string name = pkb->getTablicaProcedur()->getNazwaProcedury(param1);
			vector<int> b = pkb->getTablicaProcedur()->getLinieProcedury(pkb->getTablicaProcedur()->getIdProcedury(name));
			setLines1.insert(b.begin(),b.end());
		}
		else
			setLines1.insert(param1);

		if(field2->getTyp() == "variable" || field2->getTyp() == "any")
		{
			vector<string> tmp = pkb->getTablicaZmiennych()->getAllVar();
			for(size_t i = 0 ; i < tmp.size() ; i++)
			{
				setLines2.insert(pkb->getTablicaZmiennych()->getZmiennaId(tmp[i]));
			}
		}
		else if(field2->getTyp() == "string")
		{
			int param2 = pkb->getTablicaZmiennych()->getZmiennaId(field2->getWartosc());
			if(param2 != -1)
				setLines2.insert(param2);
		}
	}
	else if(field1->getTyp() != "constant" && field2->getTyp() != "constant")
	{
		//Sprawdzenie atrybutu z pola pierwszego
		if(field1->getTyp() == "if")
		{
			map<int,vector<int>> ifLinesStart = pkb->getTablicaLiniiKodu()->getLinieZawartosciIf();

			for (map<int, vector<int>>::iterator it = ifLinesStart.begin(); it != ifLinesStart.end(); ++it) {
				vector<int> ifLines = (*it).second;
				setLines1.insert((*it).first);
				for(size_t i = 0 ; i < ifLines.size() ; i++)
				{
					setLines1.insert(ifLines[i]);
				}
			}
		}
		else if(field1->getTyp() == "while")
		{
			map<int,vector<int>> whileLinesStart = pkb->getTablicaLiniiKodu()->getLinieZawartosciWhile();

			for (map<int, vector<int>>::iterator it = whileLinesStart.begin(); it != whileLinesStart.end(); ++it) {
				vector<int> whileLines = (*it).second;
				setLines1.insert((*it).first);
				for(size_t i = 0 ; i < whileLines.size() ; i++)
				{
					setLines1.insert(whileLines[i]);
				}
			}
		}
		else if(field1->getTyp() == "assign")
		{
			setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		}
		else if(field1->getTyp() == "procedure")
		{
			map<int,vector<int>> procLinesStart = pkb->getTablicaProcedur()->getProceduresBodyLines();

			for (map<int, vector<int>>::iterator it = procLinesStart.begin(); it != procLinesStart.end(); ++it)
			{
				vector<int> procLines = (*it).second;
				int procParam = pkb->getTablicaProcedur()->getPierwszaLiniaProcedury((*it).first);
				setLines1.insert(procParam);
				for(size_t i = 0 ; i < procLines.size() ; i++)
				{
					setLines1.insert(procLines[i]);
				}
			}
		}
		else if(field1->getTyp() == "call")
		{
			vector<int> callLinesStart = pkb->getTablicaLiniiKodu()->getLinieCall();
			for(size_t i = 0 ; i < callLinesStart.size() ; i++)
			{
				string nazwaProcedury = pkb->getTablicaLiniiKodu()->getWywolanaNazwaProcedury(callLinesStart[i]);
				int idProcedury = pkb->getTablicaProcedur()->getIdProcedury(nazwaProcedury);
				if(idProcedury != -1)
				{
					vector<int> callLines = pkb->getTablicaProcedur()->getLinieProcedury(idProcedury);

					for(size_t j = 0 ; j < callLines.size() ; j++)
					{
						setLines1.insert(callLines[j]);
					}
				}
			}
		}
		else if(field1->getTyp() == "string")
		{
			int idProcedury = pkb->getTablicaProcedur()->getIdProcedury(field1->getWartosc());
			if(idProcedury != -1)
			{
				vector<int> procLines = pkb->getTablicaProcedur()->getLinieProcedury(idProcedury);

				for(size_t j = 0 ; j < procLines.size() ; j++)
				{
					setLines1.insert(procLines[j]);
				}
			}
		}
		else if(field1->getTyp() == "stmt")
		{
			vector<int> allLines = pkb->getTablicaLiniiKodu()->getLines();
			for(size_t j = 0 ; j < allLines.size() ; j++)
			{
				setLines1.insert(allLines[j]);
			}
		}

		//Sprawdzenie atrybutu z pola drugiego
		if(field2->getTyp() == "variable" || field2->getTyp() == "any")
		{
			vector<string> tmp = pkb->getTablicaZmiennych()->getAllVar();
			for(size_t i = 0 ; i < tmp.size() ; i++)
			{
				setLines2.insert(pkb->getTablicaZmiennych()->getZmiennaId(tmp[i]));
			}
		}
		else if(field2->getTyp() == "string")
		{
			int param2 = pkb->getTablicaZmiennych()->getZmiennaId(field2->getWartosc());
			if(param2 != -1)
				setLines2.insert(param2);
		}
	}

	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru pierwszego
	if(field1->getTyp() != "constant" && field1->getTyp() != "any")
		setLines1 = cutSetLines(field1->getWartosc(), setLines1);
	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru drugiego
	if(field2->getTyp() != "constant" && field2->getTyp() != "any")
		setLines2 = cutSetLines(field2->getWartosc(), setLines2);

	vector<int> resultPart;
	set<std::pair<string,int>> tmpUsesPairs;
	//Sprawdzenie cz wszystkie parametry były dobre, jesli nie sa to zwracana jest pusta lista 
	if (!setLines1.empty() && !setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if (pkb->getUses()->uses(*l1, pkb->getTablicaZmiennych()->getNazwaZmiennej(*l2)) == true) {
					if(pierwszyUses == true)
					{
						usesPairs.insert(std::pair<string,int>(std::make_pair(pkb->getTablicaZmiennych()->getNazwaZmiennej((*l2)),*l1)));
					}
					else
					{
						tmpUsesPairs.insert(std::pair<string,int>(std::make_pair(pkb->getTablicaZmiennych()->getNazwaZmiennej((*l2)),*l1)));
					}

					if (selectValue == field1->getWartosc() && selectValue == field2->getWartosc() && selectValue != "boolean") {
						//Jeżeli oba parametry śa takie same, a nie są to constant, znaczy to że nie ma odpowiedzi
						pierwszyUses = false;
						usesPairs.clear();
						return resultPart;
					} else if (selectValue == field1->getWartosc() && selectValue != "boolean") {
						//Jeśli parametr pierwszy jest tym, którego szukamy to wybieramy z listy pierwszej
						if(find(resultPart.begin(),resultPart.end(),*l1) >= resultPart.end())
						{
							resultPart.push_back(*l1);
						}
					} else if (selectValue == field2->getWartosc() && selectValue != "boolean") {
						//Jeśli parametr drugi jest tym, którego szukamy to wybieramy z listy drugiej
						if(find(resultPart.begin(),resultPart.end(),*l1) >= resultPart.end())
						{
							resultPart.push_back(*l1);
						}
					} else {
						//Jeśli żaden parametr nie jest szukany to zwracane są wszystkie wartościs
						pierwszyUses = false;
						return lines;
					}
				}
			}
		}

		if(pierwszyUses != true)
		{
			set<string> usesPairsName;
			set<string> actPairsName;
			set<int> usesPairsLines;
			set<int> actPairsLines;

			cout << endl;
			for (set<std::pair<string,int>>::iterator it = tmpUsesPairs.begin(); it != tmpUsesPairs.end(); ++it) {
				actPairsName.insert((*it).first);
				actPairsLines.insert((*it).second);

				cout << "TMP USES ---> " << (*it).first << " " << (*it).second << endl;
			}
			cout << endl;
			for (set<string>::iterator it = actPairsName.begin(); it != actPairsName.end(); ++it) {
				cout << "ACT NAME ---> " << (*it) << endl;
			}
			cout << endl;
			for (set<int>::iterator it = actPairsLines.begin(); it != actPairsLines.end(); ++it) {
				cout << "ACT LINE ---> " << (*it) << endl;
			}
			cout << endl;

			for (set<std::pair<string,int>>::iterator it = usesPairs.begin(); it != usesPairs.end(); ++it) {

				if(find(actPairsName.begin(),actPairsName.end(),(*it).first) == actPairsName.end())
				{
					cout << "Usuwam: " << (*it).first << " " << (*it).second << endl;
					usesPairs.erase(*it);
				}
				if(find(actPairsLines.begin(),actPairsLines.end(),(*it).second) == actPairsLines.end())
				{
					cout << "Usuwam: " << (*it).first << " " << (*it).second << endl;
					usesPairs.erase(*it);
				}
			}

			for (set<std::pair<string,int>>::iterator it = usesPairs.begin(); it != usesPairs.end(); ++it) {
				cout << "PAIR ---> " << (*it).first << " " << (*it).second << endl;
			}
		}

	}

	pierwszyUses = false;
	return resultPart;
}

vector<int> QueryEvaluator::getCallWynik(Field* field1, Field* field2, vector<int> lines, string selectValue) {
		set<int> candidatesForParameter1;
		set<int> candidatesForParameter2;
		set<int> resultPart;
		vector<int> returnIt;
		if (field1 == nullptr || field2 == nullptr) {
			return returnIt;
		}
		string firstParameterType = field1->getTyp();
		string secondParameterType = field2->getTyp();
		int firstProcedureId = -1;
		int secondProcedureId = -1;

		//Dla parametru pierwszego tworzy się lista z możliwymi wartościami dla tego parametru
		if (firstParameterType == "string") {
			firstProcedureId = pkbApi->getIdProcedury(field1->getWartosc());
			if (firstProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter1.insert(firstProcedureId);
		}
		else if (firstParameterType == "constant") {
			firstProcedureId = pkbApi->getIdProcedury(field1->stringNaInt());
			if (firstProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter1.insert(firstProcedureId);
		}
		else {
			for (int i = 0; i <= pkbApi->getMaxIdProcedury(); i++) {
				candidatesForParameter1.insert(i);
			}
		}

		//Dla parametru drugiego tworzy się lista z możliwymi wartościami dla tego parametru
		if (secondParameterType == "string") {
			secondProcedureId = pkbApi->getIdProcedury(field2->getWartosc());
			if (secondProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter2.insert(secondProcedureId);
		}
		else if (secondParameterType == "constant") {
			secondProcedureId = pkbApi->getIdProcedury(field2->stringNaInt());
			if (secondProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter2.insert(secondProcedureId);
		}
		else {
			for (int i = 0; i <= pkbApi->getMaxIdProcedury(); i++) {
				candidatesForParameter2.insert(i);
			}
		}

		//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru pierwszego
		if (field1->getTyp() != "constant" && field1->getTyp() != "any") {
			candidatesForParameter1 = cutSetLines(field1->getWartosc(), candidatesForParameter1);
		}
		//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru drugiego
		if (field2->getTyp() != "constant" && field2->getTyp() != "any") {
			candidatesForParameter2 = cutSetLines(field2->getWartosc(), candidatesForParameter2);
		}

		if (firstParameterType == "constant" && secondParameterType == "constant") {
			/*if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
				resultPart.insert(pkbApi->getPierwszaLiniaProcedury(firstProcedureId));
			}
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;*/
		}
		else if (firstParameterType == "string"  && secondParameterType == "string") {
			/*
			if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
				resultPart.insert(pkbApi->getPierwszaLiniaProcedury(firstProcedureId));
			}
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;*/
		}
		else if (firstParameterType == "any" && secondParameterType == "any") {
			//Jesli "any" spełnia wymagania to wszystkie linie są dobre
			/*if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
				for (int i = 0; i <= pkbApi->getMaxIdProcedury(); i++) {
					resultPart.insert(pkbApi->getPierwszaLiniaProcedury(i));
				}
			}*/
			//Jeśli "any" nie spełnia wymagań to wszystkie linie są złe (nie ma procedur w programie)
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else if (selectValue == field1->getWartosc() && selectValue == field2->getWartosc() && selectValue != "boolean") {
			//Jeśli w zapytaniu sa dwie takie same wartości, a nie jest to "any" to zwraca pusty wynik, ponieważ nie można wywołać rekurencyjnie
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else {
			for (set<int>::iterator parameter1 = candidatesForParameter1.begin(); parameter1 != candidatesForParameter1.end(); ++parameter1) {
				for (set<int>::iterator parameter2 = candidatesForParameter2.begin(); parameter2 != candidatesForParameter2.end(); ++parameter2) {
					/*if (pkbApi->calls(*parameter1, *parameter2) && *parameter1 != *parameter2)
					{
						if (selectValue == field1->getWartosc() && selectValue != "boolean") {
							//Dodaje możliwość z parameter1 do wyniku, gdy call(parmeter1,*) gdzie * = 'any','const','var'
							if(find(resultPart.begin(),resultPart.end(),pkbApi->getPierwszaLiniaProcedury(*parameter1)) == resultPart.end())
								resultPart.insert(pkbApi->getPierwszaLiniaProcedury(*parameter1));
						}
						else if (selectValue == field2->getWartosc() && selectValue != "boolean") {
							//Dodaje możliwość z parameter2 do wyniku, gdy call(parmeter2,*) gdzie * = 'any','const','var'
							if(find(resultPart.begin(),resultPart.end(),pkbApi->getPierwszaLiniaProcedury(*parameter2)) == resultPart.end())
								resultPart.insert(pkbApi->getPierwszaLiniaProcedury(*parameter2));
						}
						else {
							//Zwraca wszystko, czy call(1,_) lub call(_,1)
							if (field1->getTyp() != "constant" && field1->getTyp() != "string") {
								resultPart.insert(pkbApi->getPierwszaLiniaProcedury(*parameter1));
							}
							if (field2->getTyp() != "constant" && field2->getTyp() != "string") {
								resultPart.insert(pkbApi->getPierwszaLiniaProcedury(*parameter2));
							}
						}
					}*/
				}
			}
		}
		copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
		return returnIt;
	}

vector<int> QueryEvaluator::getCallSWynik(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	set<int> candidatesForParameter1;
	set<int> candidatesForParameter2;
	set<int> resultPart;
	vector<int> returnIt;
	if (field1 == nullptr || field2 == nullptr) {
		return returnIt;
	}
	else {
		//cout << "Both fields are != nullptr" << endl;
	}
	string firstParameterType = field1->getTyp();
	string secondParameterType = field2->getTyp();
	int firstProcedureId = -1;
	int secondProcedureId = -1;

	//Dla parametru pierwszego tworzona jest lista z możliwymi wartościami dla tego parametru
	if (firstParameterType == "string") {
		firstProcedureId = pkbApi->getIdProcedury(field1->getWartosc());
		if (firstProcedureId == -1) {
			return returnIt;
		}
		candidatesForParameter1.insert(firstProcedureId);
	}
	else if (firstParameterType == "constant") {
		firstProcedureId = pkbApi->getIdProcedury(field1->stringNaInt());
		if (firstProcedureId == -1) {
			return returnIt;
		}
		candidatesForParameter1.insert(firstProcedureId);
	}
	else {
		for (int i = 0; i < pkbApi->getMaxIdProcedury(); i++) {
			candidatesForParameter1.insert(i);
		}
	}

	//Dla parametru drugiego tworzona jest lista z możliwymi wartościami dla tego parametru
	if (secondParameterType == "string") {
		secondProcedureId = pkbApi->getIdProcedury(field2->getWartosc());
		if (secondProcedureId == -1) {
			return returnIt;
		}
		candidatesForParameter2.insert(secondProcedureId);
	}
	else if (secondParameterType == "constant") {
		secondProcedureId = pkbApi->getIdProcedury(field2->stringNaInt());
		if (secondProcedureId == -1) {
			return returnIt;
		}
		candidatesForParameter2.insert(secondProcedureId);
	}
	else {
		for (int i = 0; i < pkbApi->getMaxIdProcedury(); i++) {
			candidatesForParameter2.insert(i);
		}
	}
	
	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru pierwszego
	if (field1->getTyp() != "constant" && field1->getTyp() != "any") {
		candidatesForParameter1 = cutSetLines(field1->getWartosc(), candidatesForParameter1);
	}
	//Biorąc pod uwage części zapytania z "with", skracana jest lista parametru drugiego
	if (field2->getTyp() != "constant" && field2->getTyp() != "any") {
		candidatesForParameter2 = cutSetLines(field2->getWartosc(), candidatesForParameter2);
	}

	if (firstParameterType == "constant" && secondParameterType == "constant") {
		/*if (pkbApi->callsS(firstProcedureId, secondProcedureId)) {
			resultPart.insert(pkbApi->getPierwszaLiniaProcedury(firstProcedureId));
		}
		copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
		return returnIt;*/
	}
	else if (firstParameterType == "string"  && secondParameterType == "string") {
		/*if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
			resultPart.insert(pkbApi->getPierwszaLiniaProcedury(firstProcedureId));
		}
		copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
		return returnIt;*/
	}
	else if (firstParameterType == "any" && secondParameterType == "any") {
		/*if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
			for (int i = 0; i <= pkbApi->getMaxIdProcedury(); i++) {
				resultPart.insert(pkbApi->getPierwszaLiniaProcedury(i));
			}
		}*/
		copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
		return returnIt;
	}
	else if (selectValue == field1->getWartosc() && selectValue == field2->getWartosc() && selectValue != "boolean") {
		copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
		return returnIt;
	}
	else {
		for (set<int>::iterator parameter1 = candidatesForParameter1.begin(); parameter1 != candidatesForParameter1.end(); ++parameter1) {
			for (set<int>::iterator parameter2 = candidatesForParameter2.begin(); parameter2 != candidatesForParameter2.end(); ++parameter2) {
				/*if (pkbApi->callsS(*parameter1, *parameter2) && *parameter1 != *parameter2)
				{
					if (selectValue == field1->getWartosc() && selectValue != "boolean") {
						//Dodaje możliwość z parameter1 do wyniku, gdy call(parmeter1,*) gdzie * = 'any','const','var'
						if(find(resultPart.begin(),resultPart.end(),pkbApi->getPierwszaLiniaProcedury(*parameter1)) == resultPart.end())
							resultPart.insert(pkbApi->getPierwszaLiniaProcedury(*parameter1));
					}
					else if (selectValue == field2->getWartosc() && selectValue != "boolean") {
						//Dodaje możliwość z parameter2 do wyniku, gdy call(parmeter2,*) gdzie * = 'any','const','var'
						if(find(resultPart.begin(),resultPart.end(),pkbApi->getPierwszaLiniaProcedury(*parameter2)) == resultPart.end())
							resultPart.insert(pkbApi->getPierwszaLiniaProcedury(*parameter2));
					}
					else {
						//Zwraca wszystko, czy call(1,_) lub call(_,1)
						if (field1->getTyp() != "constant" && field1->getTyp() != "string") {
							resultPart.insert(pkbApi->getPierwszaLiniaProcedury(*parameter1));
						}
						if (field2->getTyp() != "constant" && field2->getTyp() != "string") {
							resultPart.insert(pkbApi->getPierwszaLiniaProcedury(*parameter2));
						}
					}
				}*/
			}
		}
	}
	copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
	return returnIt;
}

void QueryEvaluator::getWithWynik(Field* field1, Field* field2, vector<int> lines) {

	set<int> setLines1;
	set<int> setLines2;
	string value1;
	string value2;

	if(field1->getTyp() == "stmt") {
		setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		setLines1.insert(pom.begin(), pom.end());
		pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		setLines1.insert(pom.begin(), pom.end());
		pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		setLines1.insert(pom.begin(), pom.end());
	}
	else if(field1->getTyp() == "while") {
		setLines1 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
	}
	else if(field1->getTyp() == "if") {
		setLines1 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
	}
	else if(field1->getTyp() == "call") {
		setLines1 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
	}
	else if(field1->getTyp() == "assign") {
		setLines1 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
	}
	else if(field1->getTyp() == "prog_line") {
		vector<int> tmp1;
		tmp1 = pkb->getTablicaLiniiKodu()->getLines();
		setLines1.insert(tmp1.begin(),tmp1.end());
	}
	else if(field1->getTyp() == "constant") {
		MatcherPQL* m = new MatcherPQL();
		if(m->czyLiczba(field1->getWartosc()))
		{
			int param = field1->stringNaInt();
			setLines1.insert(param);
		}
		else
		{
			vector<int> tmp1;
			tmp1 = pkb->getTablicaLiniiKodu()->getLines();
			setLines1.insert(tmp1.begin(),tmp1.end());
		}
		delete m;
	}
	else if(field1->getTyp() == "variable") {
		value1 = field1->getWartosc();
	} else if(field1->getTyp() == "procedure") {
		vector<int> tmp1;
		tmp1 = pkb->getTablicaProcedur()->getProceduresLines();
		setLines1.insert(tmp1.begin(),tmp1.end());
	} else if(field1->getTyp() == "string") {
		value1 = field1->getWartosc();
	}

	if(field2->getTyp() == "stmt") {
		setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
		set<int> pom = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
		setLines2.insert(pom.begin(), pom.end());
		pom = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
		setLines2.insert(pom.begin(), pom.end());
		pom = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
		setLines2.insert(pom.begin(), pom.end());
	}
	else if(field2->getTyp() == "while") {
		setLines2 = pkb->getTablicaLiniiKodu()->getOrderedWhileLines();
	}
	else if(field2->getTyp() == "if") {
		setLines2 = pkb->getTablicaLiniiKodu()->getOrderedIfLines();
	}
	else if(field2->getTyp() == "call") {
		setLines2 = pkb->getTablicaLiniiKodu()->getOrderedCallLines();
	}
	else if(field2->getTyp() == "assign") {
		setLines2 = pkb->getTablicaLiniiKodu()->getOrderedAssignLines();
	}
	else if(field2->getTyp() == "prog_line") {
		vector<int> tmp2;
		tmp2 = pkb->getTablicaLiniiKodu()->getLines();
		setLines2.insert(tmp2.begin(),tmp2.end());
	}
	else if(field2->getTyp() == "constant") {
		MatcherPQL* m = new MatcherPQL();
		if(m->czyLiczba(field2->getWartosc()))
		{
			int param = field2->stringNaInt();
			setLines2.insert(param);
		}
		else
		{
			vector<int> tmp2;
			tmp2 = pkb->getTablicaLiniiKodu()->getLines();
			setLines2.insert(tmp2.begin(),tmp2.end());
		}
		delete m;
	}
	else if(field2->getTyp() == "variable") {
		value2 = field2->getWartosc();
	} else if(field2->getTyp() == "procedure") {
		vector<int> tmp2;
		tmp2 = pkb->getTablicaProcedur()->getProceduresLines();
		setLines2.insert(tmp2.begin(),tmp2.end());
	} else if(field2->getTyp() == "string") {
		value2 = field2->getWartosc();
	}

	vector<int> withLines;
	if(field1->getTyp() != "procedure" && field1->getTyp() != "variable" && field1->getTyp() != "string" &&
	   field2->getTyp() != "procedure" && field2->getTyp() != "variable" && field2->getTyp() != "string")
	{
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if(*l1 == *l2 && find(lines.begin(), lines.end(), *l1) != lines.end())
					withLines.push_back(*l1);
			}
		}
		withMap.insert(std::pair<string,vector<int>>(field1->getWartosc(),withLines));
		withMap.insert(std::pair<string,vector<int>>(field2->getWartosc(),withLines));
	}
	else if(field1->getTyp() == "procedure" && field2->getTyp() == "string")
	{
		int line = pkb->getTablicaProcedur()->getPierwszaLiniaProcedury(pkb->getTablicaProcedur()->getIdProcedury(value2));
		if(find(lines.begin(), lines.end(), line) != lines.end())
		{
			withLines.push_back(line);
		}
		withMap.insert(std::pair<string,vector<int>>(field1->getWartosc(),withLines));
	}
	else if(field2->getTyp() == "procedure" && field1->getTyp() == "string")
	{
		int line = pkb->getTablicaProcedur()->getPierwszaLiniaProcedury(pkb->getTablicaProcedur()->getIdProcedury(value1));
		if(find(lines.begin(), lines.end(), line) != lines.end())
		{
			withLines.push_back(line);
		}
		withMap.insert(std::pair<string,vector<int>>(field2->getWartosc(),withLines));
	}
	else if(field1->getTyp() == "variable" && field2->getTyp() == "string")
	{
		vector<int> l = pkb->getTablicaLiniiKodu()->getLines();
		for(size_t i = 0; i < l.size() ; i++)
		{
			if(pkb->getUses()->uses(l[i],value2) && find(lines.begin(), lines.end(), l[i]) != lines.end()) {
				int param2 = pkb->getTablicaZmiennych()->getZmiennaId(value2);
				if(param2 != -1)
					withLines.push_back(param2);
			}
		}
		withMap.insert(std::pair<string,vector<int>>(field1->getWartosc(),withLines));
	}
	else if(field2->getTyp() == "variable" && field1->getTyp() == "string")
	{
		vector<int> l = pkb->getTablicaLiniiKodu()->getLines();
		for(size_t i = 0; i < l.size() ; i++)
		{
			if(pkb->getUses()->uses(l[i],value1) && find(lines.begin(), lines.end(), l[i]) != lines.end()) {
				int param2 = pkb->getTablicaZmiennych()->getZmiennaId(value1);
				if(param2 != -1)
					withLines.push_back(param2);
			}
		}
		withMap.insert(std::pair<string,vector<int>>(field2->getWartosc(),withLines));
	}
	else if(field1->getTyp() == "string" && field2->getTyp() == "string" && value1 == value2)
	{
		vector<int> l = pkb->getTablicaLiniiKodu()->getLines();
		for(size_t i = 0; i < l.size() ; i++)
		{
			if(find(lines.begin(), lines.end(), l[i]) != lines.end()) {
				withLines.push_back(l[i]);
			}
		}
		withMap.insert(std::pair<string,vector<int>>("all",withLines));
	}
	else if(field1->getTyp() == "string" && field2->getTyp() == "string" && value1 != value2)
	{
		vector<int> empty;
		withMap.insert(std::pair<string,vector<int>>("all",empty));
	}
}

set<int> QueryEvaluator::cutSetLines(string fieldValue, set<int> setLines)
{
	vector<int> fieldMap;
	if(withMap.count(fieldValue) > 0) fieldMap = withMap[fieldValue];
	set<int> setLines1tmp;
	vector<int> allMap;
	if(withMap.count("all") > 0) allMap = withMap["all"];

	if(!withMap.empty())
	{
		if(!allMap.empty())
		{
			for (set<int>::iterator l1 = setLines.begin(); l1 != setLines.end(); ++l1) {
				if(find(allMap.begin(), allMap.end(), *l1) != allMap.end())
				{
					setLines1tmp.insert(*l1);
				}
			}
			setLines.clear();
			setLines.insert(setLines1tmp.begin(), setLines1tmp.end());
		}

		if(allMap.empty() && withMap.count("all") > 0)
		{
			setLines.clear();
		}

		if(!fieldMap.empty())
		{
			for (set<int>::iterator l1 = setLines.begin(); l1 != setLines.end(); ++l1) {
				if(find(fieldMap.begin(), fieldMap.end(), *l1) != fieldMap.end())
				{
					setLines1tmp.insert(*l1);
				}
			}
			setLines.clear();
			setLines.insert(setLines1tmp.begin(), setLines1tmp.end());
		}
	}
	return setLines;
}

void QueryEvaluator::cutUsesPairs(vector<int> lines)
{
	vector<string> usesName;
	for (set<std::pair<string,int>>::iterator i = usesPairs.begin(); i != usesPairs.end(); ++i) {
		if(find(usesName.begin(),usesName.end(),(*i).first) == usesName.end())
			usesName.push_back((*i).first);
	}

	for(size_t i = 0; i < usesName.size() ; i++)
	{
		for(size_t j = 0; j < lines.size() ; j++)
		{
			std::pair<string,int> tmp = {usesName[i],lines[j]};
			if(find(usesPairs.begin(),usesPairs.end(),tmp) == usesPairs.end())
			{
				for(set<std::pair<string,int>>::iterator it = usesPairs.begin(); it != usesPairs.end(); ++it)
				{
					if((*it).first == tmp.first && (*it).second == tmp.second) usesPairs.erase((*it));
				}
			}
		}
	}
}
}
