#ifndef KEYWORDS_H_
#define KEYWORDS_H_

class NodeName {
public:
	static string ASSIGN;
	static string CALL;
	static string ELSE;
	static string EXPR;
	static string FAKE_CLOSE;
	static string FAKE_OPEN;
	static string IF;
	static string OPERAND;
	static string PROCEDURE;
	static string PROGRAM;
	static string STM_LIST;
	static string WHILE;
};
string NodeName::ASSIGN = "ASSIGN";
string NodeName::CALL = "CALL";
string NodeName::ELSE = "ELSE";
string NodeName::EXPR = "EXPR";
string NodeName::FAKE_CLOSE = "FAKE_CLOSE";
string NodeName::FAKE_OPEN = "FAKE_OPEN";
string NodeName::IF = "IF";
string NodeName::OPERAND = "OPERAND";
string NodeName::PROCEDURE = "PROCEDURE";
string NodeName::PROGRAM = "PROGRAM";
string NodeName::STM_LIST = "STM_LIST";
string NodeName::WHILE = "WHILE";

class Keywords {
public:
	static string ASSIGN;
	static string CALL;
	static string ELSE;
	static string EXPR;
	static string IF;
	static string OPERAND;
	static string PARENTSISES;
	static string PROCEDURE;
	static string PROGRAM;
	static string WHILE;
};

string Keywords::ASSIGN = "=";
string Keywords::CALL = "call";
string Keywords::ELSE = "else";
string Keywords::EXPR = "expr";
string Keywords::IF = "if";
string Keywords::OPERAND = "op";
string Keywords::PARENTSISES = "PARENTISIES";
string Keywords::PROCEDURE = "procedure";
string Keywords::PROGRAM = "program";
string Keywords::WHILE = "while";
#endif /* KEYWORDS_H_ */

