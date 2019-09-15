#ifndef NODE_H
#define NODE_H

#define RED   0
#define BLACK 1

#ifndef NULL
#define NULL 0
#endif

template <class Type>
class RedBlackTree;

template <class Type>
class Node {
	friend class RedBlackTree<Type>;
public:
	Node() : key(), left(NULL), right(NULL), parent(NULL), color(RED) {}
	Node(Type key, Node<Type>* l = NULL, Node<Type>* r = NULL){
		this->key = key;
		this->left = l;
		this->right = r;
		this->parent = NULL;
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
	Node<Type>* getUncle() {
		Node<Type>* p = this->parent;
		if (!p) return nil;
		if (this->isLeftNode()) return p->right;
		else return p->left;
	}

	void setRedColor(){ this->color = RED; }
	void setBlackColor(){ this->color = BLACK; }
	void swapColor(Node<Type>* node){
		int c = this->color;
		this->color = node->color;
		node->color = c;
	}

	bool isRed() const { return this->color == RED; }
	/* bool isBlack() const { return this->color == BLACK; } */
	bool isLeftNode() const {
		if (this->parent) return this->parent->left == this;
		return false;
	}
	bool isRightNode() const {
		if (this->parent) return this->parent->right == this;
		return false;
	}
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
