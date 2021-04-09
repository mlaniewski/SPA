#ifndef SRC_QUERYPREPROCESSOR_H_
#define SRC_QUERYPREPROCESSOR_H_

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <iterator>

#include "../PQL/Exceptions.h"
#include "../PQL/MatcherPQL.h"
#include "../PQL/tree_util.hh"
#include "../PQL/PQLNode.h"
#include "../PQL/PQLTree.h"

#include "../PQL/Field.h"
#include "../PQL/PQLNode.h"

using namespace std;

class QueryPreProcessor {
public:
	QueryPreProcessor();
	virtual ~QueryPreProcessor();
	void parseQuery(string query);
	MatcherPQL* matcher;
	Exceptions* exc;

private:

	//wywouje sie w split, patrz nizej
	vector<string> &split(const string &s, char delim, vector<string> &elems) {
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	//rozdziela zapytania dla konkretnego ogranicznika(delim), np. ";" (srednik)
	vector<string> split(const string &s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}

	//rozdziela zapytania dla konkretnego tokena, np. "and"
	vector<string> split(const string &s, string token) {
		vector<string> elems;
		vector<size_t> positions;
		size_t pos = s.find(token, 0);
		while (pos != string::npos) {
			//elems.push_back(s.substr(0, pos));
			positions.push_back(pos);
			pos = s.find(token, pos + 5);
		}

		int p1 = 0;
		int p2 = s.length();

		if (positions.size() > 0)
		{
			for (size_t i = 0; i < positions.size(); i++) {
				pos = positions[i];
				elems.push_back(s.substr(p1,pos-p1));
				p1 = pos + 5;
			}
			elems.push_back(s.substr(p1,p2-p1));
		}
		else
			elems.push_back(s.substr(p1,p2));

        return elems;
	}

	vector<Field> makeFields(vector<string> &elems) {
		vector<Field> fields;
		vector<string> tmp = elems;
		vector<string> tokens = matcher->getTokensList();

		for (size_t i = 0; i < tmp.size(); i++) {

			if (!matcher->checkAll(tmp[i])) {
				for(size_t j = 0 ; j < tokens.size() ; j ++)
				{
					if (tmp[i].find(tokens[j]) < tmp[i].length()) {
						vector<Field> tmpFields = makeFieldType(tokens[j],tmp[i]);
						for (size_t j = 0; j < tmpFields.size(); j++) {
							fields.push_back(tmpFields[j]);
						}
					}
				}
			} else {
				exc->throwException();
				return vector<Field>();
			}
		}

		return fields;
	}

	vector<Field> makeFieldType(string type, string declaration) {
		declaration.erase(remove_if(declaration.begin(), declaration.end(), ptr_fun<int, int>(isspace)), declaration.end());
		declaration = declaration.substr(type.length(), declaration.length());
		vector<string> tmp = split(declaration, ',');
		vector<Field> declarationFields;
		for (size_t i = 0; i < tmp.size(); i++) {
			declarationFields.push_back(Field(type, tmp[i]));
		}

		return declarationFields;
	}

	PQLTree* PqlTree;

	void makeTree(vector<string> &elems)
	{
		if(elems.size() > 1) exc->throwException();

		vector<string> queryMainTokens;
		for(size_t i = 0 ; i < elems.size() ; i ++)
		{
			if(matcher->checkTokens(elems[i],"Select")) {
			}
			else {
				exc->throwException();
			}

			if(matcher->checkTokens(elems[i],"such that")) {
				queryMainTokens.push_back("such that");
			}

			if(matcher->checkTokens(elems[i],"with")) {
				queryMainTokens.push_back("with");
			}
		}


		vector<string> elements = splitQuery(elems[0],queryMainTokens);


		for(size_t i = 0 ; i < queryParts.size() ; i ++)
		{
			switch(checkType(queryParts[i]))
			{
				case 0:
					exc->throwSplitException();
					break;
				case 1:
					makeSelectNode(queryParts[i]);
					break;
				case 2:
					makeSuchNode(queryParts[i]);
					break;
				case 3:
					makeWithNode(queryParts[i]);
					break;
			}
		}

		tree<tree_node_<PQLNode*>*>::iterator iter;
		PQLNode* node;
		tree_node_<PQLNode*>* treeNode;

		PqlTree = new PQLTree();

		node = new PQLNode("queryNode");
		treeNode = new tree_node_<PQLNode*>(node);
		iter = PqlTree->appendRoot(treeNode);

		if(selectNodes.size() > 0)
		{
			node = new PQLNode("resultMainNode");
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			node = selectNodes[0];
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			for(size_t i = 1 ; i < selectNodes.size() ; i ++)
			{
				node = selectNodes[i];
				treeNode = new tree_node_<PQLNode*>(node);
				iter = PqlTree->appendSibling(iter, treeNode);
			}
		}

		if(withNodes.size() > 0)
		{
			iter = PqlTree->getKorzenDrzewa();

			node = new PQLNode("withMainNode");
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			node = withNodes[0];
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			for(size_t i = 1 ; i < withNodes.size() ; i ++)
			{
				node = withNodes[i];
				treeNode = new tree_node_<PQLNode*>(node);
				iter = PqlTree->appendSibling(iter, treeNode);
			}
		}

		if(suchNodes.size() > 0)
		{
			iter = PqlTree->getKorzenDrzewa();

			node = new PQLNode("suchMainNode");
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			node = suchNodes[0];
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			for(size_t i = 1 ; i < suchNodes.size() ; i ++)
			{
				node = suchNodes[i];
				treeNode = new tree_node_<PQLNode*>(node);
				iter = PqlTree->appendSibling(iter, treeNode);
			}
		}

	}

	vector<PQLNode*> selectNodes;
	vector<PQLNode*> suchNodes;
	vector<PQLNode*> withNodes;

	void makeSelectNode(string selectPart)
	{
		string type = "Select";
		int startPos = selectPart.find(type);
		selectPart = selectPart.substr(startPos+type.length());
		selectPart.erase(remove_if(selectPart.begin(), selectPart.end(), ptr_fun<int, int>(isspace)), selectPart.end());
		vector<string> selectParts = split(selectPart,',');


		Field* aktField;
		int dotPos;
		for(size_t i = 0 ; i < selectParts.size() ; i ++)
		{
			if(matcher->checkAll(selectParts[i]))
			{
				if(!matcher->checkBOOLEAN(selectParts[i]))
				{
					dotPos = selectParts[i].find(".");
					aktField = findField(selectParts[i].substr(0,dotPos));
					if(aktField != nullptr)
					{
						if(matcher->checkNazwaProcedury(selectParts[i])) {
							aktField->setNazwaProcedury(true);
						}

						if (matcher->checkVarName(selectParts[i])) {
							aktField->setNazwaZmiennej(true);
						}

						if (matcher->checkStmt_(selectParts[i])) {
							aktField->setStatement(true);
						}

						if (matcher->checkValue(selectParts[i])) {
							aktField->setWartosc1(true);
						}
						selectNodes.push_back(new PQLNode("resultNode", aktField));
					}
					else
					{
						exc->throwUnexpectedSelectPart();
					}
				}
				else if(matcher->checkBOOLEAN(selectParts[i]))
				{
					aktField = new Field("boolean","boolean");
					selectNodes.push_back(new PQLNode("resultNode", aktField));
				}
				else
				{
					exc->throwUnexpectedSelectPart();
				}
			}
			else
			{
				aktField = findField(selectParts[i]);
				if(aktField != nullptr)
				{
					selectNodes.push_back(new PQLNode("resultNode", aktField));
				}
				else
				{
					exc->throwUnexpectedSelectPart();
				}
			}
		}

	}

	void makeSuchNode(string suchPart)
	{
		string type = "such that";
		int startPos = suchPart.find(type);
		suchPart = suchPart.substr(startPos+type.length());
		vector<string> suchParts = split(suchPart," and ");
		string suchType;
		vector<Field*> attr;
		bool star;

		for(size_t i = 0 ; i < suchParts.size() ; i ++)
		{
			suchParts[i].erase(remove_if(suchParts[i].begin(), suchParts[i].end(), ptr_fun<int, int>(isspace)), suchParts[i].end());

			suchType = matcher -> checkSuchThatType(suchParts[i]);
			if(suchType == "") exc->throwUnexpectedTypeOfClause();
			star = matcher->isStar(suchParts[i],suchType.length());

			attr = makeSuchNodeAttributes(suchType, star, suchParts[i]);

			if(attr.size() == 2)
			{
				suchNodes.push_back(new PQLNode("suchNode", suchType, attr[0], attr[1], star));
			}
			else
			{
				exc->throwInvalidNumberOfArguments();
			}

		}
	}

	vector<Field*> makeSuchNodeAttributes(string suchtype, bool star, string suchPart)
	{
		vector<Field*> attr;
		vector<string> attrParts;
		int pos = suchtype.length() + star;
		suchPart = suchPart.substr(pos);
		if(matcher->hasTwoElem(suchPart))
		{
			if(matcher->isBracket(suchPart))
			{
				suchPart.erase(matcher->getPosition(suchPart,"("),1);
				suchPart.erase(matcher->getPosition(suchPart,")"),1);
			}

			attrParts = split(suchPart,',');

			for(size_t i = 0; i < attrParts.size() ; i ++)
			{
				if(matcher->isString(attrParts[i]))
				{
					attrParts[i].erase(matcher->getPosition(attrParts[i],"\""),1);
					attrParts[i].erase(matcher->getPosition(attrParts[i],"\""),1);
					attr.push_back(new Field("string",attrParts[i]));
				}
				else
				{
					Field* field = findField(attrParts[i]);
					if(field == nullptr)
					{
						if(matcher->is_(attrParts[i]))
							attr.push_back(new Field("any","_"));
						else if(matcher->czyLiczba(attrParts[i]))
							attr.push_back(new Field("constant",attrParts[i]));
						else
							exc->throwInvalidTypeOfArguments();
					}
					else
					{
						attr.push_back(new Field(field->getTyp(),field->getWartosc(),field->czyNazwaProcedury(),field->czyNazwaZmiennej(),field->getWartosc1(),field->czyStatement()));
					}
				}
			}
		}
		else
		{
			exc->throwToMuchArguments();
		}

		return attr;
	}

	void makeWithNode(string withPart)
	{
		string type = "with";
		int startPos = withPart.find(type);
		withPart = withPart.substr(startPos+type.length());
		vector<string> withParts = split(withPart,"and");


		vector<Field*> attr;

		for(size_t i = 0 ; i < withParts.size() ; i++)
		{
			withParts[i].erase(remove_if(withParts[i].begin(), withParts[i].end(), ptr_fun<int, int>(isspace)), withParts[i].end());

			attr = makeWithNodeAttributes(withParts[i]);

			if(attr.size() == 2)
			{
				if(matcher->checkWithAttributes(attr[0],attr[1]))
					withNodes.push_back(new PQLNode("withNode", attr[0], attr[1]));
				else
					exc-> throwInvalidWithStatementAttributes();
			}
			else
			{
				exc->throwInvalidNumberOfArguments();
			}
		}
	}

	vector<Field*> makeWithNodeAttributes(string withPart)
	{
		vector<Field*> attr;
		vector<string> attrParts;
		int dotPos;
		if(matcher->withHasTwoElem(withPart))
		{
			attrParts = split(withPart,'=');

			for(size_t i = 0; i < attrParts.size() ; i ++)
			{
				attrParts[i].erase(remove_if(attrParts[i].begin(), attrParts[i].end(), ptr_fun<int, int>(isspace)), attrParts[i].end());
				if(matcher->isString(attrParts[i]))
				{
					attrParts[i].erase(matcher->getPosition(attrParts[i],"\""),1);
					attrParts[i].erase(matcher->getPosition(attrParts[i],"\""),1);
					attr.push_back(new Field("string",attrParts[i]));
				}
				else
				{
					dotPos = attrParts[i].find('.');

					if(dotPos < attrParts[i].length())
					{
						Field* field = findField(attrParts[i].substr(0,dotPos));
						if(field == nullptr)
						{
							if(matcher->czyLiczba(attrParts[i]))
								attr.push_back(new Field("constant",attrParts[i]));
							else
								exc->throwInvalidWithStatement();
						}
						else
						{
							attr.push_back(new Field(field->getTyp(),
													field->getWartosc(),
													matcher->checkNazwaProcedury(attrParts[i]),
													matcher->checkVarName(attrParts[i]),
													matcher->checkValue(attrParts[i]),
													matcher->checkStmt_(attrParts[i])));
						}
					}
					else
					{
						Field* field = findField(attrParts[i]);
						if(field == nullptr)
						{
							if(matcher->czyLiczba(attrParts[i]))
								attr.push_back(new Field("constant",attrParts[i]));
							else
								exc->throwInvalidWithStatement();
						}
						else
						{
							attr.push_back(new Field(field->getTyp(),field->getWartosc()));
						}
					}
				}
			}
		}
		else
		{
			exc->throwToMuchArguments();
		}

		return attr;
	}

	Field* findField(string name)
	{
		for(size_t i = 0 ; i < fields.size() ; i ++)
		{
			if(fields[i].getWartosc() == name) return &fields[i];
		}
		return nullptr;
	}

	int checkType(string queryPart)
	{
		if(queryPart.find("Select") < queryPart.length()) return 1;
		if(queryPart.find("such that") < queryPart.length()) return 2;
		if(queryPart.find("with") < queryPart.length()) return 3;

		return 0;
	}

	int aktPos;
	int lastPos;
	int queryLength;
	string aktToken;
	string lastToken;
	vector<string> queryParts;

	vector<string> splitQuery(string query,vector<string> tokensElems)
	{
		aktPos = aktToken.length();
		lastPos = query.length();
		queryLength = query.length();
		findPositions(query, tokensElems);

		return queryParts;
	}

	void findPositions(string query, vector<string> tokens)
	{
		bool isnext = false;
		int tmpPos;
		for(size_t i = 0 ; i < tokens.size() ; i ++)
		{
			tmpPos = query.find(tokens[i], aktPos);
			if(tmpPos > 0 && tmpPos < lastPos && tmpPos < query.length())
			{
				isnext = true;
				lastPos = tmpPos;
				lastToken = aktToken;
				aktToken = tokens[i];
			}
		}
		if(isnext)
			queryParts.push_back(query.substr(aktPos-lastToken.length(),lastPos));
		else
			queryParts.push_back(query);


		query = query.substr(lastPos,queryLength);

		if(getNextPosition(tokens, query)) splitQuery(query, tokens);
	}

	bool getNextPosition(vector<string> tokens, string query)
	{
		for(size_t i = 0 ; i < tokens.size() ; i ++)
		{
			if(query.find(tokens[i]) < query.length())
				return true;
		}
		return false;
	}

public:
	vector<Field>& getFields() {
		return fields;
	}

	void setFields(vector<Field>& fields) {
		this->fields = fields;
	}

	PQLTree*& getDrzewo() {
		return PqlTree;
	}

private:
	vector<Field> fields;
};

#endif /* SRC_QUERYPREPROCESSOR_H_ */
