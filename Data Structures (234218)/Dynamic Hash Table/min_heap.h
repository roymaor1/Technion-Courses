#ifndef MIN_HEAP
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#define REMOVED -10

class MinHeap {
	int* heap;
	int size;
	int used;
public:
	class MinHeapAllocFailed : public std::exception {};

	MinHeap(int size, int* arr = NULL) : size(size) {
		if (size > 0) {
			try {
				heap = new int[size];
			}
			catch (std::bad_alloc&) {
				throw MinHeapAllocFailed();
			}
			for (int i = 0; i < size; i++)
				heap[i] = arr[i];
			used = size;
			for (int i = (int)(std::floor((size - 1) / 2)); i >= 0; i--)
				SiftDown(i);
		}
	}

	MinHeap(const MinHeap& m) {
		size = m.size;
		for (int i = 0; i < size; i++)
			heap[i] = m.heap[i];
	}

	~MinHeap() {
		delete[] heap;
	}

	void Insert(int new_key) {
		if (used == size)
			ModifySize(size * 2);
		int new_key_index = used;
		int father_index = (int)(std::floor((used - 1) / 2));
		heap[new_key_index] = new_key;
		SiftUp(new_key_index, father_index);
		used++;
	}

	void DelMin() {
		Switch(0, used - 1); // switch the root and the last son in whole heap
		heap[used - 1] = REMOVED;
		used--; 
		SiftDown(0);
		if (used == (size / 4))
			ModifySize(size / 2);
	}

	int Root() {
		if ((used == 0) || (size == 0))
			return REMOVED;
		return heap[0];
	}

private:
	void SiftUp(int son_index, int father_index) {
		while (heap[son_index] < heap[father_index]) {
			Switch(son_index, father_index);
			son_index = father_index;
			father_index = (int)(std::floor((son_index - 1)/ 2));
		}
	}

	bool inRange(int index) {
		if (index < 0) return false;
		if (index >= used) return false;
		return true;
	}

	void SiftDown(int father_index) {
		bool needed = true; 
		while (needed) {	
			int l_son_index = 2 * father_index + 1, r_son_index = 2 * father_index + 2;
			if (((inRange(r_son_index)) && (heap[father_index] > heap[r_son_index])) ||
				((inRange(l_son_index)) && (heap[father_index] > heap[l_son_index]) )) {
				if ( (inRange(r_son_index)) && (inRange(l_son_index)) && (heap[r_son_index] > heap[l_son_index]) ) {
					Switch(father_index, l_son_index);
					father_index = l_son_index;
				}
				else if (!inRange(r_son_index)) {
					Switch(father_index, l_son_index);
					father_index = l_son_index;
				}
				else {
					Switch(father_index, r_son_index);
					father_index = r_son_index;
				}
				needed = true;
			} else 
				needed = false;
		}
	}

	void Switch(int index1, int index2) {
		int temp = heap[index1];
		heap[index1] = heap[index2];
		heap[index2] = temp;
	}

	void ModifySize(int new_size) {
		try {
			// (1) allocate new heap
			int* modified = new int[new_size];
			for (int i = 0; i < new_size; i++)
				modified[i] = REMOVED;

			// (2) copy all old heap elements to new heap
			for (int it = 0; it < size; it++)
				modified[it] = heap[it];
			delete[] heap;
			// (3) update accordingly
			heap = modified;
			size = new_size;
		}
		catch (std::bad_alloc&) {
			throw MinHeapAllocFailed();
		}
	}
};
#endif // !MIN_HEAP
