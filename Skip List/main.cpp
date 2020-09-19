#include "Skip_List.h"
#include <iostream>
using namespace std;

int main() {
	int level;
	cin >> level;
	Skip_List<int, string>list(level);
	string cmd;
	while (cin >> cmd) {
		if (cmd == "insert") {
			int key;
			string val;
			cin >> key >> val;
			list.insert(key, val);
		}
		else if (cmd == "print") {
			list.print_list();
			cout << "\n";
		}
	}
}