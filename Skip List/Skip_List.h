
#ifndef __SKIP_LIST__
#define __SKIP_LIST__

#include "Skip_List_Node.h"

template<class key_t, class value_t>
class Skip_List {
private:
	Skip_List_Node<key_t, value_t>* head;
	int max_level;
public:
	Skip_List<key_t, value_t>(int level = 1) : max_level(level) {
		head = new Skip_List_Node<key_t, value_t>();
	}
};

#endif