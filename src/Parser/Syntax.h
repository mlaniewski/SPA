#ifndef TYPES_SYNTAX_H_
#define TYPES_SYNTAX_H_
#include "../exceptions.h"

class Syntax {
protected:
	int braces = 0;
	bool multiLine = false;
	bool semicolon = true;
	static string anyWord;
	static string any;
	vector<Matcher> syntax;


	string toLower(string str) {
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}

	void ltrim(string &s) {
		s.erase(s.begin(),
				find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	}

	void rtrim(string &s) {
		s.erase(
				find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
				s.end());
	}

	void trim(string &s) {
		ltrim(s);
		rtrim(s);
	}

	void usunBialeZnaki(string &str) {
		str.erase(remove_if(str.begin(), str.end(), ptr_fun<int, int>(isspace)),
				str.end());
	}

	bool czyLitera(char c) {
		return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
	}

	bool czyLiczba(char c) {
		int number = c - 48;
		return number >= 0 && number <= 9;
	}

	bool isVar(string name) {
		if (!czyLitera(name[0])) {
			return false;
		}
		for (unsigned i = 1; i < name.size(); i++) {
			if (!czyLiczba(name[i]) && !czyLitera(name[i])) {
				return false;
			}
		}
		return true;
	}

	bool czyLiczba(string name) {
		for (unsigned i = 1; i < name.size(); i++) {
			if (!czyLiczba(name[i])) {
				return false;
			}
		}
		return true;
	}

	int getNawias() {
		int b = braces;
		braces = 0;
		return b;
	}

	void throwException(string msg, bool runtime = false) {
		Exception e;
		if (runtime) {
			e = RuntimeException();
		} else {
			e = Exception();
		}
		e.line = realLine;
		e.parser = keyWord;
		e.msg = msg;
		throw e;
	}

	vector<string> match(string str) {
		vector < string > vars;
		int position = 0;
		int newPosition = 0;
		if (semicolon) {
			int bracePos = str.find("}");
			int semicolonPos = str.find(";");
			if (semicolonPos == string::npos) {
				throwException("no semicolon found");
			} else if (bracePos != string::npos && bracePos > semicolonPos) {
				string end = str.substr(semicolonPos + 1);
				usunBialeZnaki(end);
				braces = count(end.begin(), end.end(), '}');
				string bracesStr = str.substr(bracePos, str.length() - 1);
				if (braces != end.length()) {
					throwException("denied characters after semicolon");
				}
				str.erase(remove(str.begin(), str.end(), '}'), str.end());
			} else if (semicolonPos != (int) str.length() - 1) {
				throwException("denied characters after semicolon");
			}
			str.erase(semicolonPos, 1);
		}
		if (multiLine) {
			int ocount = count(str.begin(), str.end(), '{');
			int ccount = count(str.begin(), str.end(), '}');
			if (max(ocount, ccount) != ocount + ccount) {
				throwException("invalid braces!", true);
			}
			int bracePos = max((int) str.find_last_of('{'), (int) str.find_last_of('}'));
			bool nextNoBrace = str.back() != '{' && str.back() != '}';
			if (noBrace && nextNoBrace || bracePos != str.length() - 1 && bracePos != string::npos) {
				throwException("missing brace!", true);
			}
			noBrace = nextNoBrace;
			if (!noBrace) {
				int bracePos = max((int) str.find('}'), (int) str.find('{'));
				string end = str.substr(bracePos);
				usunBialeZnaki(end);
				string str2 = str;
				usunBialeZnaki(str2);
				if (count(end.begin(), end.end(), end[0]) == end.size() && end.size() == str2.size()) {
					throwException(end, false);
				}
			}
			if (!nextNoBrace) {
				str.pop_back();
			}
		}

		for (vector<Matcher>::iterator it = syntax.begin(); it != syntax.end(); ++it) {
			if (it != syntax.end() - 1) {
				vector<Matcher>::iterator it2 = next(it, 1);
				Matcher m = *it2;
				newPosition = it->match(str, position, &m);
			} else {
				newPosition = it->match(str, position);
			}
			if (!it->strict && (it->word == any || it->word == anyWord)) {
				string result = str.substr(position, newPosition - position + 1);
				trim(result);
				vars.push_back(result);
			}
			position = newPosition;
		}
		return vars;
	}

	vector<string> split(string s) {
		istringstream iss(s);
		vector < string > tokens;
		copy(istream_iterator < string > (iss), istream_iterator<string>(), back_inserter(tokens));
		return tokens;
	}

public:
	vector<Syntax*> possibleChildren;
	static map<string, Syntax*> allSynstax;
	static int currLine;
	static int realLine;
	static bool noBrace;
	string keyWord;
	virtual ASTTree* parseLine(string str)=0;
	virtual ~Syntax() {
	}
};
string Syntax::anyWord = "*";
string Syntax::any = "**";
int Syntax::currLine = 0;
int Syntax::realLine = 0;
bool Syntax::noBrace = false;
map<string, Syntax*> Syntax::allSynstax;

class RecursiveSyntax: public Syntax {
protected:

	string toString(vector<string> str) {
		stringstream ss;
		for (size_t i = 0; i < str.size(); ++i) {
			ss << str[i] << " ";
		}
		return ss.str();
	}

	ASTTree* parse(vector<string> line) {
		string str = toString(line);
		trim(str);
		if (!str.length()) {
			return NULL;
		}
		return parse(str);
	}

	ASTTree* parse(string str) {
		ASTTree* node = NULL;
		for (vector<Syntax*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
			try {
				node = (*it)->parseLine(str);
				if (node) {
					break;
				}
			} catch (Exception &e) {
			/*	if (debug) {
					e.print();
				}*/
			}
		}
		if (!node) {
			RuntimeException e = RuntimeException();
			e.line = realLine;
			e.parser = keyWord;
			e.msg = "could not parse string " + str;
			throw e;
		}
		return node;
	}

public:
	vector<Syntax*> parsers;

};

class ProcedureSyntax: public Syntax {
protected:
	static map<string, tree_node_<ASTNode*>*> procedures;

public:
	ProcedureSyntax() {
		keyWord = Keywords::PROCEDURE;
		syntax = {Matcher(keyWord,Matcher::anyWord, Matcher::space), Matcher(Matcher::anyWord,Matcher::space, Matcher::anyWord)};
		multiLine = true;
		semicolon = false;
	}

	ASTTree* parseLine(string str) {
		vector < string > splitStr = split(str);
		if (toLower(splitStr[0]) != keyWord) {
			return NULL;
		}
		vector < string > args = match(str);
		if (procedures[args[0]]) {
			throwException("procedure " + args[0] + " already exists!", true);
		}
		ASTTree* node = NodeUtil::createProcedureNode(args[0], currLine);
		procedures[args[0]] = *(node->getKorzenDrzewa());
		return node;
	}

	void validate(ASTTree* t, tree<tree_node_<ASTNode*>*>::iterator n) {
		n = t->getParent(n);
		if ((*n)->data->type != NodeName::PROGRAM) {
			throwException("procedure node can be child only of program node!", true);
		}
	}
};
map<string, tree_node_<ASTNode*>*> ProcedureSyntax::procedures;

class OperandSyntax: public Syntax {
public:
	OperandSyntax() {
		keyWord = Keywords::OPERAND;
		semicolon = false;
	}

	ASTTree* parseLine(string str) {
		if (isVar(str) || czyLiczba(str)) {
			return NodeUtil::createOperandNode(str, currLine);
		}
		return NULL;
	}
};

class ParentsisesSyntax: public RecursiveSyntax {
public:
	ParentsisesSyntax() {
		keyWord = Keywords::PARENTSISES;
		syntax = {Matcher("("), Matcher(Matcher::anyWord,Matcher::anyWord, Matcher::anyWord), Matcher(")")};
		semicolon = false;
	}

	ASTTree* parseLine(string str) {
		if (str.find("(") == string::npos || str.find(")") == string::npos) {
			return NULL;
		}
		vector < string > args = match(str);
		ASTTree* result = parse(args[0]);
		if (!result) {
			throwException("no argument for operand " + keyWord, true);
		}
		return result;
	}
};

class MathSyntax: public RecursiveSyntax {

public:
	MathSyntax(string expr, bool strict = false) {
		semicolon = false;
		keyWord = expr;
		syntax = {Matcher(Matcher::anyWord), Matcher(keyWord, Matcher::anyWord, Matcher::anyWord, strict), Matcher(Matcher::any)};
		parsers.push_back(allSynstax["op"]);
	}

	ASTTree* parseLine(string str) {
		if (str.find(keyWord) == string::npos) {
			return NULL;
		}
		vector < string > args = match(str);
		ASTTree* n = NodeUtil::createExpressionNode(keyWord, currLine);
		ASTTree* lResult = parse(args[0]);
		if (lResult) {
			NodeUtil::appendChild(n, lResult);
		}

		ASTTree* rResult = parse(args[1]);
		if (lResult) {
			NodeUtil::appendChild(n, rResult);
		}
		if (!lResult || !rResult) {
			throwException("no argument for operand " + keyWord, true);
		}
		return n;
	}
};

class AssingmentSyntax: public RecursiveSyntax {
public:
	AssingmentSyntax() {
		keyWord = Keywords::ASSIGN;
		syntax = {Matcher(Matcher::anyWord), Matcher(keyWord), Matcher(any)};
		parsers.push_back(allSynstax["op"]);
	}

	ASTTree* parseLine(string str) {
		if (str.find(keyWord) == string::npos) {
			return NULL;
		}
		vector < string > args = match(str);
		string name = args.front();
		if (!isVar(name)) {
			throwException(name + " is not variable name!", true);
		}
		ASTTree* var = NodeUtil::createOperandNode(args.front(), currLine);
		args.erase(args.begin());
		ASTTree* n = NodeUtil::createAssignmentNode(currLine);
		(*(n->getKorzenDrzewa()))->data->closed = getNawias();
		NodeUtil::appendChild(n, var);

		ASTTree* result = parse(args);
		if (result) {
			NodeUtil::appendChild(n, result);
		} else {
			throwException("could not parse expression");
		}
		return n;
	}
};
class WhileSyntax: public Syntax {
public:
	WhileSyntax() {
		keyWord = Keywords::WHILE;
		syntax = {Matcher(keyWord, Matcher::any, Matcher::space), Matcher(anyWord)};
		multiLine = true;
		semicolon = false;
	}

	ASTTree* parseLine(string str) {
		vector < string > splitStr = split(str);
		if (str.find(keyWord) == string::npos) {
			return NULL;
		}
		vector < string > args = match(str);
		string name = args.front();
		if (!isVar(name)) {
			throwException(name + " is not variable name!", true);
		}
		ASTTree* n = NodeUtil::createWhileNode(name, currLine);
		return n;
	}
};

class IfSyntax: public Syntax {
public:
	IfSyntax() {
		keyWord = Keywords::IF;
		syntax = {Matcher(keyWord,Matcher::anyWord, Matcher::space), Matcher(Matcher::anyWord,Matcher::space, Matcher::space),
			Matcher("then")};
		semicolon = false;
		multiLine = true;
	}

	ASTTree* parseLine(string str) {
		vector < string > splitStr = split(str);
		if (toLower(splitStr[0]) != keyWord) {
			return NULL;
		}
		vector < string > args = match(str);
		if (!isVar(args[0])) {
			throwException(args[0] + " is not variable name!", true);
		}
		ASTTree* node = NodeUtil::createIfNode(args[0], currLine);
		return node;
	}
};

class ElseSyntax: public Syntax {
public:
	ElseSyntax() {
		keyWord = Keywords::ELSE;
		syntax = {Matcher(keyWord,Matcher::anyWord, Matcher::anyWord)};
		semicolon = false;
		multiLine = true;
	}

	ASTTree* parseLine(string str) {
		vector < string > splitStr = split(str);
		if (str.find(keyWord) == string::npos) {
			return NULL;
		}
		match(str);
		ASTTree* node = NodeUtil::createElseNode(currLine);
		return node;
	}

	void validate(ASTTree* t, tree<tree_node_<ASTNode*>*>::iterator n) {
		n = t->getPrevSibling(n);
		if ((*n)->data->type != NodeName::IF) {
			throwException("else node could be placed only after if node!", true);
		}
	}
};

class EmptySyntax: public Syntax {
public:
	EmptySyntax() {
		syntax = {};
		keyWord = NodeName::FAKE_OPEN;
		semicolon = false;
		multiLine = true;
	}

	ASTTree* parseLine(string str) {
		vector < string > splitStr = split(str);
		string name="";
		try {
			match(str);
		} catch(Exception &e) {
			if(e.runtime) {
				throw e;
			}
			name=e.msg;
		}
		if(name[0]=='}') {
			return NodeUtil::createFakeCloseNode(name.length());
		} else {
			return NodeUtil::createFakeOpenNode();

		}
	}
};

#endif /* TYPES_SYNTAX_H_ */
