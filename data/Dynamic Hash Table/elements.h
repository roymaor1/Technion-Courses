#ifndef ELEMENTS
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "rank_tree.h"
#include "hash_table.h"
#include "min_heap.h"

class Arena {
public:
	class ArenaAllocFailed : public std::exception {};
	class ArenaFAILURE : public std::exception {};

	class Pair {
		int id, score;
	public:
		Pair(int g_id, int score) : id(g_id), score(score) {}
		~Pair() {}
		Pair(const Pair &g) {
			this->id = g.id;
			this->score = g.score;
		}
		int& ID() { return id; }
		int& Score() { return score; }
	};

	class Convert {
	public:
		int operator() (Pair a) {
			return a.Score();
		}

		int operator() (int a) {
			return a;
		}
	};

	class Compares {
	public:
		int operator() (int a, int b) {
			if (a == b) return 0;
			else if (a > b) return 1;
			return -1;
		}

		int operator() (Pair a, Pair b) {
			if (a.Score() > b.Score()) return 1;
			else if ((a.Score() == b.Score()) && (a.ID() < b.ID())) return 1;
			else if (a.Score() < b.Score()) return -1;
			else if ((a.Score() == b.Score()) && (a.ID() > b.ID())) return -1;
			else return 0;
		}
	};	

	/* ----------------------------------------------------------------------------------------------- */
	/* -------------- Group class -------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------------------- */

	class Group {
		int id;
		bool lost, enabled;
		RankTree<Pair, Pair, Compares, Convert>* gladiators;
	public:
		Group(int id) : id(id), lost(false), enabled(false), gladiators(new RankTree<Pair, Pair, Compares, Convert>(Compares(), Convert())) {}
		~Group() {}

		int Id() { return id; }
		bool& Enabled() { return enabled; }
		bool& Lost() { return lost; }
		
		void AddGlad(int glad_id, int score) {
			Pair* p = NULL;
			try {
				p = new Pair(glad_id, score);
				gladiators->Insert(*p, p);
			}
			catch (RankTree<Pair, Pair, Compares, Convert>::RankTreeNodeAlreadyExist&) {
				if(p) delete p;
				throw ArenaFAILURE();
			}
			catch (RankTree<Pair, Pair, Compares, Convert>::RankTreeAllocFailed&) {
				if (p) delete p;
				throw ArenaAllocFailed();
			}
		}

		int GetTopK(int k) {
			if (gladiators->Size() < k) return -1;
			try {
				return gladiators->GetTopK(k);
			} catch (RankTree<Pair, Pair, Compares, Convert>::RankTreeNotEnoughNodes&) {
				return -1;
			}
		}

	};

/* ----------------------------------------------------------------------------------------------- */
/* -------------- Arena class -------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------- */
private:
	Hash<int,Group,Compares>* groups_table;
	MinHeap* groups_heap;
	RankTree<int, int, Compares, Convert>* gladiators_id_tree;

public:
	Arena(int n, int* ids) : groups_table(NULL), groups_heap(NULL), gladiators_id_tree(NULL) {
		try {
			groups_table = new Hash<int, Group, Compares>(n, Compares());
			gladiators_id_tree = new RankTree<int, int, Compares, Convert>(Compares(), Convert());
			Group* g;
			for (int i = 0; i < n; i++) {
				if (ids[i] < 0) {
					delete groups_table;
					delete gladiators_id_tree;
					throw ArenaAllocFailed();
				}
				g = new Group(ids[i]);
				g->Enabled() = true;
				groups_table->Insert(ids[i], g);
			}
			groups_heap = new MinHeap(n, ids);
		}
		catch (std::bad_alloc&) {
			if (groups_table) delete groups_table;
			if (gladiators_id_tree) delete gladiators_id_tree;
			if (groups_heap) delete groups_heap;
			throw ArenaAllocFailed();
		}
		catch (Hash<int, Group, Compares>::HashTAllocFailed&) {
			throw ArenaAllocFailed();
		}
		catch (MinHeap::MinHeapAllocFailed&) {
			if (groups_table) delete groups_table;
			if (gladiators_id_tree) delete gladiators_id_tree;
			if (groups_heap) delete groups_heap;
			throw ArenaAllocFailed();
		}
		catch (Hash<int, Group, Compares>::HashTAlreadyExist&) {
			if (groups_table) delete groups_table;
			if (gladiators_id_tree) delete gladiators_id_tree;
			if (groups_heap) delete groups_heap;
			throw ArenaAllocFailed();
		}
	}

	~Arena() {
		delete groups_heap;
		delete groups_table;
		delete gladiators_id_tree;
	}

	void AddGroup(int id) {
		Group* g = NULL; 
		try {
			try {
				if (groups_table->GetData(id)->Enabled())
					throw ArenaFAILURE();
			}
			catch (Hash<int, Group, Compares>::HashTNotExist&) { // if group_id wasn't inserted in initial id's array
				try {
					g = new Group(id);
					groups_table->Insert(id, g);
				}
				catch (std::bad_alloc&) {
					throw ArenaAllocFailed();
				}
			}
			groups_table->GetData(id)->Enabled() = true;
			groups_heap->Insert(id);
		}
		catch (std::bad_alloc&) {
			throw ArenaAllocFailed();
		}
		catch (MinHeap::MinHeapAllocFailed&) {
			if(g) delete g;
			throw ArenaAllocFailed();
		}
	}
	
	void AddGladiator(int id, int score, int group_id) { // throws : ArenaAllocFailed
		try {
			if ((groups_table) && (groups_table->GetData(group_id)->Enabled() == true)) { // can throw HashTNotExist()
				gladiators_id_tree->Insert(id, &id); // can throw RankTreeNodeAlreadyExist()
				groups_table->GetData(group_id)->AddGlad(id, score);
			}			
		}
		catch (RankTree<int, int, Compares, Convert>::RankTreeAllocFailed&) {
			throw ArenaAllocFailed();
		}
		catch (Hash<int, Group, Compares>::HashTNotExist&) {
			throw ArenaFAILURE();
		}
		catch (RankTree<int, int, Compares, Convert>::RankTreeNodeAlreadyExist&) {
			throw ArenaFAILURE();
		}
	}

	void Fight(int group1_id, int k1, int group2_id, int k2) {
		try {
			if (group1_id == group2_id)
				throw ArenaFAILURE();
			else if ((groups_table->GetData(group1_id)->Enabled() == false) ||
				(groups_table->GetData(group2_id)->Enabled() == false))
				throw ArenaFAILURE();
			else if ((groups_table->GetData(group1_id)->Lost() == true) ||
				(groups_table->GetData(group2_id)->Lost() == true))
				throw ArenaFAILURE();

			// GetTopK function checks if (k > total_gladiators_in_group)
			// if so, returns -1 as a result
			int lost_group = 0, sum1 = groups_table->GetData(group1_id)->GetTopK(k1), 
				sum2 = groups_table->GetData(group2_id)->GetTopK(k2);
			
			if ((sum1 < 0) || (sum2 < 0)) 
				throw ArenaFAILURE();

			// determine which group lost
			if (sum1 == sum2) {
				if (group1_id < group2_id)
					lost_group = group2_id;
				else
					lost_group = group1_id;
			}
			else if (sum1 > sum2)
				lost_group = group2_id;
			else
				lost_group = group1_id;

			groups_table->GetData(lost_group)->Lost() = true;

			// remove all accesible groups (that lost) from heap
			try
			{
				while ((groups_heap->Root() >= 0) && (groups_table->GetData(groups_heap->Root())->Lost() == true))
						groups_heap->DelMin();
			}
			catch (Hash<int, Group, Compares>::HashTNotExist&) {}
		} catch (Hash<int, Group, Compares>::HashTNotExist&) {
			throw ArenaFAILURE();
		}
	}

	int GetMinGroup() {
		int min = groups_heap->Root();
		if (min < 0)
			throw ArenaFAILURE();
		return min;
	}
};

#endif // !ELEMENTS










