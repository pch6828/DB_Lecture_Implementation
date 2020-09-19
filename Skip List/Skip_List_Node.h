#ifndef __SKIP_LIST_NODE__
#define __SKIP_LIST_NODE__

#include <iostream>

template<class key_t, class value_t>
class Skip_List_Node {
private:
	key_t key;
	value_t value;
	int level;

	Skip_List_Node<key_t, value_t>** prev, ** next;
	bool find_in_list(int level, key_t key, value_t*& value);
	void insert_in_list(Skip_List_Node<key_t, value_t>* now, int level);
	Skip_List_Node<key_t, value_t>* pull_out_node(int level, key_t key);
	void reconnect_list();
public:
	Skip_List_Node<key_t, value_t>(int level) : level(level) {
		prev = new Skip_List_Node<key_t, value_t> * [level];
		next = new Skip_List_Node<key_t, value_t> * [level];

		for (int i = 0; i < level; i++) {
			prev[i] = next[i] = NULL;
		}
	}
	~Skip_List_Node() {
		delete[] this->prev;
		delete[] this->next;
	}
	Skip_List_Node<key_t, value_t>(int level, key_t key, value_t value) : level(level), key(key), value(value) {
		prev = new Skip_List_Node<key_t, value_t> * [level];
		next = new Skip_List_Node<key_t, value_t> * [level];

		for (int i = 0; i < level; i++) {
			prev[i] = next[i] = NULL;
		}
	}

	bool find(key_t key, value_t*& value);
	void insert(int level, key_t key, value_t value);
	void erase(key_t key);
	void print_list();
	void delete_all();
};

template<class key_t, class value_t>
bool Skip_List_Node<key_t, value_t>::find_in_list(int level, key_t key, value_t*& value) {
	if (this->key == key) {
		value = &(this->value);
		return true;
	}
	for (int i = level - 1; i >= 0; i--) {
		if (this->next[i]&&this->next[i]->key<=key) {
			return this->next[i]->find_in_list(i + 1, key, value);
		}
	}
	return false;
}

template<class key_t, class value_t>
bool Skip_List_Node<key_t, value_t>::find(key_t key, value_t*& value) {
	for (int i = this->level - 1; i >= 0; i--) {
		if (this->next[i] && this->next[i]->key <= key) {
			return this->next[i]->find_in_list(i + 1, key, value);
		}
	}
	return false;
}

template<class key_t, class value_t>
void Skip_List_Node<key_t, value_t>::insert_in_list(Skip_List_Node<key_t, value_t>* now, int level) {
	if (now->next[level - 1] && now->next[level - 1]->key < key) {
		insert_in_list(now->next[level - 1], level);
		return;
	}
	else {
		if (this->level >= level) {
			this->next[level - 1] = now->next[level - 1];
			if (now->next[level - 1]) {
				now->next[level - 1]->prev[level - 1] = this;
			}
			now->next[level - 1] = this;
			this->prev[level - 1] = now;
		}
		level--;
		if (level) {
			insert_in_list(now, level);
		}
	}
}

template<class key_t, class value_t>
void Skip_List_Node<key_t, value_t>::insert(int level, key_t key, value_t value) {
	Skip_List_Node<key_t, value_t>* new_node = new Skip_List_Node<key_t, value_t>(level, key, value);

	new_node->insert_in_list(this, level);
}

template<class key_t, class value_t>
void Skip_List_Node<key_t, value_t>::reconnect_list(){
	for (int i = 0; i < this->level; i++) {
		Skip_List_Node<key_t, value_t>* nx = this->next[i], * pr = this->prev[i];
		pr->next[i] = nx;
		if (nx) {
			nx->prev[i] = pr;
		}
	}
}


template<class key_t, class value_t>
Skip_List_Node<key_t, value_t>* Skip_List_Node<key_t, value_t>::pull_out_node(int level, key_t key) {
	if (this->key == key) {
		this->reconnect_list();
		return this;
	}
	for (int i = level - 1; i >= 0; i--) {
		if (this->next[i] && this->next[i]->key <= key) {
			return this->next[i]->pull_out_node(i + 1, key);
		}
	}

	return NULL;
}


template<class key_t, class value_t>
void Skip_List_Node<key_t, value_t>::erase(key_t key) {
	Skip_List_Node<key_t, value_t>* target = NULL;
	for (int i = this->level - 1; i >= 0; i--) {
		if (this->next[i] && this->next[i]->key <= key) {
			target = this->next[i]->pull_out_node(i + 1, key);
			break;
		}
	}
	if (target) {
		delete target;
	}
}

template<class key_t, class value_t>
void Skip_List_Node<key_t, value_t>::print_list() {
	std::cout << this->key << " ";
	if (this->next[0]) {
		this->next[0]->print_list();
	}
}

template<class key_t, class value_t>
void Skip_List_Node<key_t, value_t>::delete_all() {
	if (this->next[0]) {
		this->next[0]->delete_all();
	}
	delete next[0];
}
#endif