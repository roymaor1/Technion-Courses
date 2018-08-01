//
// Created by ZOHAR on 28/11/2017.
//

#ifndef UNTITLED3_DOUBLELINKEDLIST_H
#define UNTITLED3_DOUBLELINKEDLIST_H


#include <iostream>

using namespace std;

template <typename A>
class DoubleLinkedList{
public:
    struct Node {
        A key;
        Node *previous;
        Node *next;

        Node(A t, Node *prev, Node *next) : key(t), previous(prev),
                                            next(next) {}


       // ~Node();
    };
//template <typename B>


public:

    Node* head;
    Node* tail;
    DoubleLinkedList()
    {
		this->head=new Node (NULL, NULL, NULL);
		this->tail=this->head;
    }
    DoubleLinkedList(A value)    //tested as working
    {
        this->head=new Node (value, NULL, NULL);
        this->tail=this->head;

    }

    void listInsert(A value)    //tested as working
    {
        if(!this->head->key){
			head = tail = new Node(value,NULL,NULL);
            return;
        }
        Node* newNode = new Node(value,tail,NULL);
        Node* oldTail = tail;
        tail = newNode;
        oldTail->next=tail;
        return;
    }
    ~DoubleLinkedList()
    {
        if(!this->head)
            return;
        Node *tmp = this->head;
        Node *tmp2;
        while (tmp->next)
        {
            tmp2 = tmp;
            tmp = tmp->next;
            delete tmp2;
        }
        delete tmp;
        head = tail = NULL;
    }
    Node* findElement(A ID)
    {
        if(!head->key) //empty list
            return NULL;
        Node *tmp = this->head;
        while (tmp)
        {
            if(*(tmp->key) == *ID)
                return tmp;
            tmp=tmp->next;
        }
        return NULL;
    }

	void printList(){
		if(!head->key)
			return;
		Node* tmp = head;
		while(tmp->next){
			cout << *(tmp->key) << endl;
			tmp = tmp->next;
		}
		cout << *(tmp->key) << endl;
	}

	int getSize(){
		int size=0;
		if(!head->key)
			return size;
		Node* tmp = head;
		while(tmp){
			tmp=tmp->next;
			size++;
		}
		return size;
	}

	static void mergeLists(DoubleLinkedList* list1, DoubleLinkedList* list2, DoubleLinkedList* merged);

};

template <typename A>
void DoubleLinkedList<A>::mergeLists(DoubleLinkedList<A>* list1, DoubleLinkedList<A>* list2, DoubleLinkedList<A>* merged){
	Node* tmp1 = list1->head;
	Node* tmp2 = list2->head;
	while(tmp1 && tmp2){
		if(!tmp1->key || !tmp2->key)
			break;
		if(*(tmp1->key)<*(tmp2->key)){
			merged->listInsert(tmp1->key);
			tmp1=tmp1->next;
		}
		else{
			merged->listInsert(tmp2->key);
			tmp2=tmp2->next;
		}
	}
	while(tmp1){
		if(!tmp1->key)
			break;
		merged->listInsert(tmp1->key);
		tmp1=tmp1->next;
	}
	while(tmp2){
		if(!tmp2->key)
			break;
		merged->listInsert(tmp2->key);
		tmp2=tmp2->next;
	}
	return;
}
#endif //UNTITLED3_DOUBLELINKEDLIST_H









