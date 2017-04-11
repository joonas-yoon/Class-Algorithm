#include "BinarySearchTree.h"
#include <cstdio>
#include <iostream>
using namespace std;

int main(){
    BinarySearchTree<int> tree;
    int n, k;
    scanf("%d", &n);
    
    while(n--){
        scanf("%d", &k);
        tree.Insert(k);
    }
    
    scanf("%d", &n);
    while(n--){
        int order, delItem;
        scanf("%d %d", &order, &delItem);
        
        tree.Delete(delItem);
        tree.printTree();
        tree.print(order);
        puts("+----------------+");
    }
    return 0;
}