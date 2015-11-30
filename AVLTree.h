#ifndef AVLTREE_H_
#define AVLTREE_H_
#include <algorithm>
#include "Exceptions.h"
namespace AVL{
	template <class T> class Node{
	private:
		template <class U> friend class Tree;
		int key;
		T data;
		int height;
		Node<T>* left;
		Node<T>* right;
		Node();
		void updateHeight(){
			int heightLeft = left ? left->height : -1;
			int heightRight = right ? right->height : -1;
			height = std::max<int>(heightLeft, heightRight) + 1;
		}
		Node<T>* removeMin(){
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
		Node<T>* rotateRight(){
			Node<T>* newRoot = left;
			left = newRoot->right;
			newRoot->right = this;
			updateHeight();
			newRoot->updateHeight();
			return newRoot;
		}
		Node<T>* rotateLeft(){
			Node<T>* newRoot = right;
			right = newRoot->left;
			newRoot->left = this;
			updateHeight();
			newRoot->updateHeight();
			return newRoot;
		}
		Node<T>* balance(){
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
		Node<T>* findMin(){
			return left ? left->findMin() : this;
		}
		Node<T>* findMax(){
			return right ? right->findMax() : this;
		}
		void killNode(){
			if (left)
				left->killNode();
			if (right)
				right->killNode();
			delete this;
		}
		Node<T>* copyNode(){
			Node<T>* newNode = new Node<T>(*this);
			newNode->key = key;
			newNode->data = data;
			newNode->height = height;
			if (left){
				newNode->left = left.copyNode();
			}
			if (right){
				newNode->right = right.copyNode();
			}
			return newNode;
		}
	public:
		Node(int key, const T& data) :key(key), data(data), left(0), right(0), height(0){};
		Node(const Node<T>& copyN) :key(copyN.key), data(copyN.data), height(copyN.height){
			right = copyN.right ? copyN.right->copyNode() : 0;
			left = copyN.left ? copyN.left->copyNode() : 0;
		};
		Node<T>& operator=(const Node<T>& copyN){
			if (this != &copyN){
				killNode();
				key = copyN.key;
				data = copyN.data;
				left = copyN.left->copyNode();
				right = copyN.right->copyNode();
				height = copyN.height;
			}
			return *this;
		}
		~Node<T>(){};
		int getHeight()const{
			return height;
		}
		int getKey()const{
			return key;
		}
		T& getData()const{
			return data;
		}
		Node* insert(int k, const T& data) {
			if (k < key)
				if (!left)
					left = new Node(k, data);
				else
					left = left->insert(k, data);
			else if (k > key)
				if (!right)
					right = new Node(k, data);
				else
					right = right->insert(k, data);
			return balance();
		}
		Node* remove(int k){
			if (k < key && left){
				left = left->remove(k);
				return balance();
			}
			else if (k > key && right){
				right = right->remove(k);
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
			else
				throw ElementNotFound();
		}
		Node* find(int k){
			if (k == key)
				return this;
			if (k > key && right)
				return right->find(k);
			if (k < key && left)
				return left->find(k);
			return nullptr;
		}
		template <typename actionT> void inorderScan(actionT action)const{
			if (left)
				left->inorderScan(action);
			action(data);
			if (right)
				right->inorderScan(action);
		}
		template <typename actionT> void reverseInorderScan(actionT action)const{
			if (right)
				right->reverseInorderScan(action);
			action(data);
			if (left)
				left->reverseInorderScan(action);
		}
	};
	template <class U> class Tree{
	private:
		int size;
	public:
		Node<U>* root;
		Tree() : root(0),size(0){};
		Tree(int key, const U& data):size(1){
			root = new Node<U>(key, data);
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
		void insert(int key, const U& data){
			root = root ? root->insert(key, data) : new Node<U>(key,data);
			++size;
		}
		void remove(int key){
			root = root ? root->remove(key) : 0;
			--size;
		}
		Node<U>* find(int key){
			return root ? root->find(key) : 0;
		}
		Node<U>* findMin(){
			return root ? root->findMin() : 0;
		}
		Node<U>* findMax(){
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