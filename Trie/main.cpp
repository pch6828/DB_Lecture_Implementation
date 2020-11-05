#include "T_Tree.h"
#include <iostream>

using namespace std;

int main() {
	T_Tree<int, string>tree;
	string cmd;

	while (cin >> cmd) {
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
		else if (cmd == "delete") {
			int key;
			cin >> key;
			tree.erase(key);
		}
		else if (cmd == "quit") {
			return 0;
		}
	}
}