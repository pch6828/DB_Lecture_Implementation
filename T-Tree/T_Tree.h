#ifndef ___T_TREE___
#define ___T_TREE___

#include "T_Tree_Node.h"

template<class key_t, class value_t>
class T_Tree {
private:
	T_Tree_Node<key_t, value_t>* root;
public:
	T_Tree() {
		root = new T_Tree_Node<key_t, value_t>;
	}

	void insert(key_t key, value_t value);
	value_t* find(key_t key);

	void print_inorder();
};

template<class key_t, class value_t>
void T_Tree<key_t, value_t>::insert(key_t key, value_t value) {
	value_t* dist;
	if (!root->find(key, dist)) {
		root = root->insert(key, value);
	}
}

template<class key_t, class value_t>
value_t* T_Tree<key_t, value_t>::find(key_t key) {
	value_t* result = NULL;
	root->find(key, result);

	return result;
}

template<class key_t, class value_t>
void T_Tree<key_t, value_t>::print_inorder() {
	root->print_inorder();
}

#endif