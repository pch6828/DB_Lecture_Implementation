
#ifndef __SKIP_LIST__
#define __SKIP_LIST__

#include <random>
#include "Skip_List_Node.h"

//Class Definition for Skip List API
//Fields: 
//header node for Skip List, maximum level for Skip List, random device to create random level
//Methods:
//Search, Insertion, Deletion, Printing(for debugging)
template<class key_t, class value_t>
class Skip_List {
private:
	Skip_List_Node<key_t, value_t>* head;
	int max_level;
	std::random_device rd;
public:
	Skip_List<key_t, value_t>(int level = 1) : max_level(level) {
		head = new Skip_List_Node<key_t, value_t>(max_level);
	}
	~Skip_List() {
		head->delete_all();
		delete head;
	}

	value_t* find(key_t key);
	void insert(key_t key, value_t value);
	void erase(key_t key);
	void print_list();
};

//Single Key Search API
//If there is corresponding key in the list, return its value pointer,
//else return NULL
template<class key_t, class value_t>
value_t* Skip_List<key_t, value_t>::find(key_t key) {
	value_t* result = NULL;
	head->find(key, result);
	return result;
}

//Single Key Insert API
//create random level for new node, and insert it
//	
//if there are corresponding key in the list, it does nothing
template<class key_t, class value_t>
void Skip_List<key_t, value_t>::insert(key_t key, value_t value) {
	if (find(key)) {
		return;
	}
	int level = rd() % max_level + 1;
	head->insert(level, key, value);
}

//Single Key Delete API	
//if there are corresponding key in the list, delete its node
//else, it does nothing
template<class key_t, class value_t>
void Skip_List<key_t, value_t>::erase(key_t key) {
	head->erase(key);
}

//For Debug Printing Operation
template<class key_t, class value_t>
void Skip_List<key_t, value_t>::print_list() {
	head->print_list();
}
#endif