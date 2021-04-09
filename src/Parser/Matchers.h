#ifndef PARSER_MATCHERS_H_
#define PARSER_MATCHERS_H_
#include "../exceptions.h"

class Matcher {
public:
	bool strict = false;
	string rSeparator = "";
	string lSeparator = "";
	static string space;
	static string any;
	static string anyWord;
	string word;

	Matcher(string word, string lSeparator = anyWord, string rSeparator = anyWord, bool strict = false) {
		this->lSeparator = lSeparator;
		this->rSeparator = rSeparator;
		this->strict = strict;
		this->word = word;
	}

	int matchWildcard(string s, int currentPos, Matcher* next) {
		if (word == any || (word == anyWord && !next)) {
			return s.length() - 1;
		}
		int pos = s.substr(currentPos, getKoniec(currentPos, s)).find(next->word.c_str());
		pos--;
		return pos + currentPos;
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

	int match(string s, int currentPos, Matcher* next = NULL) {
		if (!strict && (word == any || word == anyWord)) {
			return matchWildcard(s, currentPos, next);
		}
		s = toLower(s);
		int pos = s.substr(currentPos, getKoniec(currentPos, s)).find(word.c_str());
		return currentPos + pos + word.length();
	}

	void usunBialeZnaki(string &str) {
		str.erase(remove_if(str.begin(), str.end(), ptr_fun<int, int>(isspace)),
				str.end());
	}

	string toLower(string str) {
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}

private:
	int getKoniec(int pos, string str) {
		int len = str.length() - pos;
		return len;
	}
	void throwException(string msg) {
		Exception e = Exception();
		e.line = -1;
		e.parser = "?";
		e.msg = msg;
		throw e;
	}


protected:
	Matcher() {
	}
};
string Matcher::space = " ";
string Matcher::any = "**";
string Matcher::anyWord = "*";

#endif /* PARSER_MATCHERS_H_ */
