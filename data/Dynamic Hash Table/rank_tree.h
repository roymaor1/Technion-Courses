#ifndef RankTree
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>

/*
classes:
-----------
RankNode	-	represents single node in a Rank tree.
RankTree	-	represents single Rank tree.
*/
/*
typedef int Key;
typedef int Data;
class RankNodeCompFunc {
public:
	int operator() (int a, int b) {
		if (a == b) return 0;
		else if (a > b) return 1;
		return -1;
	}
};

class RankNodeConvert {
public:
	int operator() (int a) {
		return a;
	}
};
*/
template<class Key, class Data, class RankNodeCompFunc, class RankNodeConvert>
class RankTree {
public:
	class RankTreeNodeAlreadyExist : public std::exception {};
	class RankTreeOverSize : public std::exception {};
	class RankTreeAllocFailed : public std::exception {};
	class RankTreeNotEnoughNodes : public std::exception {};
	/* ----------------------------------------------------------------------------------------------- */
	/* ------------ RankNode class ------------------------------------------------------------------ */
	/* ----------------------------------------------------------------------------------------------- */
	class RankNode {
	private:
		Key key;
		Data* data;
		RankNode* rson, *lson, *up;
		int HL, HR, subtree_count, subtree_sum;
	public:
		RankNode(Key k, Data* d, RankNodeConvert convert) : key(k), data(d), rson(NULL), lson(NULL) , up(NULL)
			, HL(0), HR(0), subtree_count(1), subtree_sum(convert(*d)) {}
		~RankNode() {
			if(rson) delete rson;
			if(lson) delete lson;
			if (up) delete up;
			delete this;
		}
		RankNode(const RankNode &node) {
			this->key = node.key;
			this->data = node.data;
			this->rson = node.rson;
			this->lson = node.lson;
			this->up = node.up;
			this->HL = node.HL;
			this->HR = node.HR;
			this->subtree_count = node.subtree_count;
			this->subtree_sum = node.subtree_sum;
		}

		Data*& GetData() { return data; }
		Key& GetKey() { return key; }
		RankNode*& Right() { return rson; }
		RankNode*& Left() { return lson; }
		RankNode*& Up() { return up; }
		int& GetHL() { return HL; }
		int& GetHR() { return HR; }
		int& Count() { return subtree_count; }
		int& Sum() { return subtree_sum; }
	};

	/* ----------------------------------------------------------------------------------------------- */
	/* ------------ RankTree Fields ----------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------------------- */

private:
	RankNode* root;
	int size;
	RankNodeCompFunc comp;
	RankNodeConvert convert;
	/* ----------------------------------------------------------------------------------------------- */
	/* -------- Main RankTree Functions ------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------------------- */
	/*
	RootKey		-	returns the key of the current root
	RootData	-	returns reference to the data's pointer of the current root
	Size		-	returns the current size of the tree (total number of nodes)
	Compare		-	returns the keys' comparing function
	Insert		-	insert new node to the tree
	Find		-	Rank the given key to tree's root. If the key doesn't exist, Rank the last node reached during search
	Delete		-	delete existing node from the tree
	DeleteAllNodes	delete all existing nodes from the tree
	ArrayToTree	-	convert given array to this Rank tree
	TreeToArray	-	convert this Rank tree to given array (memory is externally allocated)	*/
public:
	RankTree(RankNodeCompFunc c, RankNodeConvert con) : root(NULL), size(0), comp(c) ,convert(con) {}
	~RankTree() {}

	int Size() { 
		return size; 
	}

	void Insert(Key k, Data* d) {
		RankNode *node = Find(k);
		if ((this->root) && (node)) 
			throw RankTreeNodeAlreadyExist();
		try {
			node = new RankNode(k, d, convert);
		}
		catch (std::bad_alloc&) {
			throw RankTreeAllocFailed();
		}
		if (!this->root) {
			this->root = node;
		} else { // tree isn't empty
			RankNode* node_father = FindFather(k);
			if (comp(node_father->GetKey(), k) == 1) { // node is the left son
				node_father->Left() = node;
				node_father->GetHL()++;
			}
			else { // node is the right son
				node_father->Right() = node;
				node_father->GetHR()++;
			}
			node_father->Sum() += convert(*d);
			node_father->Count()++;
			node->Up() = node_father;
			if(node_father->Up())
				FixTree(node_father->Up()->GetKey());
		}
		size++;
	}

	void Delete(Key key) {
		RankNode *del = Find(key);
		if (!del) return;
		else if (this->size == 1) {
			delete this->root;
			this->root = NULL;
			this->size = 0;
			return;
		}
		bool isLeft, isRoot = (del->Up() ? false : true);
		Key del_father_key;
		if (!isRoot) {
			del_father_key = del->Up()->GetKey();
			isLeft = (comp(del->Up()->GetKey(), key) == 1) ? true : false;
		}if (del->Left() && del->Right()) {
			Delete2(del);
			FixTree((isRoot ? root->GetKey() :
				(isLeft ? Find(del_father_key)->Left()->GetKey() : Find(del_father_key)->Right()->GetKey())));
		} else if (!del->Left() && !del->Right()) {
			Delete0(del);
			FixTree(del_father_key);
		} else {
			Delete1(del);
			FixTree(del_father_key);
		}
		this->size--;
	}

	void ArrayToTree(int arrays_size, RankNodeCompFunc comp, Key dummy_key, Key**& keys, Data**& data) {
		RankNode* result = AuxBuildEmptyTree((int)(ceil(log2(arrays_size))), arrays_size, dummy_key);
		this->root = result;
		int* indexes = new int[arrays_size];
		for (int i = 0; i < arrays_size; i++)
			indexes[i] = 0;
		AuxArrayToTreeInsertInorder(this->root, 0, keys, data, arrays_size, indexes);
		delete indexes;
		this->size = arrays_size;
	}

	int GetTopK(int k) {
		if (size < k) throw RankTreeNotEnoughNodes();
		if (size == k) return root->Sum();
		int rank = size - k, invert_sum = 0; 
		bool found = false;

		// select function with summation of subtrees according to learned algorithm
		RankNode* iter = root;
		while (!found) { 
			if ((iter->Left()) && (iter->Left()->Count() == rank - 1)) {
				found = true;
				invert_sum += convert(*iter->GetData()) + iter->Left()->Sum();
			}
			else if ((!iter->Left()) && (rank -1 == 0)) {
				found = true;
				invert_sum += convert(*iter->GetData());
			} else if ((iter->Left()) && (iter->Left()->Count() > rank - 1))
				iter = iter->Left();
			else {
				rank = rank - (iter->Left() ? iter->Left()->Count() : 0) - 1;
				invert_sum += (iter->Left() ? iter->Left()->Sum() : 0) + convert(*iter->GetData());
				iter = iter->Right();
			}
		}

		// return : (total_score - calculated_score)
		return (root->Sum() - invert_sum);
	}

	/* ----------------------------------------------------------------------------------------------- */
	/* -------- Aux RankTree Functions -------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------------------- */
/*
	AuxTreeToArray	-	recursive aux function of TreeToArray
	InsertFixTree	-	check if the current node is balanced. If not finds the correct roll and rolls it
	FindSequentialKey -	finds the next sequential node after the one with given key
	RR			-		perform single RR rotation
	LL			-		perform single LL rotation
	RL			-		perform single RL rotation
	LR			-		perform single LR rotation
	AuxBuildEmptyTree -	Creates almost complete empty tree with given size (the height is calculated using log2 function)
	AuxArrayToTreeInsertInorder	- Traverses in-order on the empty tree and updates all nodes from given array
*/
private:

	RankNode* Find(Key key) {
		if (!this->root) return NULL;
		RankNode *it = root;
		while (it && (comp(it->GetKey(), key) != 0)) {
			if (comp(it->GetKey(), key) == 1)
				it = it->Left();
			else
				it = it->Right();
		}
		return it;
	}

	RankNode* FindFather(Key son_key) {
		if (!this->root) return NULL;
		RankNode *it = root, *it_father = NULL;
		while (it && (comp(it->GetKey(), son_key) != 0)) {
			it_father = it;
			if (comp(it->GetKey(), son_key) == 1)
				it = it->Left();
			else
				it = it->Right();
		}
		return it_father;
	}

	void Switch(RankNode*& a, RankNode*& b) {
		RankNode* temp = a;
		if (a->Up())
			a->Up()->Sum() = a->Up()->Sum() - convert(*a->GetData()) + convert(*b->GetData());
		if (b->Up())
			b->Up()->Sum() = b->Up()->Sum() - convert(*b->GetData()) + convert(*a->GetData());
		a->GetKey() = b->GetKey();
		a->GetData() = b->GetData();
		a->Count() = b->Count();
		a->Sum() = b->Sum();
		b->GetKey() = temp->GetKey();
		b->GetData() = temp->GetData();
		b->Count() = temp->Count();
		b->Sum() = temp->Sum();
	}

	void Delete2(RankNode* del) {
		RankNode* to_switch = FindSequentialKey(del);
		Switch(del, to_switch);
		if (!to_switch->Left() && !to_switch->Right())
			Delete0(to_switch);
		else
			Delete1(to_switch);
	}

	void Delete1(RankNode* del) {
		if (del->Left()) {
			Switch(del, del->Left());
			Delete0(del->Left());
		}
		else {
			Switch(del, del->Right());
			Delete0(del->Right());
		}
		del->Count() = 1;
		del->Sum() = convert(*del->GetData());
	}

	void Delete0(RankNode* del) {
		if (comp(del->Up()->GetKey(), del->GetKey()) == 1) {
			del->Up()->Left() = NULL;
			del->Up()->GetHL()--;
		}
		else {
			del->Up()->Right() = NULL;
			del->Up()->GetHR()--;
		}
		del->Up()->Count()--;
		del->Up()->Sum() -= convert(*del->GetData());
		delete del;
	}
	
	void AuxTreeToArray(RankNode* x, int index, Data** res, int* indexes, int res_size) {
		if (!x) return;
		if (index == res_size) throw RankTreeOverSize();
		AuxTreeToArray(x->Left(), index, res, indexes, res_size);
		while (indexes[index] == 1)
			index++;
		res[index] = x->GetData();
		indexes[index] = 1;
		AuxTreeToArray(x->Right(), index + 1, res, indexes, res_size);
	}

	void FixTree(Key start_fix_from_key) {
		// the for loop follows the search route of father_key to the root.
		for (RankNode* iter = Find(start_fix_from_key); iter != NULL; iter = iter->Up()) {
			//update count and sum params of iter
			iter->GetHL() = (iter->Left() ? 1 + std::max(iter->Left()->GetHL(), iter->Left()->GetHR()) : 0);
			iter->GetHR() = (iter->Right() ? 1 + std::max(iter->Right()->GetHL(), iter->Right()->GetHR()) : 0);
			iter->Count() = 1 + (iter->Left() ? iter->Left()->Count() : 0)
				+ (iter->Right() ? iter->Right()->Count() : 0);
			iter->Sum() = convert(*iter->GetData()) + (iter->Left() ? iter->Left()->Sum() : 0)
				+ (iter->Right() ? iter->Right()->Sum() : 0);

			//calc the BFs of iter and its sons
			int iter_BF = iter->GetHL() - iter->GetHR(), iterLeft_BF = 0, iterRight_BF = 0;
			if (iter->Left()) 
				iterLeft_BF = iter->Left()->GetHL() - iter->Left()->GetHR();
			if (iter->Right()) 
				iterRight_BF = iter->Right()->GetHL() - iter->Right()->GetHR();
			
			//check if need to roll
			if ((iter_BF == 2) && (iterLeft_BF >= 0))
				LL(iter->Up(), iter);
			else if ((iter_BF == -2) && (iterRight_BF <= 0))
				RR(iter->Up(), iter);
			else if ((iter_BF == -2) && (iterRight_BF == 1))
				RL(iter, iter->Up());
			else if ((iter_BF == 2) && (iterLeft_BF == -1))
				LR(iter,iter->Up());
		}
	}

	void LL(RankNode* father, RankNode*& oldRoot) {
		RankNode* newRoot = oldRoot->Left();
		if (!newRoot->Right())
			oldRoot->Left() = NULL;
		else {
			oldRoot->Left() = newRoot->Right();
			newRoot->Right()->Up() = oldRoot;
		}
		newRoot->Right() = oldRoot;
		oldRoot->Up() = newRoot;

		// update heights of oldRoot and newRoot
		oldRoot->GetHL() = (oldRoot->Left() ? 1+ std::max(oldRoot->Left()->GetHL(), oldRoot->Left()->GetHR()) : 0);
		oldRoot->GetHR() = (oldRoot->Right() ? 1 + std::max(oldRoot->Right()->GetHL(), oldRoot->Right()->GetHR()) : 0);
		newRoot->GetHL() = (newRoot->Left() ? 1 + std::max(newRoot->Left()->GetHL(), newRoot->Left()->GetHR()) : 0);
		newRoot->GetHR() = (newRoot->Right() ? 1 + std::max(newRoot->Right()->GetHL(), newRoot->Right()->GetHR()) : 0);

		//update count and sum of oldRoot and newRoot
		oldRoot->Count() = 1 + (oldRoot->Left() ? oldRoot->Left()->Count() : 0) + (oldRoot->Right() ? oldRoot->Right()->Count() : 0);
		oldRoot->Sum() = convert(*oldRoot->GetData()) + (oldRoot->Left() ? oldRoot->Left()->Sum() : 0) + (oldRoot->Right() ? oldRoot->Right()->Sum() : 0);
		newRoot->Count() = 1 + (newRoot->Left() ? newRoot->Left()->Count() : 0) + (newRoot->Right() ? newRoot->Right()->Count() : 0);
		newRoot->Sum() = convert(*newRoot->GetData()) + (newRoot->Left() ? newRoot->Left()->Sum() : 0) + (newRoot->Right() ? newRoot->Right()->Sum() : 0);

		if (!father) {
			this->root = newRoot;
			newRoot->Up() = NULL;
		} else {
			if (this->comp(father->GetKey(), oldRoot->GetKey()) == 1)
				father->Left() = newRoot;
			else
				father->Right() = newRoot;
			newRoot->Up() = father;
			// update heights, count and sum of father
			father->GetHL() = (father->Left() ? 1 + std::max(father->Left()->GetHL(), father->Left()->GetHR()) : 0);
			father->GetHR() = (father->Right() ? 1 + std::max(father->Right()->GetHL(), father->Right()->GetHR()) : 0);
			father->Count() = 1 + (father->Left() ? father->Left()->Count() : 0) + (father->Right() ? father->Right()->Count() : 0);
			father->Sum() = convert(*father->GetData()) + (father->Left() ? father->Left()->Sum() : 0) + (father->Right() ? father->Right()->Sum() : 0);
		}
	}

	void RR(RankNode* father, RankNode*& oldRoot) {
		RankNode* newRoot = oldRoot->Right();
		if (!newRoot->Left())
			oldRoot->Right() = NULL;
		else {
			oldRoot->Right() = newRoot->Left();
			newRoot->Left()->Up() = oldRoot;
		}
		newRoot->Left() = oldRoot;
		oldRoot->Up() = newRoot;

		// update heights of oldRoot and newRoot
		oldRoot->GetHL() = (oldRoot->Left() ? 1 + std::max(oldRoot->Left()->GetHL(), oldRoot->Left()->GetHR()) : 0);
		oldRoot->GetHR() = (oldRoot->Right() ? 1 + std::max(oldRoot->Right()->GetHL(), oldRoot->Right()->GetHR()) : 0);
		newRoot->GetHL() = (newRoot->Left() ? 1 + std::max(newRoot->Left()->GetHL(), newRoot->Left()->GetHR()) : 0);
		newRoot->GetHR() = (newRoot->Right() ? 1 + std::max(newRoot->Right()->GetHL(), newRoot->Right()->GetHR()) : 0);

		//update count and sum of oldRoot and newRoot
		oldRoot->Count() = 1 + (oldRoot->Left() ? oldRoot->Left()->Count() : 0) + (oldRoot->Right() ? oldRoot->Right()->Count() : 0);
		oldRoot->Sum() = convert(*oldRoot->GetData()) + (oldRoot->Left() ? oldRoot->Left()->Sum() : 0) + (oldRoot->Right() ? oldRoot->Right()->Sum() : 0);
		newRoot->Count() = 1 + (newRoot->Left() ? newRoot->Left()->Count() : 0) + (newRoot->Right() ? newRoot->Right()->Count() : 0);
		newRoot->Sum() = convert(*newRoot->GetData()) + (newRoot->Left() ? newRoot->Left()->Sum() : 0) + (newRoot->Right() ? newRoot->Right()->Sum() : 0);

		if (!father) {
			this->root = newRoot;
			newRoot->Up() = NULL;
		} else {
			if (this->comp(father->GetKey(), oldRoot->GetKey()) == 1)
				father->Left() = newRoot;
			else
				father->Right() = newRoot;
			newRoot->Up() = father;
			// update heights, count and sum of father
			father->GetHL() = (father->Left() ? 1 + std::max(father->Left()->GetHL(), father->Left()->GetHR()) : 0);
			father->GetHR() = (father->Right() ? 1 + std::max(father->Right()->GetHL(), father->Right()->GetHR()) : 0);
			father->Count() = 1 + (father->Left() ? father->Left()->Count() : 0) + (father->Right() ? father->Right()->Count() : 0);
			father->Sum() = convert(*father->GetData()) + (father->Left() ? father->Left()->Sum() : 0) + (father->Right() ? father->Right()->Sum() : 0);
		}
	}

	void RL(RankNode*& x, RankNode* x_father) {
		LL(x, x->Right());
		RR(x->Up(), x);
	}

	void LR(RankNode*& x, RankNode* x_father) {
		RR(x, x->Left());
		LL(x->Up(), x);
	}

	RankNode* FindSequentialKey(RankNode* node) {
		RankNode* it = node->Right();
		while (it->Left()) it = it->Left();
		return it;
	}

	RankNode* AuxBuildEmptyTree(int height, int size, Key dummy_key) {
		if (size == 0) return NULL;
		RankNode* root = new RankNode(dummy_key, NULL, convert);
		if (size == 1)
			return root;
		root->Left() = AuxBuildEmptyTree(height - 1, ((size - 1) % 2 == 0) ? ((size - 1) / 2) : (((size - 1) / 2) + 1), dummy_key);
		root->Right() = AuxBuildEmptyTree(height - 1, ((size - 1) / 2), dummy_key);
		return root;
	}

	void AuxArrayToTreeInsertInorder(RankNode* root, int index, Key** keys, Data** data, int arrays_size, int* indexes) {
		if (!root) return;
		if (index == arrays_size) throw std::exception();
		AuxArrayToTreeInsertInorder(root->Left(), index, keys, data, arrays_size, indexes);
		while (indexes[index] == 1)
			index++;
		root->GetKey() = *(keys[index]);
		root->GetData() = data[index];
		root->GetHL() = (root->Left() ? 1 + std::max(root->Left()->GetHL(), root->Left()->GetHR()) : 0);
		root->GetHR() = (root->Right() ? 1 + std::max(root->Right()->GetHL(), root->Right()->GetHR()) : 0);
		root->Count() = 1 + (root->Left() ? root->Left()->Count() : 0) + (root->Right() ? root->Right()->Count() : 0);
		root->Count() = convert(*root->GetData()) + (root->Left() ? root->Left()->Sum() : 0) + (root->Right() ? root->Right()->Sum() : 0);
		indexes[index] = 1;
		AuxArrayToTreeInsertInorder(root->Right(), index + 1, keys, data, arrays_size, indexes);
	}
};

#endif // !RankTree