#include <cstdio>
#include <cstdlib>
#include <cstddef>

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

template <class Type>
class BinarySearchTree {
public:
	BinarySearchTree() : root(NULL) { }
	~BinarySearchTree(){ root = NULL; }
	
	int insert(Type x){
	    int count = 0;
	    root = treeInsert(root, x, count);
	    return count;
	}
	
	Node<Type>* treeInsert(Node<Type> *node, Type x, int &callCount){
	    // for report
	    callCount += 1;
	    
	    if(node == NULL){
	        node = new Node<Type>(x, NULL, NULL);
	    } else if(x < node->key){
	        node->left = treeInsert(node->left, x, callCount);
	    } else {
	        node->right = treeInsert(node->right, x, callCount);
	    }
	    return node;
	}
	
	Node<Type>* getRoot(){return root;}
	void print(Node<Type>* n, int dep = 0, char c = 'L'){
	    if(n == NULL) return;
	    for(int i=0; i<dep; ++i) printf("  ");
	    printf("[%c] %d\n", c, n->key);
	    
	    print(n->left, dep+1, 'L');
	    print(n->right, dep+1, 'L');
	}
private:
    Node<Type>* root;
};

int main(){
    BinarySearchTree<int> tree;
    int n;
    scanf("%d", &n);
    
    int k, sum = 0;
    while(n--){
        scanf("%d", &k);
        int m = tree.insert(k);
        // printf("%d: %d\n", k, m);
        // tree.print(tree.getRoot());
        sum += m;
    }
    printf("%d", sum);
    return 0;
}