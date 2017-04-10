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
	void _coloringAfterInsert(Node<Type>*);

	Node<Type>* _rotateLeft(Node<Type>*);
	Node<Type>* _rotateRight(Node<Type>*);

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
		_coloringAfterInsert(newNode);
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
void RBTree<Type>::_coloringAfterInsert(Node<Type> *x){
	if (_isNull(x) || _isNull(x->parent)) return;
	// 루트이거나 블랙 노드면 문제가 없다.
	if (x == root || !x->isRed()) return;
	Node<Type> *p = x->parent;
	// 레드 노드는 레드인 자식을 가질 수 없다. 조정이 필요하다.
	if (p == root || !p->isRed()) return;

	// 삽입된 현재 노드는 레드, 부모 노드도 레드인 상황

	Node<Type> *s = p->getUncle(), *pp = p->parent;
	// CASE 1 (Double Red)
	if (s->isRed()){
		p->setBlackColor();
		s->setBlackColor();
		pp->setRedColor();
		_coloringAfterInsert(pp);
	}
	// CASE 2 (Unbalanced)
	else {
		bool xLeft = x->isLeftNode(),
			 pLeft = p->isLeftNode();

		// CASE 2-1
		// 방향이 엇갈리는 상황 (XOR)
		if (xLeft ^ pLeft){
			if (xLeft)
				_rotateRight(p);
			else 
				_rotateLeft(p);
			_coloringAfterInsert(p);
		}
		// CASE 2-2
		else {
			if (pLeft)
				_rotateRight(pp);
			else
				_rotateLeft(pp);
			p->swapColor(pp);
			_coloringAfterInsert(x);
		}
	}
}

template <class Type>
Node<Type>* RBTree<Type>::_rotateRight(Node<Type> *x){
	if (_isNull(x)) return x;
	Node<Type> *l = x->left, *r = x->right, *p = x->parent;

	bool isLeft = x->isLeftNode();

	x->left = l->right;
	x->left->parent = x;
	l->right = x;
	l->right->parent = l;

	// (부모/자식 관계를 갱신)
	// 돌리려는 중심이 루트였다면 루트를 변경
	if (x == root){
		root = l;
		root->parent = nil;
	}
	else {
		l->parent = p;
		if (isLeft) p->left = l;
		else p->right = l;
	}

	// 새로운 중심을 반환한다.
	return l;
}

template <class Type>
Node<Type>* RBTree<Type>::_rotateLeft(Node<Type> *x){
	if (_isNull(x)) return x;
	Node<Type> *l = x->left, *r = x->right, *p = x->parent;

	bool isLeft = x->isLeftNode();

	x->right = r->left;
	x->right->parent = x;
	r->left = x;
	r->left->parent = r;

	if (x == root){
		root = r;
		root->parent = nil;
	}
	else {
		r->parent = p;
		if (isLeft) p->left = r;
		else p->right = r;
	}

	// 새로운 중심을 반환한다.
	return r;
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