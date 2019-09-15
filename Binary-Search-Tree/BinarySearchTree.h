#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H
 
#include "Node.h"

#include <iostream>

#define ORDER_PRE  0
#define ORDER_IN   1
#define ORDER_POST 2

#ifndef NULL
#define NULL 0
#endif
 
template <class Type>
class BinarySearchTree {
public:
    BinarySearchTree() : root(NULL) { }
    ~BinarySearchTree(){
        while (root) {
            root = deleteNode(root);
        }
    }
    
    int Insert(Type x){
        int count = 0;
        root = Insert(root, x, count);
        return count;
    }
     
    // 삭제할 원소는 BST에 있다고 가정한다.
    void Delete(Type x){
        root = searchAndDelete(root, x);
    }
    
    void printTree(){
        std::cout << "Tree-Style:\n";
        printTree(root);
    }
    void print(int order){
        char *str[3]={"Pre", "In", "Post"};
        std::cout << str[order] << "-order: ";
        printOrder(root, order);
        std::cout << std::endl;
    }
     
    // find parent of node which has X
    Node<Type>* searchAndDelete(Node<Type>* node, Type x){
        if( ! node ) return node;
         
        if( x == node->key ){
            node = deleteNode(node);
            return node;
        }
         
        if( ! node->left && !node->right ) return node;
         
        if( node->left && node->left->key == x ){
            node->left = deleteNode(node->left);
        }
         
        if( node->right && node->right->key == x ){
            node->right = deleteNode(node->right);
        }
             
        if( x < node->key ){
            node->left = searchAndDelete(node->left, x);
        } else {
            node->right = searchAndDelete(node->right, x);
        }
 
        return node;
    }
     
    Node<Type>* Insert(Node<Type> *node, Type x, int &callCount){
        // for report
        callCount += 1;
         
        if(node == NULL){
            node = new Node<Type>(x, NULL, NULL);
        } else if(x < node->key){
            node->left = Insert(node->left, x, callCount);
        } else {
            node->right = Insert(node->right, x, callCount);
        }
        return node;
    }
     
    Node<Type>* deleteNode(Node<Type>* node){
        if ( ! node ) return NULL;
         
        if (node->right == NULL){
            // have no child
            Node<Type>* left = node->left;
            delete node;
            return left;
        }
        else {
            Node<Type>* child = node->right;
            if( child->left == NULL ){
                node->key = child->key;
                node->right = child->right;
                delete child;
                return node;
            }
 
            Node<Type>* parent = child;
            while (child && child->left){
                parent = child;
                child = child->left;
            }
            node->key = child->key;
             
            // left child have no right child
            if (child == node->right) node->right = child->right;
            else parent->left = child->right;
            delete child;
            return node;
        }
    }
     
    Node<Type>* getRoot(){return root;}
    void printTree(Node<Type>* n, int dep = 0){
        if(n == NULL) return;
 
        for(int i=0; i<dep; ++i) std::cout<<"    ";
        std::cout<< n->key <<'\n';
 
        printTree(n->left, dep+1);
        printTree(n->right, dep+1);
    }
    void printOrder(Node<Type>* n, int printOrder = ORDER_PRE){
        if(n == NULL) return;
         
        if(printOrder == ORDER_PRE) std::cout<< n->key << ' ';
        printOrder(n->left, printOrder);
        if(printOrder == ORDER_IN) std::cout<< n->key << ' ';
        printOrder(n->right, printOrder);
        if(printOrder == ORDER_POST) std::cout<< n->key << ' ';
    }
private:
    Node<Type>* root;
};

#endif