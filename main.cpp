#include <cstdio>
#include <iostream>
#include "RedBlackTree.h"
using namespace std;

int main(){
	int x;
	RBTree<int> rbtree;
	while (cin >> x){
		cout << "Inserted " << x << '\n';
		rbtree.Insert(x);
		rbtree.Print(rbtree.getRoot());
	}
	return 0;
}