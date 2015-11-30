#ifndef AVLTREE_H_
#define AVLTREE_H_
#include <algorithm>
#include <stdlib.h>
#include "Exceptions.h"
#include <math.h>
namespace AVL{
	template <class S, class T> class Node;
	template <class N, class M>
	Node<N,M>* buildEmpty(int size){
		if (size == 0){
			return nullptr;
		}
		else if (size == 1){
			return new Node<N, M>();
		}
		Node<N, M>* newNode = new Node<N, M>();
		newNode->left = buildEmpty<N,M>((int)std::ceil((size-1)/2));
		newNode->right = buildEmpty<N,M>((int)std::floor((size - 1) / 2));
		newNode->updateHeight();
		return newNode;
	}
	template <class S, class T> class Node{
	private:
		template <class F, class U> friend class Tree;
		template <class N, class M> friend Node<N,M>* buildEmpty(int size);
		S key;
		T data;
		int height;
		Node<S, T>* left;
		Node<S, T>* right;
		void updateHeight(){
			int heightLeft = left ? left->height : -1;
			int heightRight = right ? right->height : -1;
			height = std::max<int>(heightLeft, heightRight) + 1;
		}
		Node<S, T>* removeMin(){
			if (!left)
				return right;
			left = removeMin();
			return balance();
		}
		int balanceFactor(){
			int heightLeft = left ? left->height : -1;
			int heightRight = right ? right->height : -1;
			return heightLeft - heightRight;
		}
		Node<S, T>* rotateRight(){
			Node<S, T>* newRoot = left;
			left = newRoot->right;
			newRoot->right = this;
			updateHeight();
			newRoot->updateHeight();
			return newRoot;
		}
		Node<S, T>* rotateLeft(){
			Node<S, T>* newRoot = right;
			right = newRoot->left;
			newRoot->left = this;
			updateHeight();
			newRoot->updateHeight();
			return newRoot;
		}
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
			return this; // no balance needed
		}
	protected:
		void killNode(){
			if (left)
				left->killNode();
			if (right)
				right->killNode();
			delete this;
		}
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
		Node() :left(0), right(0), height(0), key(), data(){};
		Node(const S& key, const T& data) :key(key), data(data), left(0), right(0), height(0){};
		Node(const Node<S, T>& copyN) :key(copyN.key), data(copyN.data), height(0), left(0), right(0){};
		Node<S, T>& operator=(const Node<S, T>& copyN){
			if (this != &copyN){
				killNode();
				key = copyN.key;
				data = copyN.data;
				left = 0;
				right = 0;
				height = 0;
			}
			return *this;
		}
		~Node<S, T>(){};
		int getHeight()const{
			return height;
		}
		Node<S, T>* findMin(){
			return left ? left->findMin() : this;
		}
		Node<S, T>* findMax(){
			return right ? right->findMax() : this;
		}
		const S& getKey()const{
			return key;
		}
		T& getData(){
			return data;
		}
		void setKey(const S& other){
			key = other;
		}
		void setData(const T& other){
			data = other;
		}
		Node* insert(S& k, const T& data) {
			if (k < key){
				if (!left)
					left = new Node(k, data);
				else
					left = left->insert(k, data);
			} else if (k == key)
				throw AlreadyExists();
			else{
				if (!right)
					right = new Node(k, data);
				else
					right = right->insert(k, data);
			}
			return balance();
		}
		Node* remove(S& k){
			if (k < key && left){
				left = left->remove(k);
				return balance();
			}
			else if (k == key){
				Node* leftTemp = left;
				Node* rightTemp = right;
				delete this;
				if (!rightTemp) return leftTemp;
				Node* min = rightTemp->findMin();
				min->right = rightTemp->removeMin();
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
		Node* find(S& k){
			if (k == key)
				return this;
			if (k > key && right)
				return right->find(k);
			if (k < key && left)
				return left->find(k);
			return nullptr;
		}
		template <typename actionT> void inorderScan(actionT action){
			if (left)
				left->inorderScan(action);
			action(*this);
			if (right)
				right->inorderScan(action);
		}
		template <typename actionT> void reverseInorderScan(actionT action){
			if (right)
				right->reverseInorderScan(action);
			action(*this);
			if (left)
				left->reverseInorderScan(action);
		}
	};
	template <class F,class U> class Tree{
	private:
		int size;
	public:
		Node<F,U>* root;
		Tree() : root(0),size(0){};
		Tree(int size):size(size){
			root = buildEmpty<F,U>(size);
		}
		Tree(F& key, const U& data):size(1){
			root = new Node<F,U>(key, data);
		}
		Tree(const Tree& tree){
			root = tree.root? tree.root->copyNode() : 0;
			size = tree.size;
		}
		Tree& operator=(const Tree& tree){
			if (this != &tree){
				if (root)
					root->killNode();
				root = tree.root? tree.root->copyNode() : 0;
				size = tree.size;
			}
			return *this;
		};
		~Tree(){
			if (root)
				root->killNode();
		}
		int getSize(){
			return size;
		}
		void insert(F& key, const U& data){
			root = root ? root->insert(key, data) : new Node<F,U>(key,data);
			++size;
		}
		void remove(F& key){
			root = root ? root->remove(key) : 0;
			--size;
		}
		Node<F,U>* find(F& key){
			return root ? root->find(key) : 0;
		}
		Node<F,U>* findMin(){
			return root ? root->findMin() : 0;
		}
		Node<F,U>* findMax(){
			return root ? root->findMax() : 0;
		}
		template <typename actionT> void inorderScan(actionT action)const{
			if (root)
				root->inorderScan(action);
		}
		template <typename actionT> void reverseInorderScan(actionT action)const{
			if (root)
				root->reverseInorderScan(action);
		}
	};

}
#endif /* AVLTREE_H_ */