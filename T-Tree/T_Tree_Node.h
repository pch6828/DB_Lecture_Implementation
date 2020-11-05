#ifndef ___T_TREE_NODE___
#define ___T_TREE_NODE___

#include "Key_Value.h"
#include <algorithm>

//Class Definition for T-Tree Node
//Fields: 
//2 Child Pointers, Parent Pointer, key-value pointer pair
//Methods:
//Searching, Insertion, Deletion, Rotation, Inorder Printing(for debugging)
template<class key_t, class value_t>
class T_Tree_Node {
private:
	T_Tree_Node<key_t, value_t>* left_child, * right_child, * parent;
	Key_Value<key_t, value_t> records[3];
	key_t min_key, max_key;
	int size, height;

	T_Tree_Node<key_t, value_t>* insert_at_node(key_t key, value_t value);
	T_Tree_Node<key_t, value_t>* erase_at_node(key_t key);
	T_Tree_Node<key_t, value_t>* single_left_rotation();
	T_Tree_Node<key_t, value_t>* single_right_rotation();
	T_Tree_Node<key_t, value_t>* double_left_rotation();
	T_Tree_Node<key_t, value_t>* double_right_rotation();

public:
	T_Tree_Node() :left_child(NULL), right_child(NULL), parent(NULL), min_key(NONE_KEY), max_key(NONE_KEY) {
		size = 0;
		height = 0;
		for (int i = 0; i < 3; i++) {
			records[i] = Key_Value<key_t, value_t>();
		}
	}

	bool find(key_t key, value_t*& dist);
	T_Tree_Node<key_t, value_t>* insert(key_t key, value_t value);
	T_Tree_Node<key_t, value_t>* erase(key_t key);
	void print_inorder();
};

//Single Key Search Operation
//If there is corresponding key, return true and set dist to its value pointer
//else, return false
//
//should be executed unique key environment
//if there are duplicate keys, dist can be UB
template<class key_t, class value_t>
bool T_Tree_Node<key_t, value_t>::find(key_t key, value_t*& dist) {
	if (min_key <= key && key <= max_key) {
		for (int i = 0; i < size; i++) {
			if (records[i].is_valid() && records[i].get_key() == key) {
				dist = records[i].get_pointer();
				return true;
			}
		}
	}
	else if (min_key > key) {
		if (left_child) {
			return left_child->find(key, dist);
		}
	}
	else if (max_key < key) {
		if (right_child) {
			return right_child->find(key, dist);
		}
	}
	return false;
}

//Helper Function for Insertion
//Add new key-value pointer pair at node, and sort it, recompute key range of node
//
//should be executed when node has an empty slot
template<class key_t, class value_t>
T_Tree_Node<key_t, value_t>* T_Tree_Node<key_t, value_t>::insert_at_node(key_t key, value_t value) {
	records[size++] = Key_Value<key_t, value_t>(key, value);
	for (int i = size - 1; i > 0; i--) {
		if (records[i].get_key() > records[i - 1].get_key()) {
			break;
		}
		swap(records[i], records[i - 1]);
	}
	min_key = records[0].get_key();
	max_key = records[size - 1].get_key();
	return this;
}

//Single Key Insert Operation
//Add new key-value pointer pair at node, and rebalance it
//rebalancing is done by
// - send minimum record to child
// - re-construct tree structure (rotation)
template<class key_t, class value_t>
T_Tree_Node<key_t, value_t>* T_Tree_Node<key_t, value_t>::insert(key_t key, value_t value) {
	int lh = -1, rh = -1;
	T_Tree_Node<key_t, value_t>* result = this;
	if (min_key <= key && key <= max_key) {
		if (size != 3) {
			return this->insert_at_node(key, value);
		}
		key_t local_min = records[0].get_key();
		value_t local_min_val = records[0].get_value();

		records[0] = Key_Value<key_t, value_t>(key, value);
		for (int i = 1; i < size; i++) {
			if (records[i].get_key() > records[i - 1].get_key()) {
				break;
			}
			swap(records[i], records[i - 1]);
		}
		min_key = records[0].get_key();
		max_key = records[size - 1].get_key();

		return this->insert(local_min, local_min_val);
	}
	else if (min_key > key) {
		if (!left_child) {
			if (size != 3) {
				return this->insert_at_node(key, value);
			}
			left_child = new T_Tree_Node();
			left_child->parent = this;
		}
		left_child = left_child->insert(key, value);
		lh = left_child->height;
		if (right_child) {
			rh = right_child->height;
		}
		if (lh - rh == 2) {
			if (key < left_child->min_key) {
				result = this->single_left_rotation();
			}
			else if (key > left_child->max_key) {
				result = this->double_left_rotation();
			}
		}
	}
	else if (max_key < key) {
		if (!right_child) {
			if (size != 3) {
				return this->insert_at_node(key, value);
			}
			right_child = new T_Tree_Node();
			right_child->parent = this;
		}
		right_child = right_child->insert(key, value);
		rh = right_child->height;
		if (left_child) {
			lh = left_child->height;
		}
		if (rh - lh == 2) {
			if (key < right_child->min_key) {
				result = this->double_right_rotation();
			}
			else if (key > right_child->max_key) {
				result = this->single_right_rotation();
			}
		}
	}
	if (result->left_child) {
		lh = result->left_child->height;
	}
	if (result->right_child) {
		rh = result->right_child->height;
	}
	result->height = std::max(lh, rh) + 1;
	return result;
}

//Helper Function for Deletion
//Eliminate corresponding key-value pointer pair at node, and pack it, recompute key range of node
template<class key_t, class value_t>
T_Tree_Node<key_t, value_t>* T_Tree_Node<key_t, value_t>::erase_at_node(key_t key) {
	for (int i = 0; i < size; i++) {
		if (records[i].get_key() == key) {
			records[i] = Key_Value<key_t, value_t>();
			break;
		}
	}
	size--;
	for (int i = 0; i < size; i++) {
		if (!records[i].is_valid()) {
			swap(records[i], records[i + 1]);
		}
	}
	min_key = records[0].get_key();
	max_key = records[size - 1].get_key();
	return this;
}

//Single Key Delete Operation
//Eliminate corresponding key-value pointer pair 
//rebalancing is done by
// - get maximum key from left child
// - get minimum key from right child
// - re-construct tree structure (rotation)
template<class key_t, class value_t>
T_Tree_Node<key_t, value_t>* T_Tree_Node<key_t, value_t>::erase(key_t key) {
	int lh = -1, rh = -1;
	T_Tree_Node<key_t, value_t>* result = this;
	if (min_key <= key && key <= max_key) {
		erase_at_node(key);
		if (this->left_child) {
			T_Tree_Node<key_t, value_t>* now = this->left_child;
			while (now->right_child) {
				now = now->right_child;
			}
			key_t key = now->records[now->size-1].get_key();
			value_t value = now->records[now->size-1].get_value();
			insert_at_node(key, value);
			this->left_child = this->left_child->erase(key);
		}
		else if (this->right_child) {
			T_Tree_Node<key_t, value_t>* now = this->right_child;
			while (now->left_child) {
				now = now->left_child;
			}
			key_t key = now->records[0].get_key();
			value_t value = now->records[0].get_value();
			insert_at_node(key, value);
			this->right_child = this->right_child->erase(key);
		}
	}
	else if (min_key > key) {
		if (left_child) {
			left_child = left_child->erase(key);
		}
	}
	else if (max_key < key) {
		if (right_child) {
			right_child = right_child->erase(key);
		}
	}

	if (left_child && left_child->size == 0) {
		delete left_child;
		left_child = NULL;
	}
	if (right_child && right_child->size == 0) {
		delete right_child;
		right_child = NULL;
	}

	if (result->left_child) {
		lh = result->left_child->height;
	}
	if (result->right_child) {
		rh = result->right_child->height;
	}

	if (lh - rh == 2) {
		if (result->left_child->right_child) {
			result = result->double_left_rotation();
		}
		else {
			result = result->single_left_rotation();
		}
	}
	else if (rh - lh == 2) {
		if (result->right_child->left_child) {
			result = result->double_right_rotation();
		}
		else {
			result = result->single_right_rotation();
		}
	}
	lh = rh = -1;
	if (result->left_child) {
		lh = result->left_child->height;
	}
	if (result->right_child) {
		rh = result->right_child->height;
	}
	result->height = std::max(lh, rh) + 1;

	return result;
}

//Rotation Type 1 : Left-Left
template<class key_t, class value_t>
T_Tree_Node<key_t, value_t>* T_Tree_Node<key_t, value_t>::single_left_rotation() {
	T_Tree_Node<key_t, value_t>* k1, * k2;
	int lh, rh;
	k2 = this;
	k1 = k2->left_child;
	k1->parent = k2->parent;
	k2->left_child = k1->right_child;
	if (k2->left_child) {
		k2->left_child->parent = k2;
	}
	k2->parent = k1;
	k1->right_child = k2;
	lh = rh = -1;
	if (k2->left_child) {
		lh = k2->left_child->height;
	}
	if (k2->right_child) {
		rh = k2->right_child->height;
	}
	k2->height = max(lh, rh) + 1;
	lh = rh = -1;
	if (k1->left_child) {
		lh = k1->left_child->height;
	}
	if (k1->right_child) {
		rh = k1->right_child->height;
	}
	k1->height = max(lh, rh) + 1;
	return k1;
}

//Rotation Type 2 : Right-Right
template<class key_t, class value_t>
T_Tree_Node<key_t, value_t>* T_Tree_Node<key_t, value_t>::single_right_rotation() {
	T_Tree_Node<key_t, value_t>* k1, * k2;
	int lh, rh;
	k2 = this;
	k1 = k2->right_child;
	k1->parent = k2->parent;
	k2->right_child = k1->left_child;
	if (k2->right_child) {
		k2->right_child->parent = k2;
	}
	k2->parent = k1;
	k1->left_child = k2;
	lh = rh = -1;
	if (k2->left_child) {
		lh = k2->left_child->height;
	}
	if (k2->right_child) {
		rh = k2->right_child->height;
	}
	k2->height = max(lh, rh) + 1;
	lh = rh = -1;
	if (k1->left_child) {
		lh = k1->left_child->height;
	}
	if (k1->right_child) {
		rh = k1->right_child->height;
	}
	k1->height = max(lh, rh) + 1;
	return k1;
}

//Rotation Type 3 : Left-Right
template<class key_t, class value_t>
T_Tree_Node<key_t, value_t>* T_Tree_Node<key_t, value_t>::double_left_rotation() {
	this->left_child = this->left_child->single_right_rotation();
	return this->single_left_rotation();
}

//Rotation Type 4 : Right-Left
template<class key_t, class value_t>
T_Tree_Node<key_t, value_t>* T_Tree_Node<key_t, value_t>::double_right_rotation() {
	this->right_child = this->right_child->single_left_rotation();
	return this->single_right_rotation();
}

//For Debug Printing Operation
//While traversing tree by inorder manner,
//print all keys in each node
template<class key_t, class value_t>
void T_Tree_Node<key_t, value_t>::print_inorder() {
	if (left_child) {
		left_child->print_inorder();
	}
	cout << "[ ";
	for (int i = 0; i < size; i++) {
		std::cout << records[i].get_key() << " ";
	}
	cout << "] ";
	if (right_child) {
		right_child->print_inorder();
	}
}
#endif