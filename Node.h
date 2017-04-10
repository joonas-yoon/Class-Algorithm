#ifndef NODE_H
#define NODE_H

#define RED   0
#define BLACK 1

#include <cstddef> // NULL

template <class Type>
class RBTree;

template <class Type>
class Node {
	friend class RBTree<Type>;
public:
	Node() : left(NULL), right(NULL), parent(NULL), color(RED) {}
	Node(Type key, Node<Type>* l = NULL, Node<Type>* r = NULL){
		this->key = key;
		this->left = l;
		this->right = r;
		this->color = RED;
	}
	Node(Node<Type>& node){
		key = node.key;
		parent = node.parent;
		left = node.left;
		right = node.right;
		color = node.color;
	}
	~Node(){
		left = right = parent = NULL;
	}

	Type getKey() const { return this->key; }

	void setRedColor(){ this->color = RED; }
	void setBlackColor(){ this->color = BLACK; }

	bool isRed() const { return this->color == RED; }
	/* bool isBlack() const { return this->color == BLACK; } */
	bool isLeaf() const {
		return !this->left || !this->right;
	}
private:
	Type key;
	Node<Type>* left;
	Node<Type>* right;
	Node<Type>* parent;
	int color;
};

#endif