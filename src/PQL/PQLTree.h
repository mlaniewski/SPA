#ifndef SRC_PQL_PQLTREE_H_
#define SRC_PQL_PQLTREE_H_

#include "PQLNode.h"
#ifndef tree_hh_
#include "tree.hh"
#endif
#ifndef _GLIBCXX_IOSTREAM
#include <iostream>
#endif

using namespace std;

class PQLTree
{
private:
	//static PQLTree *Ptree;


public:

	tree<tree_node_<PQLNode*>*> *Tree;

	PQLTree() {
		Tree = new tree<tree_node_<PQLNode*>*>();
	}

	static PQLTree* getInstance()
	{
		PQLTree *Ptree;
		Ptree = new PQLTree();
		return Ptree;

	}

	~PQLTree()
	{
	}

	//Wstawianie w�ze�a jako ostatniego lub pierwszego dziecka w�z�a wskazanego przez pozycj�
	tree<tree_node_<PQLNode*>*>::iterator appendChild(tree<tree_node_<PQLNode*>*>::iterator& nodeIter,tree_node_<PQLNode*>*& child) {
		return Tree->append_child(nodeIter,child);
	}
	tree<tree_node_<PQLNode*>*>::iterator appendRoot(tree_node_<PQLNode*>*& node) {
		Tree->set_head(node);
		return Tree->begin();
	}
	//Wstawianie w�ze�a jako nast�pnego rodze�stwa w�z�a wskazywanego przez pozycj�
	tree<tree_node_<PQLNode*>*>::iterator appendSibling(tree<tree_node_<PQLNode*>*>::iterator& sibling, tree_node_<PQLNode*>*& node) {
		return Tree->insert_after(sibling, node);
	}
	//Wstawianie w�ze�a jako poprzedniego rodze�stwo w�z�a wskazywanego przez pozycj�
	tree<tree_node_<PQLNode*>*>::iterator appendLeftSibling(tree<tree_node_<PQLNode*>*>::iterator& sibling, tree_node_<PQLNode*>*& node) {
		return Tree->insert(sibling, node);
	}
	//zwracanie n-tego dziecka w�z�a na tej pozycji
	tree<tree_node_<PQLNode*>*>::iterator getDziecko(tree<tree_node_<PQLNode*>*>::iterator& nodeIter,uint16_t childNumber) {
		return Tree->child(nodeIter, childNumber);
	}
	//zwracanie iteratora do pozycji rodzica wezla
	tree<tree_node_<PQLNode*>*>::iterator getParent(tree<tree_node_<PQLNode*>*>::iterator& nodeIter) {
		return Tree->parent(nodeIter);
	}
	//Zwracanie iteratora do nast�pnego rodze�stwa w�z�a
	tree<tree_node_<PQLNode*>*>::iterator getNextSibling(tree<tree_node_<PQLNode*>*>::iterator& nodeIter) {
		return Tree->next_sibling(nodeIter);
	}
	//Zwracanie iteratora do poprzedniego rodze�stwa w�z�a
	tree<tree_node_<PQLNode*>*>::iterator getPrevSibling(tree<tree_node_<PQLNode*>*>::iterator& nodeIter) {
		return Tree->previous_sibling(nodeIter);
	}
	//obliczanie liczby dzieci wezla na tej pozycji
	int getLiczbaDzieci(tree<tree_node_<PQLNode*>*>::iterator& nodeIter) {
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
	int getSize(tree<tree_node_<PQLNode*>*>::iterator& nodeIter) {
		return Tree->size(nodeIter);
	}
	//zliczanie rodze�stwa (lewego i prawego) w�z�a w iteratorze. ��czna liczba w�z��w na tym poziomie wynosi +1
	int getNumberOfSibilings(tree<tree_node_<PQLNode*>*>::iterator& nodeIter) {
		return Tree->number_of_siblings(nodeIter);
	}

	//obliczanie g��boko�ci do roota lub do poprzedniego(stalego) iteratora
	tree<tree_node_<PQLNode*>*>::iterator getKorzenDrzewa() {
		return Tree->begin();
	}

	void printTree() {
		tree<tree_node_<PQLNode*>*>::iterator begin = Tree->begin();
		tree<tree_node_<PQLNode*>*>::iterator end = Tree->end();
		while (begin != end) {
			for (int i = 0; i<Tree->depth(begin); ++i)
				cout << "    |";
				cout << " \\ " << ((*begin)->data->getType());

				if((*begin)->data->getType() == "suchNode")
					cout << "[" << (*begin)->data->getField1()->printField() << "," << (*begin)->data->getField2()->printField() << "]";
				if((*begin)->data->getType() == "resultNode")
					cout << "[" << (*begin)->data->getField1()->printField() << "]";
				if((*begin)->data->getType() == "withNode")
					cout << "[" << (*begin)->data->getField1()->printField() << "," << (*begin)->data->getField2()->printField() << "]";

			cout << endl;
			++begin;
		}
	}


};

#endif /* SRC_PQL_PQLTREE_H_ */
