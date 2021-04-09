#ifndef SRC_PQL_PQLNODE_H_
#define SRC_PQL_PQLNODE_H_

#include <string>
#include "Field.h"

using namespace std;

class PQLNode {
public:
	// dla ustawionego przez siebie pola
	PQLNode() {
	}

	PQLNode(string type) {
		this->type = type; //typy QueryNode,ResultMainNode,SuchMainNode
	}

	PQLNode(string type, Field* field1) {
		this->type = type; //typ resultNode
		this->field1 = field1;
	}

	PQLNode(string type, Field* field1, Field* field2) {
		this->type = type; //typ withNode
		this->field1 = field1;
		this->field2 = field2;
	}

	PQLNode(string type, string nodeType, Field* field1, Field* field2,
			bool star) {
		this->type = type; // typ suchNode
		this->nodeType = nodeType; // modifies, uses, call, parent, follows,
		this->field1 = field1;
		this->field2 = field2;
		this->star = star;
	}

	Field*& getField1() {
		return field1;
	}

	void setField1(Field*& field1) {
		this->field1 = field1;
	}

	Field*& getField2() {
		return field2;
	}

	void setField2(Field*& field2) {
		this->field2 = field2;
	}

	string& getNodeType() {
		return nodeType;
	}

	void setNodeType(string& nodeType) {
		this->nodeType = nodeType;
	}

	bool isStar() {
		return star;
	}

	void setStar(bool star) {
		this->star = star;
	}

	string& getType() {
		return type;
	}

	void setTyp(string& type) {
		this->type = type;
	}

private:
	//queryNode, resultMainNode, resultNode, suchMainNode, suchNode
	string type;
	//parent, modifies, uses, follows
	string nodeType;
	//Result Node - field1
	//suchNode - field1 w relations
	//withNode - czesc przed "="
	Field* field1;
	//resultNode - puste
	//suchNode - field2 w relations
	//withNode - field2 czesc po '='
	Field* field2;
	//suchField - tylko dla relacji z gwiazdkami lub nie
	bool star;
};

#endif /* SRC_PQL_PQLNODE_H_ */
