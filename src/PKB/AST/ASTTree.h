#ifndef SRC_PKB_AST_ASTTree_H_
#define SRC_PKB_AST_ASTTree_H_

using namespace std;

class ASTTree {
private:
	//static ASTTree *Atree;

public:
	ASTTree() {

		Tree = new tree<tree_node_<ASTNode*>*>();

	}
	tree<tree_node_<ASTNode*>*> *Tree;

	~ASTTree() {
	}

	//Wstawianie w�ze�a jako ostatniego lub pierwszego dziecka w�z�a wskazanego przez pozycj�
	tree<tree_node_<ASTNode*>*>::iterator appendChild(tree<tree_node_<ASTNode*>*>::iterator& nodeIter, tree_node_<ASTNode*>*& child) {
		return Tree->append_child(nodeIter, child);
	}
	tree<tree_node_<ASTNode*>*>::iterator appendRoot(tree_node_<ASTNode*>*& node) {
		Tree->set_head(node);
		return Tree->begin();
	}
	//Wstawianie w�ze�a jako nast�pnego rodze�stwa w�z�a wskazywanego przez pozycj�
	tree<tree_node_<ASTNode*>*>::iterator appendSibling(tree<tree_node_<ASTNode*>*>::iterator& sibling, tree_node_<ASTNode*>*& node) {
		return Tree->insert_after(sibling, node);
	}
	//Wstawianie w�ze�a jako poprzedniego rodze�stwo w�z�a wskazywanego przez pozycj�
	tree<tree_node_<ASTNode*>*>::iterator appendLeftSibling(tree<tree_node_<ASTNode*>*>::iterator& sibling, tree_node_<ASTNode*>*& node) {
		return Tree->insert(sibling, node);
	}
	//zwracanie n-tego dziecka w�z�a na tej pozycji
	tree<tree_node_<ASTNode*>*>::iterator getDziecko(tree<tree_node_<ASTNode*>*>::iterator& nodeIter, uint16_t childNumber) {
		return Tree->child(nodeIter, childNumber);
	}
	//zwracanie iteratora do pozycji rodzica wezla
	tree<tree_node_<ASTNode*>*>::iterator getParent(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->parent(nodeIter);
	}
	//Zwracanie iteratora do nast�pnego rodze�stwa w�z�a
	tree<tree_node_<ASTNode*>*>::iterator getNextSibling(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->next_sibling(nodeIter);
	}
	//Zwracanie iteratora do poprzedniego rodze�stwa w�z�a
	tree<tree_node_<ASTNode*>*>::iterator getPrevSibling(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->previous_sibling(nodeIter);
	}
	//obliczanie liczby dzieci wezla na tej pozycji
	int getLiczbaDzieci(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->number_of_children(nodeIter);
	}
	//sprawdzanie czy drzewo jest puste
	bool isEmpty() {
		return Tree->empty();
	}
	//zliczanie wszystkich wezlow
	int getSize() {
		return Tree->size();
	}


	//zliczanie wszystkich wezlow ponizej wskazanego wezla (+1)
	int getSize(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->size(nodeIter);
	}
	//zliczanie rodze�stwa (lewego i prawego) w�z�a w iteratorze. ��czna liczba w�z��w na tym poziomie wynosi +1
	int getNumberOfSibilings(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->number_of_siblings(nodeIter);
	}

	//obliczanie g��boko�ci do roota lub do poprzedniego(stalego) iteratora
	int getDepth(tree<tree_node_<ASTNode*>*>::iterator& nodeIter){
		return Tree->depth(nodeIter);
	}

	tree<tree_node_<ASTNode*>*>::iterator appendSubTree(tree<tree_node_<ASTNode*>*>::iterator pos, ASTTree* subtree) {
		return Tree->append_child(pos, subtree->getKorzenDrzewa());
	}

	tree<tree_node_<ASTNode*>*>::iterator getKorzenDrzewa() {
		return Tree->begin();
	}

	//zwracanie iteratora rodze�stwa do pierwszego potomka danego w�z�a
	tree<tree_node_<ASTNode*>*>::iterator getFirstChild(tree<tree_node_<ASTNode*>*>::iterator& it) {
			return Tree->begin(it);
		}
	//zwracanie iteratora ko�ca rodze�stwa dla dzieci danego w�z�a
	tree<tree_node_<ASTNode*>*>::iterator getLastChild(tree<tree_node_<ASTNode*>*>::iterator& it) {
				return Tree->end(it);
			}

	tree<tree_node_<ASTNode*>*>::iterator getKoniec() {
			return Tree->end();
		}


	tree<tree_node_<ASTNode*>*>::leaf_iterator getKoniecOfLeaf(tree<tree_node_<ASTNode*>*>::iterator& it){
			return Tree->end_leaf(it);
		}

	tree<tree_node_<ASTNode*>*>::leaf_iterator getBeginLeaf(tree<tree_node_<ASTNode*>*>::iterator& it){
				return Tree->begin_leaf(it);
			}

	tree<tree_node_<ASTNode*>*>::iterator parent(tree<tree_node_<ASTNode*>*>::iterator& it) {
			return Tree->parent(it);
		}
	//sprawdzanie czy iterator jest juz w drzewie
	bool czyPrawidlowe(tree<tree_node_<ASTNode*>*>::iterator& it){
		return Tree->is_valid(it);
	}

	void printTree() {
		tree<tree_node_<ASTNode*>*>::iterator begin = Tree->begin();
		tree<tree_node_<ASTNode*>*>::iterator end = Tree->end();
		while (begin != end) {
			for (int i = 0; i < Tree->depth(begin); ++i) {
				cout << "    |";
			}
			if (!(*begin)->data) {
				cout << "??" << endl;
			} else {
				cout << " \\ " << "(" << (*begin)->data->type << " : " << (*begin)->data->value << ")" <<" : " << (*begin)->data->liNr1 << ")" << endl;
			}
			++begin;
		}
	}

};
#endif
