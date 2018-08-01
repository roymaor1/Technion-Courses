//
// Created by Roy on 02/12/2017.
//

#ifndef INC_021217_SPLAY_H
#define INC_021217_SPLAY_H

#include "library1.h"
#include <iostream>


using namespace std;

template <typename KeyType> class Splay {
public:
	class Node {
	public:
		KeyType key;
		KeyType data;
		Node *left;
		Node *right;

		Node(KeyType k = NULL, KeyType d = NULL, Node *l = NULL, Node *r = NULL)
				:
				key(k), data(d), left(l), right(r) {}

		void printTreeSub() {
			if (left && right) {
				cout << "\n" << *key << " | left: " << *(left->key) <<
					 " | right: " << *(right->key) << endl;
				left->printTreeSub();
				right->printTreeSub();
				return;
			} else if (left) {
				cout << "\n" << *key << " | left: " << *(left->key) << endl;
				left->printTreeSub();
				return;
			} else if (right) {
				cout << "\n" << *key << " | right: " << *(right->key) << endl;
				right->printTreeSub();
				return;
			}
			return;
		}

		void addLeft(Node *le) {
			left = le;
		}

		void addRight(Node *rg) {
			right = rg;
		}

	};

	Node *root;


	Splay() {
		root = new Node();
	}


	void printTree() {
		if (!root->key)
			return;
		Node *rt = root;
		if (!rt->left && !rt->right)
			cout << "\n" << *(rt->key) << endl;
		else
			rt->Node::printTreeSub();
		return;
	}

	void setRoot(Node *rt) {
		root = rt;
	}

	Splay *RR() {
		Node *n1 = root->left;
		root->left = n1->right;
		n1->right = root;
		setRoot(n1);
		return this;
	}

	Splay *LL() {
		Node *n1 = root->right;
		root->right = n1->left;
		n1->left = root;
		setRoot(n1);
		return this;
	}

	Splay *SplayIt(KeyType sKey) {
		Node rt;
		Node *LeftRoot = &rt;
		Node *RightRoot = &rt;
		while (1) {
			if (*sKey < *(root->key)) {
				if (!root->left)
					break;
				if (*sKey < *(root->left->key)) {
					RR();
					if (!root->left)
						break;
				}
				RightRoot->addLeft(root);
				RightRoot = RightRoot->left;
				root = root->left;
				RightRoot->addLeft(NULL);
			} else if (*sKey > *(root->key)) {
				if (!root->right)
					break;
				if (*sKey >
					*(root->right->key)) {
					LL();
					if (!root->right)
						break;
				}
				LeftRoot->addRight(root);
				LeftRoot = LeftRoot->right;
				root = root->right;
				LeftRoot->addRight(NULL);
			} else
				break; //found it
		}
		LeftRoot->addRight(root->left);
		RightRoot->addLeft(root->right);
		root->addLeft(rt.right);
		root->addRight(rt.left);
		return this;
	}


	Node *Insert(KeyType iKey, KeyType dKey = NULL) {
		Node *nd = new Node(iKey, dKey);
		if (!root->key) {
			root = nd;
			return root;
		}
		SplayIt(iKey);
		if (*iKey < *(root->key)) {
			nd->addLeft(root->left);
			nd->addRight(root);
			root->addLeft(NULL);
			root = nd;
		} else if (*iKey > *(root->key)) {
			nd->addRight(root->right);
			nd->addLeft(root);
			root->addRight(NULL);
			root = nd;
		} else
			return nd;
		return root;
	}


	Node *Find(KeyType sKey) {
		if (!root->key)
			return NULL;
		return (SplayIt(sKey)->root);
	}


	void Delete(KeyType iKey) {
		if (!root->key)
			return;
		Node *found = SplayIt(iKey)->root;
		if (*iKey != *(found->key))
			return;
		else {
			if (!root->right && !root->left) {
				found->key = NULL;
				found->data = NULL;
				return;
			} else if (!root->left)
				root = root->right;
			else {
				Node *t = root;
				Splay T;
				Splay *T_ptr = &T;
				T_ptr->setRoot(root->left);
				T_ptr->SplayIt(iKey);
				root = T_ptr->root;
				root->right = t->right;
			}
		}
		delete found;
		return;
	}

	Node *Find_Max() {
		if (!root->key)
			return NULL;
		if (!root->right)
			return root;
		Node *rightN = root->right;
		while (rightN->right) {
			rightN = rightN->right;
		}
		SplayIt(rightN->key);
		return root;
	}

	int getSize() {
		if (!root || !root->key)
			return 0;
		Splay RT;
		Splay LT;
		RT.root = root->right;
		LT.root = root->left;
		return RT.getSize() + LT.getSize() + 1;
	}


	void DeleteTree() { //O(n)
		Node *found = root;
		if(!root)
			return;
		if (!root->right && !root->left) {
			root->key = NULL;
			root->data = NULL;
			return;
		} else if (!root->left) {
			root = new Node(*(root->right));
			delete found;
			DeleteTree();
		} else if (!root->right) {
			root = new Node(*(root->left));
			delete found;
			DeleteTree();
		} else {
			Splay RT;
			Splay LT;
			RT.root = new Node(*(root->right));
			LT.root = new Node(*(root->left));
			delete found;
			LT.DeleteTree();
			RT.DeleteTree();
		}
	}
};

#endif