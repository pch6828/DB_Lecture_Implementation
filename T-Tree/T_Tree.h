#ifndef ___T_TREE___
#define ___T_TREE___

#include "T_Tree_Node.h"

//Class Definition for T-Tree API
//Fields: 
//Root for T-Tree
//Methods:
//Searching, Insertion, Deletion, Inorder Printing(for debugging)
template<class key_t, class value_t>
class T_Tree {
private:
	T_Tree_Node<key_t, value_t>* root;
public:
	T_Tree() {
		root = new T_Tree_Node<key_t, value_t>;
	}

	void insert(key_t key, value_t value);
	void erase(key_t key);
	value_t* find(key_t key);

	void print_inorder();
};

//Single Key Insert API
//Since T-Tree might change its root by insertion, 
//It should be abstracted by it
//
//if there is corresponding key in the tree, it does nothing
template<class key_t, class value_t>
void T_Tree<key_t, value_t>::insert(key_t key, value_t value) {
	value_t* dist;
	if (!root->find(key, dist)) {
		root = root->insert(key, value);
	}
}

//Single Key Delete API
//Since T-Tree might change its root by deletion, 
//It should be abstracted by it
template<class key_t, class value_t>
void T_Tree<key_t, value_t>::erase(key_t key) {
		root = root->erase(key);
}

//Single Key Search API
//If there is corresponding key in the tree, return its value pointer,
//else return NULL
template<class key_t, class value_t>
value_t* T_Tree<key_t, value_t>::find(key_t key) {
	value_t* result = NULL;
	root->find(key, result);

	return result;
}

//For Debug Printing Operation
template<class key_t, class value_t>
void T_Tree<key_t, value_t>::print_inorder() {
	root->print_inorder();
}
#endif