#include <cstdio>
#include <iostream>
#include "RedBlackTree.h"
using namespace std;

int main(){
	char cmd;
	int number;
	RedBlackTree<int> rbtree;
	while (cin >> cmd >> number){
		if (cmd == 'x') break;

		if (cmd == 'i'){
			cout << "Insert " << number << '\n';
			rbtree.Insert(number);
		}
		else {
			cout << "Delete " << number << '\n';
			rbtree.Delete(number);
		}

		// print for debug
		rbtree.Print();
	}
	return 0;
}