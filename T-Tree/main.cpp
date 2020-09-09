#include "T_Tree.h"
#include <iostream>

using namespace std;

int main() {
	T_Tree_Node<int, int>tree;

	while (true) {
		string cmd;
		cin >> cmd;
		if (cmd == "insert") {
			int key, val;
			cin >> key >> val;
			tree.insert(key, val);
		}
		else if (cmd == "print") {
			tree.print_inorder();
			cout << "\n";
		}

	}

	return 0;
}