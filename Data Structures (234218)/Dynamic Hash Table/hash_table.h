#ifndef HASH_TABLE

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "linked_list.h"

template<class Key, class Data, class KeyCompare>
class Hash {
	class Pair {
		Key key;
		Data* data;
	public:
		Pair(Key k, Data* d) : key(k), data(d) {}
		~Pair() {}
		Key GetKey() { return key; }
		Data*& GetData() { return data; }
	};

	int size, used;
	LinkedList<Pair>** table;
	KeyCompare comp;
public:
	class HashTAllocFailed : public std::exception {};
	class HashTAlreadyExist : public std::exception {};
	class HashTNotExist : public std::exception {};
	class HashTFull : public std::exception {};
	class HashTEmpty : public std::exception {};
	
	Hash(int size, KeyCompare comp) : used(0), comp(comp) {
		this->size = ChooseSize(size);
		int i = 0;
		try {
			table = new LinkedList<Pair>*[size];
			for (i = 0; i < this->size; i++)
				table[i] = new LinkedList<Pair>();
		}
		catch (std::bad_alloc&) {
			if (table && table[0] && (i < this->size))
				for (int j = 0; j < i; j++)
					delete table[j];
			if (table) delete[] table;
			throw HashTAllocFailed();
		}
	}
	~Hash() {}

	void Insert(Key key, Data* data) {
		try {
			if (this->GetKeyIndex(key) >= 0)
				throw HashTAlreadyExist();
			if (used == size)
				ModifySize(2 * size);
			int index = hash(key, size);
			Pair* new_pair = new Pair(key, data);
			table[index]->Insert(new_pair);
			used++;
		}
		catch (std::bad_alloc&) {
			throw HashTAllocFailed();
		}
	}

	void Remove(Key key) {
		try {
			if (used == 0) return;
			int index = GetKeyIndex(key);
			if (index < 0) return; // meaning key not exist in the table
			typename LinkedList<Pair>::iterator it = table[index]->Begin();
			while (it != table[index]->End()) {
				if (comp((*it)->GetKey(), key) == 0) {
					table[index]->Remove(*it);
					break;
				}
				it++;
			}
			this->used--;
			if (used <= (size / 4))
				ModifySize(size / 2);
		} catch (std::exception&) {
			throw HashTNotExist();
		}
	}

	Data*& GetData(Key key) {
		int index = GetKeyIndex(key);
		if (index < 0)
			throw HashTNotExist();
		typename LinkedList<Pair>::iterator it = table[index]->Begin();
		while (it != table[index]->End()) {
			if (comp((*it)->GetKey(), key) == 0)
				break;
			it++;
		}
		return (*it)->GetData();
	}

	int Size() {
		return size;
	}
private:
	
	int hash(int x, int m) {
		return (x % m);
	}

	int GetKeyIndex(Key key) {
		int index = hash(key, size);
		typename LinkedList<Pair>::iterator it = table[index]->Begin(); 
		while (it != table[index]->End()) {
			if (comp((*it)->GetKey(), key) == 0)
				return index;
			it++;
		}
		return -1;
	}

	int ChooseSize(int n) {
		int i = 1;
		while (pow(2, i) < n)
			i++;
		if ((n % 2 == 0) || (pow(2, i) == n))
			return n + 1;
		return n;
	}
 		
	void ModifySize(int new_size) {
		int i = 0;
		LinkedList<Pair>** modified;
		try {
			// (1) set new size and allocate new table
			int modified_size = ChooseSize(new_size);
			modified = new LinkedList<Pair>*[modified_size];
			for (i = 0; i < modified_size; i++)
				modified[i] = new LinkedList<Pair>();

			// (2) insert (by hashing) all elements (of all lists) to new table
			for (int index = 0; index < size; index++) {
				if (table[index]->IsEmpty())
					continue;
				typename LinkedList<Pair>::iterator it = table[index]->Begin();
				while( it != table[index]->End()) {
					int key = (*it)->GetKey(), new_index = hash(key, modified_size);
					modified[new_index]->Insert(new Pair(key, (*it)->GetData()));
					it++;
				}
			}
			// (3) update HashT fields accordingly
			table = modified;
			size = modified_size;
		}
		catch (std::bad_alloc&) {
			if (i < new_size)
				for (int j = 0; j < i; j++)
					delete modified[j];
			delete table;
			throw HashTAllocFailed();
		}
	}

};


#endif // !HASH_TABLE
