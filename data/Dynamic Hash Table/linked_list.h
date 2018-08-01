#ifndef LINKEDLIST
#include <cstdio>
#include <cstdlib>

template<typename Data>
class LinkedList {

	class Node {
	public:
		Data* data;
		Node* next;
		Node(Data*& t, Node* next = NULL) :
			data(t), next(next) {}
	};


public:
	class iterator {
	public:
		class iterationError : public std::exception {};
		Node* current;
		iterator(Node* node) {
			if (!node) current = NULL;
			current = node;
		}

		Data*& operator*() {
			if (!current) {
				throw iterationError();
			}
			return current->data;
		}

		Node*& GetNode() {
			return current;
		}

		bool operator==(const iterator& it) const {
			return (current == it.current);
		}
		bool operator!=(const iterator& it) const {
			return (!(*this == it));
		}
		iterator& operator++() {
			if (!this->current->next) current = NULL;
			else current = current->next;
			return *this;
		}

		iterator operator++(int n) {
			iterator old = *this;
			++(*this);
			return old;
		}
	};

	class LinkedListError : public std::exception {}; 
	
	Node* head;
	
	LinkedList() : head(NULL) {}
	
	iterator Begin() {
		return iterator(this->head);
	}

	iterator End() {
		return iterator(NULL);
	}

	bool IsEmpty() {
		if (head == NULL) return true;
		return false;
	}

	void Insert(Data* d) {
		try {
			if (!head) {
				head = new Node(d);
				return;
			}
			Node* node = new Node(d, head);
			head = node;
		}
		catch (std::bad_alloc&) {
			throw LinkedListError();
		}
	}
	void Remove(Data* d) {
		if (head->data == d) {
			Node* rem = head;
			head = head->next;
			delete rem;
			return;
		}
		bool found = false;
		Node*& prev = Begin().GetNode();
		for (LinkedList::iterator it = Begin(); (it != End()) && (!found); it++) {
			if (*it != d) {
				prev = it.GetNode();
				continue;
			}
			prev->next = it.GetNode()->next;
			delete it.GetNode();
			found = true;
		}
		if (!found) throw LinkedListError();
	}

	Data* Find(int studentID) {
		for (LinkedList::iterator it = Begin(); it != End(); it++) {
			if ((*it)->GetID() == studentID) {
				return (*it);
			}
		}
		return NULL;
	}

};
#endif // LINKEDLIST