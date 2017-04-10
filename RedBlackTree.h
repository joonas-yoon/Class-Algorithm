#ifndef RB_TREE_H
#define RB_TREE_H

#include "Node.h"

#include <cstdio>
#include <cstddef>
#include <iostream>

template <class Type>
class RBTree {
public:
	RBTree() {
		nil = new Node<Type>(-1, nil, nil);
		nil->setBlackColor();
		root = nil;
	}
	~RBTree(){ root = nil = NULL; }

	void Insert(Type);
	void Delete(Type);
	void Print(Node<Type>* n, int d = 0);

	Node<Type>* getRoot() const { return this->root; }

private:
	Node<Type>* _insert(Node<Type>*, Node<Type>*);

	bool _isNull(Node<Type>*);

private:
	Node<Type>* root;
	Node<Type>* nil;
};

template <class Type>
void RBTree<Type>::Insert(Type data){
	Node<Type> *newNode = new Node<Type>(data, nil, nil);
	if (!root){
		root = newNode;
	}
	else {
		root = _insert(root, newNode);
		// _coloring(newNode);
	}
	root->setBlackColor();
	root->parent = nil;
}

template <class Type>
void RBTree<Type>::Delete(Type data){
	return;
}

template <class Type>
Node<Type>* RBTree<Type>::_insert(Node<Type> *node, Node<Type> *newItem){
	if (_isNull(node)){
		node = newItem;
	}
	else if (newItem->getKey() > node->getKey()){
		Node<Type>* child = _insert(node->right, newItem);
		node->right = child;
		child->parent = node;
	}
	else {
		Node<Type>* child = _insert(node->left, newItem);
		node->left = child;
		child->parent = node;
	}
	return node;
}

template <class Type>
void RBTree<Type>::Print(Node<Type> *n, int dep){
	if (_isNull(n)) return;
	for (int i = 0; i < dep; ++i){
		printf(" %c ", i == dep-1 ? '|' : ' ');
	}
	std::cout<< n->key << (n->isRed() ? "[R]":"[B]") <<'\n';
	Print(n->left, dep + 1);
	Print(n->right, dep + 1);
}

template <class Type>
bool RBTree<Type>::_isNull(Node<Type> *x){
	return x == NULL || x == nil;
}

#endif