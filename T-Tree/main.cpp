#include "T_Tree.h"
#include <iostream>

using namespace std;

int main() {
	T_Tree<int, string>tree;

	while (true) {
		string cmd;
		cin >> cmd;
		if (cmd == "insert") {
			int key;
			string val;
			cin >> key >> val;
			tree.insert(key, val);
		}
		else if (cmd == "print") {
			tree.print_inorder();
			cout << "\n";
		}
		else if (cmd == "find") {
			int key;
			cin >> key;
			string* result = tree.find(key);
			if (result) {
				cout << *result << "\n";
			}
			else {
				cout << "No Data\n";
			}
		}

	}

	return 0;
}