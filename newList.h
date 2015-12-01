
#ifndef NEWLIST_H_
#define NEWLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include "Exceptions.h"

using namespace std;

namespace LinkedList{

	template <class S>
	class Node{
			S data;
			Node* next;
	public:
			Node(const S& newData) :  data(newData), next(NULL) {};
			S& getData(){
				return data;
			}
			Node* getNext(){
				return next;
			}
			void setNext(Node* newNext){
				next = newNext;
			}
	};



	template <class T>
	class List{

	Node<T>* head;
	void clear(){
		for(Node<T>* p = head; p!=NULL;){
			Node<T>* r = p;
			p=p->getNext();
			delete r;
		}
		head=NULL;
	}
	Node<T>* find(const T& data){
		Node<T>* p = head;
		for(; p; p = p->getNext()){
			if(p->getData() == data)
				return p;
		}
		return NULL;
	}

	public:
	List() : head(NULL){};
	List(const List& copyList) : head(NULL) {
		for(Node<T>* p = copyList.head; p!=NULL; p=p->getNext()){
			this->insert(p->getData());
		}
	}
	List& operator=(const List& copyList){
		if(this == &copyList){
			return *this;
		}
		this->clear();
		for(Node<T>* p = copyList.head; p!=NULL; p=p->getNext()){
			this->insert(p->getData());
		}
		return *this;
	}
	~List(){
		clear();
	}
	void insert(const T& data){
		if(!head){
			head = new Node<T>(data);
			return;
		}
		Node<T>* newNode = new Node<T>(data);
		newNode->setNext(head);
		head = newNode;
		return;
	}
	void remove(const T& data){
		if(!head)
			throw IsEmptyException();
		if(head->getData() == data){
			Node<T>* r = head;
			head = head->getNext() ? head->getNext() : NULL;
			delete r;
			return;
		}
		for(Node<T>* p = head; p!=NULL; p=p->getNext()){
			if(!p->getNext())
				throw ElementNotFound();
			if( (p->getNext())->getData() == data){
				Node<T>* r = p->getNext();
				Node<T>* temp = r->getNext();
				p->setNext(temp);
				delete r;
				return;
			}

		}
	}

	Node<T>* begin(){
		return head;
	}
	template<class Predicate>
		T& find(const Predicate& predicate) {
				for (Node<T>* p = head; p!=NULL; p=p->getNext()) {
					if (predicate(p->getData())) {
						return p->getData();
					}
				}
				throw ElementNotFound();
			}

		bool isIn(const T& data){
			return find(data) ? true : false;
		}
	};
}

#endif /* NEWLIST_H_ */
