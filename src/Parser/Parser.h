#ifndef PARSER_H_
#define PARSER_H_

#include "../globals.h"

class Parser {

public:
	ASTTree* root = new ASTTree();
	bool fileExists = false;

	void parse() {
		LinesTable * linesTable = pkb->getTablicaLiniiKodu();
		string line;

		getline(stream, line);
		linesTable->addLine(line);
		count++;
		parseLine(line);
		while (!stream.eof()){
			getline(stream, line);
			linesTable->addLine(line);
			count++;
			parseLine(line);
//			root->printTree();
		}
	}

	Parser(string fileName) {
		tree_node_<ASTNode*>* program = NodeUtil::createProgramNode();
		tree<tree_node_<ASTNode*>*>::iterator it = root->appendRoot(program);
		openedNodes.push(it);
		stream.open(fileName.c_str());

		if (stream.good() && stream.is_open()) {
			fileExists = true;
			parsers.push_back(Syntax::allSynstax[Keywords::PROCEDURE]);
			parsers.push_back(Syntax::allSynstax[Keywords::WHILE]);
			parsers.push_back(Syntax::allSynstax[Keywords::CALL]);
			parsers.push_back(Syntax::allSynstax[Keywords::ASSIGN]);
			parsers.push_back(Syntax::allSynstax[Keywords::IF]);

			parsers.push_back(Syntax::allSynstax[Keywords::ELSE]);
			parsers.push_back(Syntax::allSynstax[NodeName::FAKE_OPEN]);
		}
		else {}
	}

private:
	stack<tree<tree_node_<ASTNode*>*>::iterator> openedNodes;

	int count = 0;
	fstream stream;
	vector<Syntax*> parsers;
	bool debug;
	int lines = 1;

	void rtrim(string &s) {
		s.erase(
				find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
				s.end());
	}

	void ltrim(string &s) {
		s.erase(s.begin(),
				find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	}

	void trim(string &s) {
		ltrim(s);
		rtrim(s);
	}

	void parseLine(string str) {
		Syntax::currLine = lines;
		Syntax::realLine = count;

		string str2 = str;
		ASTTree* node = NULL;
		string line;
		Syntax* s;
		trim(str);
		int closed;

		vector<Syntax*>::iterator it = parsers.begin();

	if (str.length() == false) {
			return;
		}

		while (it != parsers.end()){
				s = *it;
				node = s->parseLine(str);

				if (node != 0) {
					string type = (*node->getKorzenDrzewa())->data->type;

					if (type != NodeName::FAKE_CLOSE) {
						if (type != NodeName::FAKE_OPEN) {
							if (type != NodeName::PROCEDURE) {
								if (type != NodeName::ELSE) {
								lines++;
								}
							}
						tree<tree_node_<ASTNode*>*>::iterator pos = NodeUtil::appendChild(root, openedNodes.top(), node);

						if ((*node->getKorzenDrzewa())->data->newLevel) {
							openedNodes.push(pos);
						}
						for (closed = (*node->getKorzenDrzewa())->data->closed; closed>0; closed--){
							openedNodes.pop();
						}
						break;
					}
				}
				}
				else
				{
					//nothing
				}

				++it;
		}
		if (node) {
			if((*node->getKorzenDrzewa())->data->type == NodeName::FAKE_CLOSE){
				for (closed = (*node->getKorzenDrzewa())->data->closed; closed > 0; closed-- ){
						openedNodes.pop();
					}
			}
		}
		else
		{
			//nothing
		}

	}

	string toLower(string str) {
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}

	void usunBialeZnaki(string &str) {
		str.erase(remove_if(str.begin(), str.end(), ptr_fun<int, int>(isspace)),
				str.end());
	}
};

#endif
