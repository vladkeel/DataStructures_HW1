#ifndef AVLTREE_H_
#define AVLTREE_H_
#include <algorithm>
#include <stdlib.h>
#include "Exceptions.h"
#include <math.h>
namespace AVL{
	template <class S, class T> class Node;
	template <class N, class M>
	// Create new empty balanced tree.
	// each subtree is half the size of the tree -1
	// <param name"size"> size of tree </param>
	// <return> root of tree </return>
	Node<N, M>* buildEmpty(int size){
		if (size == 0){
			return NULL;
		}
		else if (size == 1){
			return new Node<N, M>();
		}
		double tempCalcLeft = double(size - 1) / 2;
		double tempCalcRight = double(size - 1) / 2;
		int leftSize = (int)ceil(tempCalcLeft);
		int rightSize = (int)floor(tempCalcRight);
		Node<N, M>* newNode = new Node<N, M>();
		newNode->left = buildEmpty<N, M>(leftSize);
		newNode->right = buildEmpty<N, M>(rightSize);
		newNode->updateHeight();
		return newNode;
	}

	// AVL tree node class.
	template <class S, class T> class Node{
	private:
		template <class F, class U> friend class Tree;
		template <class N, class M> friend Node<N,M>* buildEmpty(int size);
		// Node key
		S key;

		// Node data
		T data;

		// Node height in tree
		int height;

		// left son node
		Node<S, T>* left;

		// right son node
		Node<S, T>* right;

		// update height of the node according to subtrees
		// <return/>
		void updateHeight(){
			int heightLeft = left ? left->height : -1;
			int heightRight = right ? right->height : -1;
			height = std::max<int>(heightLeft, heightRight) + 1;
		}

		// remove minimal node (to the left) of subtree to
		// determin new root of subtree
		// <return> balanced root of subtree </return>
		Node<S, T>* newRoot(){
			if (!left)
				return right;
			left = left->newRoot();
			return balance();
		}

		// determin balance factor of node for AVL tree balancing
		// <return> balance factor </return>
		int balanceFactor(){
			int heightLeft = left ? left->height : -1;
			int heightRight = right ? right->height : -1;
			return heightLeft - heightRight;
		}

		// rotate tree right to balance AVL tree
		// <return> new root of subtree </return>
		Node<S, T>* rotateRight(){
			Node<S, T>* newRoot = left;
			left = newRoot->right;
			newRoot->right = this;
			updateHeight();
			newRoot->updateHeight();
			return newRoot;
		}

		// rotate tree left to balance AVL tree
		// <return> new root of subtree </return>
		Node<S, T>* rotateLeft(){
			Node<S, T>* newRoot = right;
			right = newRoot->left;
			newRoot->left = this;
			updateHeight();
			newRoot->updateHeight();
			return newRoot;
		}

		// balance tree (if needs balancing) by RR,LL,RL or LR rotation
		// <return> new root of subtree </return>
		Node<S, T>* balance(){
			updateHeight();
			if (balanceFactor() == 2)
			{
				if (left && left->balanceFactor() < 0)
					left = left->rotateLeft();
				return rotateRight();
			}
			if (balanceFactor() == -2)
			{
				if (right && right->balanceFactor() > 0)
					right = right->rotateRight();
				return rotateLeft();
			}
			return this;
		}

	protected:
		// copy whole subtree to new subtree, since copy c'tor
		// only copies key and data
		// <return> copy of subtree </return>
		Node<S, T>* copyNode(){
			Node<S, T>* newNode = new Node<S, T>(*this);
			newNode->key = key;
			newNode->data = data;
			newNode->height = height;
			if (left){
				newNode->left = left->copyNode();
			}
			if (right){
				newNode->right = right->copyNode();
			}
			return newNode;
		}

	public:
		// default c'tor
		Node() :  key(), data(), height(0),left(0), right(0){};

		// data c'tor
		// <param name="key"> key of new node </param>
		// <param name="data"> data of new node </param>
		Node(const S& key, const T& data) :key(key), data(data),height(0),
			left(0), right(0) {};
		
		// copy c'tor
		Node(const Node<S, T>& copyN) :key(copyN.key), data(copyN.data),
			height(0), left(0), right(0){};
		
		// recursive d'tor
		~Node<S, T>(){
			if (left)
				delete left;
			if (right)
				delete right;
		};

		// assignment operator
		// <param name="copyN"> node to copy from </param>
		// <return> referenced node with new data and key </return>
		Node<S, T>& operator=(const Node<S, T>& copyN){
			if (this != &copyN){
				if (left)
					delete left;
				if (right)
					delete right;
				key = copyN.key;
				data = copyN.data;
				left = 0;
				right = 0;
				height = 0;
			}
			return *this;
		}

		// get height of node
		inline int getHeight()const{
			return height;
		}

		// get minimal key node in subtree
		Node<S, T>* findMin(){
			return left ? left->findMin() : this;
		}

		//get maximal key node in subtree
		Node<S, T>* findMax(){
			return right ? right->findMax() : this;
		}

		// get node key
		inline const S& getKey()const{
			return key;
		}

		//get node data
		inline T& getData(){
			return data;
		}

		//set new key
		inline void setKey(const S& other){
			key = other;
		}

		//set new data
		inline void setData(const T& other){
			data = other;
		}

		// insert new node to subtree. inserts the node in
		// to correct position according to key, then balance tree.
		// <param name="k"> node key </param>
		// <param name="data"> node data </param>
		// <return> new root of subtree </return>
		Node* insert(S& k, const T& data){
			if (k < key){
				if (!left)
					left = new Node(k, data);
				else
					left = left->insert(k, data);
			}
			else if (k == key)
				throw AlreadyExists();
			else{
				if (!right)
					right = new Node(k, data);
				else
					right = right->insert(k, data);
			}
			return balance();
		}

		// find node with given key in subtree and remove it.
		// then balance tree.
		// <param name="k"> node key </param>
		// <return> new root of subtree </return>
		Node* remove(S& k){
			if (k < key && left){
				left = left->remove(k);
				return balance();
			}
			else if (k == key){
				Node* leftTemp = left;
				Node* rightTemp = right;
				left = 0;
				right = 0;
				delete this;
				if (!rightTemp) return leftTemp;
				Node* min = rightTemp->findMin();
				min->right = rightTemp->newRoot();
				min->left = leftTemp;
				return min->balance();
			}
			else if (right){
				right = right->remove(k);
				return balance();
			}
			else
				throw ElementNotFound();
		}

		// find and return node with given key.
		// <param name="k"> node key </param>
		// <return> the node, or NULL if none </return>
		Node* find(S& k){
			if (k == key)
				return this;
			if (k > key && right)
				return right->find(k);
			if (k < key && left)
				return left->find(k);
			return NULL;
		}

		// perform in-order scan of subtree. perform action with iterator
		// on each node in subtree.
		// <param name="action"> function or class with operator() to perform </param>
		// <param name="iterator"> auxiliary parameter for actions thar=t require an
		// iterator of nodes </param>
		// <return/>
		template <typename actionT> void inorderScan(actionT action, int* iterator){
			if (left){
				left->inorderScan(action, iterator);
			}
			action(*this, iterator);
			++(*iterator);
			if (right)
				right->inorderScan(action, iterator);
		}
	};

	// AVL tree container class
	template <class F,class U> class Tree{
	private:
		// size of tree
		int size;
	public:
		// root of tree
		Node<F,U>* root;

		// Default c'tor
		Tree() : size(0), root(0){};

		// Empty tree c'tor
		// <param name="size"> size of empty tree </param>
		Tree(int size):size(size){
			root = buildEmpty<F,U>(size);
		}

		// copy c'tor
		Tree(const Tree& tree){
			root = tree.root? tree.root->copyNode() : 0;
			size = tree.size;
		}


		// d'tor - calls node d'tor
		~Tree(){
			if (root)
				delete root;
		}

		// assignment operator
		Tree& operator=(const Tree& tree){
			if (this != &tree){
				if (root)
					delete root;
				root = tree.root ? tree.root->copyNode() : 0;
				size = tree.size;
			}
			return *this;
		};

		// get size of tree
		inline int getSize(){
			return size;
		}

		// insert new node into tree
		// <param name="key"> key of new node </param>
		// <param name="data"> data of new node </param>
		// </return>
		void insert(F& key, const U& data){
			root = root ? root->insert(key, data) : new Node<F,U>(key,data);
			++size;
		}

		// remove node from tree
		// <param name="key"> key of node to remove </param>
		// <return/>
		void remove(F& key){
			root = root ? root->remove(key) : 0;
			--size;
		}

		// find node in tree
		// <param name="key"> key of node to find </param>
		// <return> pointer to the node or NULL if non </return>
		Node<F,U>* find(F& key){
			return root ? root->find(key) : 0;
		}

		// find minimal key node in tree
		// <return> pointer to minimal key or NULL if no tree </return>
		Node<F,U>* findMin(){
			return root ? root->findMin() : 0;
		}

		// find maximal key node in tree
		// <return> pointer to max key node or NULL if no tree </return>
		Node<F,U>* findMax(){
			return root ? root->findMax() : 0;
		}

		// perform inorder scan of tree, activating action on each node
		// <param name="action"> function or class with operator() to activate </param>
		// <param name="iterator"> auxiliary value for actions that need node iterator </param>
		template <typename actionT> void inorderScan(actionT action, int* iterator)const{
			if (root)
				root->inorderScan(action, iterator);
		}
	};

}
#endif /* AVLTREE_H_ */
