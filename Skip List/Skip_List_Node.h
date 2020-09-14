#ifndef __SKIP_LIST_NODE__
#define __SKIP_LIST_NODE__

#include <random>

template<class key_t, class value_t>
class Skip_List_Node {
private:
	key_t key;
	value_t value;
	int level;

	Skip_List_Node<key_t, value_t>** prev, ** next;
public:
	Skip_List_Node<key_t, value_t>(int level) : level(level) {
		prev = new Skip_List_Node<key_t, value_t> * [level];
		next = new Skip_List_Node<key_t, value_t> * [level];

		for (int i = 0; i < level; i++) {
			prev[i] = next[i] = NULL;
		}
	}
	Skip_List_Node<key_t, value_t>(int level, key_t key, value_t value) : Skip_List_Node(level), (key), value(value) {}


};

#endif