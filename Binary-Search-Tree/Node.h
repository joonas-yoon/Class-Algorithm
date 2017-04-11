#pragma once
 
#define NULL 0

template <class Type>
class BinarySearchTree;
 
template <class Type>
class Node {
    friend class BinarySearchTree<Type>;
public:
    Node() : left(NULL), right(NULL) {}
    Node(Type key, Node<Type>* l = NULL, Node<Type>* r = NULL){
        this->key = key;
        this->left = l;
        this->right = r;
    }
    Node(Node<Type>& node){
        key = node.key;
        left = node.left;
        right = node.right;
    }
    ~Node(){
        left = right = NULL;
    }
 
private:
    Type key;
    Node<Type>* left;
    Node<Type>* right;
};