#pragma once
 
#include "Node.h"
#include <cstdlib>
#include <cstddef>
#include <iostream>

#define ORDER_PRE  0
#define ORDER_IN   0
#define ORDER_POST 0

#define NULL 0
 
template <class Type>
class BinarySearchTree {
public:
    BinarySearchTree() : root(NULL) { }
    ~BinarySearchTree(){ root = NULL; }
    
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
        puts("Tree-Style:");
        print_tree(root);
    }
    void print(int order){
        char *str[3]={"Pre", "In", "Post"};
        printf("%s-order: ", str[order]);
        print_order(root, order); puts("");
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
            return node->left;
        }
        else {
            Node<Type>* child = node->right;
            if( child->left == NULL ){
                node->key = child->key;
                node->right = child->right;
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
            return node;
        }
    }
     
    Node<Type>* getRoot(){return root;}
    void print_tree(Node<Type>* n, int dep = 0){
        if(n == NULL) return;
 
        for(int i=0; i<dep; ++i) std::cout<<"    ";
        std::cout<< n->key <<'\n';
 
        print_tree(n->left, dep+1);
        print_tree(n->right, dep+1);
    }
    void print_order(Node<Type>* n, int printOrder = ORDER_PRE){
        if(n == NULL) return;
         
        if(printOrder == ORDER_PRE) std::cout<< n->key << ' ';
        print_order(n->left, printOrder);
        if(printOrder == ORDER_IN) std::cout<< n->key << ' ';
        print_order(n->right, printOrder);
        if(printOrder == ORDER_POST) std::cout<< n->key << ' ';
    }
private:
    Node<Type>* root;
};
