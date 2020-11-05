#include "Cuckoo_Hashing.h"
#include <iostream>
#include <functional>
#include <unordered_map>

using namespace std;

int main() {
	string cmd;
	Cuckoo_Hashmap<int, int>table;
	while (cin >> cmd) {
		if (cmd == "insert") {
			int a, b;
			cin >> a >> b;
			//table[a] = b;	
			table.insert(a, b);
		}
		else if (cmd == "print") {
			table.print_table();
		}
		else if (cmd == "find") {
			int a;
			cin >> a;
			cout << table.find(a) << endl;
		}
		else if (cmd == "assign") {
			int a, b;
			cin >> a >> b;
			table[a] = b;	
		}
		else if (cmd == "delete") {
			int a;
			cin >> a;
			table.erase(a);
		}
		else if (cmd == "quit") {
			return 0;
		}
	}
}