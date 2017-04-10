#ifndef RB_TREE_H
#define RB_TREE_H

#include "Node.h"

#include <cstdio>
#include <cstddef>
#include <iostream>

#define CASE_AUTO -2
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
	Node<Type>* _delete(Node<Type>*, bool*);
	Node<Type>* _search(Node<Type>*, Type);
	Node<Type>* _findMaxNode(Node<Type>*);

	void _coloringAfterInsert(Node<Type>*);
	void _coloringAfterDelete(Node<Type>*, int caseNumber = CASE_AUTO);

	int whatCaseOf(Node<Type>*);

	Node<Type>* _rotateLeft(Node<Type>*);
	Node<Type>* _rotateRight(Node<Type>*);

	bool _isNull(Node<Type>*);
	bool _detectDanger(Node<Type>*); // 이 노드를 지우려는 경우, 문제가 발생할 수 있는가?

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
	if (_isNull(delNode)){
		std::cerr << data << " : Not Exists\n";
		return;
	}
	bool hasProblem = _detectDanger(delNode);
	Node<Type> *newMid = _delete(delNode, &hasProblem);
	printf("삭제 후 새로운 X : %d (parent: %d)\n", newMid->getKey(), newMid->parent->getKey());
	puts("+--------------------------+");
	puts("색상 변경 전"); Print(root);
	if (hasProblem){
		puts("색상 변경 후");
		_coloringAfterDelete(newMid);
		Print(root);
	}
	puts("+--------------------------+");
}

template <class Type>
Node<Type>* RBTree<Type>::_delete(Node<Type> *node, bool *hasProblem){
	if (_isNull(node)) return nil;

	bool hasLeft = ! _isNull(node->left),
		hasRight = ! _isNull(node->right);

	bool isBlack = node->isRed() == false;

	bool isLeft = node->isLeftNode(),
		isRight = node->isRightNode();
	
	if (hasLeft && hasRight){
		Node<Type>* rightMax = _findMaxNode(node->left);

		Node<Type>* newMid = rightMax;
		// 왼쪽 서브트리가 아예 없는 경우
		if (_isNull(rightMax)){
			rightMax = node->right;
		}
		// 왼쪽 서브트리에서 왼쪽 노드만 있는 경우
		else if (rightMax == node->left){
			if (isLeft){
				rightMax->parent = node->parent;
				rightMax->parent->left = rightMax;
			}
			else {
				rightMax->parent = node->parent;
				rightMax->parent->right = rightMax;
			}
			rightMax->right = node->right;

			rightMax->left->parent = rightMax;
			newMid = rightMax->left;
			printf("[%d] is %s\n", rightMax->key, rightMax->isRed() ? "RED" : "BLACK");
			*hasProblem |= rightMax->isRed() == false;
		}
		else {
			// 가장 오른쪽 노드의 왼쪽 서브트리를 부모와 연결해준다. (삭제를 위해)
			rightMax->parent->right = rightMax->left;
			rightMax->left->parent = rightMax->parent;

			// 지우려는 노드의 자리를 대체
			rightMax->left = node->left;
			rightMax->right = node->right;
		}
		rightMax->parent = node->parent;
		rightMax->right->parent = rightMax->left->parent = rightMax;

		if (node == root){
			root = rightMax;
		}
		else {
			if (isLeft)
				rightMax->parent->left = rightMax;
			else
				rightMax->parent->right = rightMax;
		}
		node->swapColor(rightMax);
		delete node;
		return newMid;
	}
	else if (hasLeft && hasRight == false){
		Node<Type>* cur = node;
		cur = cur->left;
		cur->parent = node->parent;
		if (node == root){
			root = cur;
			root->parent = nil;
		}
		else {
			if (isLeft) cur->parent->left = cur;
			else cur->parent->right = cur;
		}

		// 유일한 자식노드가 레드라면 문제되지 않는다.
		*hasProblem &= cur->isRed() == false;
		node->swapColor(cur);
		delete node;

		return cur->right;
	}
	else if (hasLeft == false && hasRight){
		Node<Type>* cur = node;
		cur = cur->right;
		cur->parent = node->parent;
		if (node == root){
			root = cur;
			root->parent = nil;
		}
		else {
			if (isLeft) cur->parent->left = cur;
			else cur->parent->right = cur;
		}

		// 유일한 자식노드가 레드라면 문제되지 않는다.
		*hasProblem &= cur->isRed() == false;
		node->swapColor(cur);
		delete node;

		return cur->left;
	}
	else {
		Node<Type>* p = node->parent, *ret = nil;
		if (node == root) root = nil;
		else if (isLeft){
			p->left = ret;
			p->left->parent = p;
		}
		else {
			p->right = ret;
			p->right->parent = p;
		}
		delete node;

		return ret;
	}
}

template <class Type>
void RBTree<Type>::_coloringAfterDelete(Node<Type> *x, int caseNumber){
	if (caseNumber == CASE_AUTO)
		caseNumber = whatCaseOf(x);

	// 블랙 노드가 아닌 레드 노드가 입력에 들어온다면
	if (x->isRed()){
		// 자기 앞에 블랙이 빠진거므로 자신을 레드로 바꾼다.
		printf("스스로를 블랙으로 바꾸면 해결됩니다. (기준점: %d (parent :%d))\n", x->key, x->parent->key);
		x->setBlackColor();
		return;
	}

	char str[8][5] = { "AUTO", "FAIL", "ROOT", "1-1", "2-1", "2-4", "*-2", "*-3" };
	printf("CASE %s에 대해 검사합니다. (기준점: %d (parent: %d))\n", str[caseNumber - CASE_AUTO], x->key, x->parent->key);

	bool isLeft = x->isLeftNode();

	Node<Type> *p = x->parent, *s = x->getUncle();
	Node<Type> *l = nil, *r = nil;
	if (_isNull(p)) s = nil;
	if (_isNull(s) == false){
		if (isLeft){
			l = s->left;
			r = s->right;
		}
		else {
			l = s->right;
			r = s->left;
		}
	}

	switch (caseNumber){
	case CASE_1_1:
		p->swapColor(s);
		break;
	case CASE_X_2:
		isLeft ? _rotateLeft(p) : _rotateRight(p);
		p->swapColor(s);
		r->setBlackColor();
		break;
	case CASE_X_3:
		isLeft ? _rotateRight(s) : _rotateLeft(s);
		s->swapColor(l);
		x->parent = p;
		_coloringAfterDelete(x, CASE_X_2);
		break;
	case CASE_2_1:
		s->setRedColor();
		_coloringAfterDelete(p);
		break;
	case CASE_2_4:
		isLeft ? _rotateLeft(p) : _rotateRight(p);
		p->swapColor(s);
		// CASE 1-1, 1-2, 1-3 중 하나로..
		_coloringAfterDelete(x);
		break;
	case CASE_ROOT:
		if (_isNull(x->left)){
			x->left->parent = x;
			_coloringAfterDelete(x->left);
		}
		else {
			x->right->parent = x;
			_coloringAfterDelete(x->right);
		}
		break;
	case CASE_FAIL:
		break;
	}
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
	bool isParentRed = false; // p
	bool isUncleRed = false; // s
	bool isCousinCloseRed = false; // x가 왼쪽일 때 기준, l
	bool isCousinFarRed = false; // x가 왼쪽일 때 기준, r

	Node<Type>* p = nodeX->parent;
	if (_isNull(p) == false){
		isParentRed |= p->isRed();
		Node<Type>* s = nodeX->getUncle();
		if (_isNull(s) == false){
			isUncleRed |= s->isRed();

			// 우선, x가 왼쪽일 때 기준으로 상태를 설정
			isCousinCloseRed |= _isNull(s->left) == false && s->left->isRed();
			isCousinFarRed |= _isNull(s->right) == false && s->right->isRed();

			// 좌우 반전인 경우에 대한 처리
			if (s->isLeftNode()){
				std::swap(isCousinCloseRed, isCousinFarRed);
			}
		}
		else {
			isUncleRed = true; // leaf is black
		}
	}
	else {
		return CASE_ROOT;
	}

	if (isParentRed && !isUncleRed){
		// CASE 1:
		// p가 레드(s는 반드시 블랙), <l의 색상, r의 색상>에 따라
		if (isCousinFarRed) return CASE_X_2;
		if (isCousinCloseRed) return CASE_X_3;
		return CASE_1_1;
	}
	else if (!isParentRed){
		// CASE 2:
		// p가 블랙 <s의 색상, l의 색상, r의 색상>에 따라
		if (isUncleRed){
			if (isCousinCloseRed || isCousinFarRed) return CASE_FAIL;
			// s가 레드이면 l과 r은 반드시 블랙이어야 한다.
			return CASE_2_4;
		}
		if (isCousinFarRed) return CASE_X_2;
		if (isCousinCloseRed) return CASE_X_3;
		return CASE_2_1;
	}

	return CASE_FAIL;
}

template <class Type>
bool RBTree<Type>::_detectDanger(Node<Type> *x){
	if (_isNull(x)) return false;

	// 삭제 노드가 레드이면 문제 없다.
	if (x->isRed()) return false;

	// 노드 x가 루트이면 문제가 없음
	Node<Type> *p = x->parent;
	if (x == root || _isNull(p)) return false;

	bool hasLeft = _isNull(x->left),
		hasRight = _isNull(x->right);

	// 삭제 노드가 블랙이더라도, 자식이 없으면 상관 없다.
	if (!hasLeft && !hasRight) return false;

	bool hasBlackChild = false;
	if (hasLeft){
		hasBlackChild |= x->left->isRed() == false;
	}
	if (hasRight){
		hasBlackChild |= x->right->isRed() == false;
	}

	// 블랙인 자식이 하나라도 있다면 문제가 생긴다.
	return hasBlackChild;
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