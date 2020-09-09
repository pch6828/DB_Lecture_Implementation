#ifndef ___T_TREE___
#define ___T_TREE___

#include "Key_Value.h"
#include<iostream>
template<class key_t, class value_t>
class T_Tree_Node {
private:
	T_Tree_Node* left_child, * right_child, * parent;
	Key_Value<key_t, value_t> records[3];
	key_t min_key, max_key;
	int size, height;

	int insert_at_node(key_t key, value_t value);
public:
	T_Tree_Node() :left_child(NULL), right_child(NULL), parent(NULL), min_key(NONE_KEY), max_key(NONE_KEY) {
		for (int i = 0; i < 3; i++) {
			records[i] = Key_Value<key_t, value_t>();
		}
	}

	int find(key_t key, value_t* dist);
	int insert(key_t key, value_t value);
	void print_inorder();
};

template<class key_t, class value_t>
int T_Tree_Node<key_t, value_t>::find(key_t key, value_t* dist) {
	if (min_key <= key && key <= max_key) {
		for (int i = 0; i < size; i++) {
			if (records[i].is_valid() && records[i].get_key() == key) {
				*dist = records[i].get_value();
				return 0;
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
	return 1;
}

template<class key_t, class value_t>
int  T_Tree_Node<key_t, value_t>::insert_at_node(key_t key, value_t value) {
	records[size++] = Key_Value<key_t, value_t>(key, value);
	for (int i = size - 1; i > 0; i++) {
		if (records[i].get_key() > records[i - 1].get_key()) {
			break;
		}
		swap(records[i], records[i - 1]);
	}
	min_key = records[0].get_key();
	max_key = records[size - 1].get_key();
	return 0;
}

template<class key_t, class value_t>
int T_Tree_Node<key_t, value_t>::insert(key_t key, value_t value) {
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
		return left_child->insert(key, value);
	}
	else if (max_key < key) {
		if (!right_child) {
			if (size != 3) {
				return this->insert_at_node(key, value);
			}
			right_child = new T_Tree_Node();
			right_child->parent = this;
		}
		return right_child->insert(key, value);
	}
}

template<class key_t, class value_t>
void T_Tree_Node<key_t, value_t>::print_inorder() {
	if (left_child) {
		left_child->print_inorder();
	}
	for (int i = 0; i < size; i++) {
		std::cout << records[i].get_key() << " ";
	}
	if (right_child) {
		right_child->print_inorder();
	}
}
#endif