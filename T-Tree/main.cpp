#include "T_Tree.h"

int main() {
	Key_Value<int, int> temp1(1, 1);
	Key_Value<int, int> temp2(2, 2);
	swap(temp1, temp2);

	cout << temp1.get_value() << "\n";

	return 0;
}