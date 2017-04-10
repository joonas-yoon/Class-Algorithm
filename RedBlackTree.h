#ifndef RB_TREE_H
#define RB_TREE_H

#include "Node.h"

#include <cstdio>
#include <cstddef>
#include <iostream>

#define CASE_FAIL -1
#define CASE_ROOT  0
#define CASE_1_1   1
#define CASE_2_1   2
#define CASE_2_4   3
#define CASE_X_2   4
#define CASE_X_3   5

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
	Node<Type>* _delete(Node<Type>*);
	Node<Type>* _search(Node<Type>*, Type);
	Node<Type>* _findMaxNode(Node<Type>*);

	void _coloringAfterInsert(Node<Type>*);

	int whatCaseOf(Node<Type>*);

	Node<Type>* _rotateLeft(Node<Type>*);
	Node<Type>* _rotateRight(Node<Type>*);

	bool _isNull(Node<Type>*);

private:
	Node<Type>* root;
	Node<Type>* nil;
};

template <class Type>
void RBTree<Type>::Insert(Type data){
	if (_isNull(_search(root, data)) == false){
		std::cout << data << " : Already Exists\n";
		return;
	}

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
void RBTree<Type>::Delete(Type data){
	Node<Type> *delNode = _search(root, data);
	_delete(delNode);
}

template <class Type>
Node<Type>* RBTree<Type>::_delete(Node<Type> *node){
	if (_isNull(node)) return nil;

	bool hasLeft = ! _isNull(node->left),
		hasRight = ! _isNull(node->right);
	
	if (hasLeft && hasRight){
		Node<Type>* rightMax = _findMaxNode(node->left);
		node->key = rightMax->key;
		node->color = rightMax->color;
		rightMax->parent->left = nil;
		node->left = _delete(node->left);
	}
	else if (hasLeft && hasRight == false){
		Node<Type>* cur = node;
		cur = cur->left;
		cur->parent->left = cur;
		if (node == root) root = cur;
		delete node;
	}
	else if (hasLeft == false && hasRight){
		Node<Type>* cur = node;
		cur = cur->right;
		cur->parent->right = cur;
		if (node == root) root = cur;
		delete node;
	}
	else {
		if (node == root) root = nil;
		delete node;
		node = nil;
	}

	return node;
}

template <class Type>
Node<Type>* RBTree<Type>::_search(Node<Type> *node, Type data){
	if (_isNull(node)) return nil;

	Type key = node->getKey();
	if (key == data) return node;
	else if (key < data) return _search(node->right, data);
	else return _search(node->left, data);
}

template <class Type>
Node<Type>* RBTree<Type>::_findMaxNode(Node<Type> *node) {
	if (_isNull(node->right))
		return node;
	return _findMaxNode(node->right);
}

template <class Type>
int RBTree<Type>::whatCaseOf(Node<Type> *nodeX){
	if (!nodeX) return CASE_FAIL;

	bool isParentRed = false; // p
	bool isUncleRed = false; // s
	bool isCousinLeftRed = false; // l
	bool isCousinRightRed = false; // r

	Node<Type>* p = nodeX->parent;
	if (p){
		isParentRed |= p->isRed();
		Node<Type>* s = nodeX->getUncle();
		if (s){
			isUncleRed |= s->isRed();
			isCousinLeftRed |= s->left && s->left->isRed();
			isCousinRightRed |= s->right && s->right->isRed();
		}
	}
	else {
		nodeX->setRedColor();
		return CASE_ROOT;
	}

	if (isParentRed && !isUncleRed){
		// CASE 1:
		if (isCousinRightRed) return CASE_X_2;
		if (isCousinLeftRed) return CASE_X_3;
		return CASE_1_1;
	}
	else if( !isParentRed ){
		// CASE 2:
		if (isUncleRed){
			if (isCousinLeftRed || isCousinRightRed) return CASE_FAIL;
			return CASE_2_4;
		}
		if (isCousinRightRed) return CASE_X_2;
		if (isCousinLeftRed) return CASE_X_3;
		return CASE_2_1;
	}

	return CASE_FAIL;
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