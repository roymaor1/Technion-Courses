
#ifndef UNTITLED3_COLOSSEUM_H
#define UNTITLED3_COLOSSEUM_H

#endif //UNTITLED3_COLOSSEUM_H

#include "splay.h"
#include "DoubleLinkedList.h"
#include <cstdlib>
using namespace std;

class Colosseum {
public:

	class Trainer {
	public:

		class Gladiator {
		public:
			Trainer* trainer;
			int GladiatorID;
			int level;

			Gladiator(Trainer* t, int gid, int lvl)
			{
				trainer = t;
				GladiatorID = gid;
				level = lvl;
			}


			bool operator<(const Gladiator &gladiator);

			bool operator>(const Gladiator &gladiator);

			bool operator==(const Gladiator &gladiator);

			bool operator!=(const Gladiator &gladiator);



			int getGladiatorID() {
				return GladiatorID;
			}

			int getGladiatorLevel() {
				return level;
			}

			Trainer *getTrainerOfGladiator() {
				return trainer;
			}

			static void inorderGladiators (int** gladiators,
										   Splay<Gladiator*>::Node* p,
										   DoubleLinkedList<int*>* dll);

			static void gladiatorListToArray (int** gladiators, int size, DoubleLinkedList<int*>* dll);

			static  void sortInfluenced (Splay<Gladiator*>::Node* p,
										 DoubleLinkedList<Gladiator*>* influenced,
										 DoubleLinkedList<Gladiator*>* uninfluenced,
										 int code, int factor);

			static void inorderID (Splay<int*>::Node* p,
								   int code, int factor);

				friend ostream& operator<<(ostream& output,
							   const Colosseum::Trainer::Gladiator& gladiator);


			};

		int trainerID;
		Splay<Gladiator *> *trainerGladiators;
		Gladiator *strongest;


		Trainer(int tid = 0)
		{
			trainerID = tid;
			trainerGladiators = new Splay<Gladiator*>();
			strongest = NULL;
		}

		bool operator==(const Trainer &rhs);

		int getTrainerID() {
			return trainerID;
		}

		Splay<Gladiator *> *getGladiatorTree() {
			return trainerGladiators;
		}

		Gladiator *getStrongestGladiator() {
			return strongest;
		}

		void updateStrongest();


		friend ostream& operator<<(ostream& output,
								 const Colosseum::Trainer& trainer);


		};

	Splay<int*>* ID_Tree;
	Splay<Trainer::Gladiator*>* Level_Tree;
	DoubleLinkedList<Trainer*>* TrainersList;



	Colosseum()
	{
		TrainersList = new DoubleLinkedList<Trainer*>();
		ID_Tree = new Splay<int*>();
		Level_Tree = new Splay<Trainer::Gladiator*>();
	}






		StatusType AddTrainerCol(int trainerID) {
		if (trainerID <= 0)
			return INVALID_INPUT;
		Trainer tmp(trainerID);
		DoubleLinkedList<Trainer *>::Node *found =
				TrainersList->DoubleLinkedList::findElement(&tmp);
		if (found)
			return FAILURE;
		try {
			Trainer *newTrainer = new Trainer(trainerID);
			TrainersList->listInsert(newTrainer);
		}
		catch (bad_alloc()) {
			return ALLOCATION_ERROR;
		}
		return SUCCESS;
	}

	StatusType BuyGladiatorCol(int gladiatorID, int trainerID, int level) {
		if (gladiatorID <= 0 || level <= 0 || trainerID <= 0)
			return INVALID_INPUT;
		Trainer tr(trainerID);
		DoubleLinkedList<Trainer*>::Node* found =
				TrainersList->DoubleLinkedList::findElement(&tr);
		if (!found)
			return FAILURE;
		int x = gladiatorID;
		int y = level;
		int* pt = new int(x);
		int* pl = new int(y);
		try {
			if (!ID_Tree->root->key || gladiatorID != *(ID_Tree->Find(pt)->key))
			{
				ID_Tree->Insert(pt,pl);
				Trainer* owner = found->key;
				Trainer::Gladiator* gl = new
						Trainer::Gladiator(owner, gladiatorID, level);
				Level_Tree->Insert(gl,NULL);
				Splay<Trainer::Gladiator*>::Node* current =
						owner->trainerGladiators->Insert(gl,NULL);
				Splay<Trainer::Gladiator*>::Node* strong =
						owner->trainerGladiators->Find_Max();
				owner->strongest = strong->key;
				current->key->trainer->strongest = strong->key;
			}
			else if(ID_Tree->root->key && gladiatorID == *(ID_Tree->Find(pt)->key))
				return FAILURE;
		}//try
		catch (bad_alloc()) {
			return ALLOCATION_ERROR;
		}
		return SUCCESS;
	}

	int FreeGladiatorCol(int gladiatorID){
		if (gladiatorID <= 0)
			return INVALID_INPUT;
		int x = gladiatorID;
		int* pt = new int(x);
		if(ID_Tree->root->key && gladiatorID == *(ID_Tree->Find(pt)->key))
		{
			int lv = *(ID_Tree->Find(pt)->data);
			ID_Tree->Delete(pt); //finds it, then deletes it from id tree
			//creating demo gladiator in order to compare
			Trainer::Gladiator gld_demo(NULL,gladiatorID,lv);
			Splay<Trainer::Gladiator*>::Node* gld_node = Level_Tree->Find(&gld_demo);
			Trainer* tr = gld_node->key->trainer;
			tr->trainerGladiators->Delete(&gld_demo);
			tr->updateStrongest();
			Level_Tree->Delete(&gld_demo); //finds it, then deletes it from lvl tree
		}
		else if (!ID_Tree->root->key || gladiatorID != *(ID_Tree->Find(pt)->key))
			return FAILURE;
		delete pt;
		return SUCCESS;
	}

	int LevelUpCol(int gladiatorID,int levelIncrease){
		if (gladiatorID <= 0 || levelIncrease<=0)
			return INVALID_INPUT;
		int x = gladiatorID;
		int* id_pt = new int(x);
		if(ID_Tree->root->key && gladiatorID == *(ID_Tree->Find(id_pt)->key))
		{
			Splay<int*>::Node* gld_node = ID_Tree->Find(id_pt);
			int oldLevel = *(gld_node->data);
			int newLevel = oldLevel + levelIncrease;
			int* lv_pt = &newLevel;
			Trainer::Gladiator gld_old(NULL,gladiatorID,oldLevel);
			Splay<Trainer::Gladiator*>::Node* gl =
					Level_Tree->Find(&gld_old);
			Trainer* tr = gl->key->trainer;
			Trainer::Gladiator* gld_new = new Trainer::Gladiator(tr,gladiatorID,newLevel);
			int signal = FreeGladiatorCol(gladiatorID); //logn
			if(signal != SUCCESS){
				delete id_pt;
				delete lv_pt;
				return signal;
			}
			//insertion
			ID_Tree->Insert(id_pt,lv_pt);
			Level_Tree->Insert(gld_new);
			tr->trainerGladiators->Insert(gld_new);
			tr->updateStrongest();
		}
		else if (!ID_Tree->root->key || gladiatorID != *(ID_Tree->Find(id_pt)->key)){
			delete id_pt;
			return FAILURE;
		}
		return SUCCESS;
	}

	int GetTopGladiatorCol(int trainerID, int* gladiatorID){
		if (trainerID == 0 || gladiatorID == NULL)
			return INVALID_INPUT;
		if(trainerID < 0){
			if(Level_Tree->root->key){
				int strength=0;
				DoubleLinkedList<Trainer*>::Node* tmp = TrainersList->head;
				while(tmp){
					if(!tmp->key->strongest){
						tmp=tmp->next;
						continue;
					}
					if(strength < tmp->key->strongest->level)
						strength = tmp->key->strongest->level;
					tmp=tmp->next;
				}
				*gladiatorID = strength;
			}
			else
				*gladiatorID = -1;
			return SUCCESS;
		}
		Trainer tr(trainerID); //we know now that trainerID>0
		DoubleLinkedList<Trainer*>::Node* found =
				TrainersList->DoubleLinkedList::findElement(&tr);
		if (!found)
			return FAILURE;
		if(found->key->trainerGladiators->root->key)
			*gladiatorID = found->key->strongest->GladiatorID;
		else
			*gladiatorID = -1;
		return SUCCESS;
	}

	int GetAllGladiatorsByLevelCol(int trainerID,int** gladiators,int* numOfGladiator){
		if(gladiators == NULL || numOfGladiator == NULL || trainerID == 0)
			return INVALID_INPUT;
		if(trainerID<0){
			*numOfGladiator = ID_Tree->getSize();
			if(*numOfGladiator == 0){
				*gladiators = NULL;
				*numOfGladiator = 0;
				return SUCCESS;
			}
			*gladiators = (int*)malloc(sizeof(int)*(*numOfGladiator));
			if(!*gladiators)
				return ALLOCATION_ERROR;
			DoubleLinkedList<int*> dll;
			Splay<Trainer::Gladiator*>::Node* p = Level_Tree->root;
			Trainer::Gladiator::inorderGladiators(gladiators,p,&dll);
			Trainer::Gladiator::gladiatorListToArray(gladiators,*numOfGladiator,&dll);
			return SUCCESS;
		}
		DoubleLinkedList<int*> dll;
		Trainer tr(trainerID); //we know now that trainerID>0
		DoubleLinkedList<Trainer*>::Node* found =
				TrainersList->DoubleLinkedList::findElement(&tr);
		if (!found)
			return FAILURE;
		*numOfGladiator = found->key->trainerGladiators->getSize();
		if(*numOfGladiator == 0){
			*gladiators = NULL;
			*numOfGladiator = 0;
			return SUCCESS;
		}
		*gladiators = (int*)malloc(sizeof(int)*(*numOfGladiator));
		if(!*gladiators)
			return ALLOCATION_ERROR;
		Splay<Trainer::Gladiator*>::Node* p =
				found->key->trainerGladiators->root;
		Trainer::Gladiator::inorderGladiators(gladiators,p,&dll);
		Trainer::Gladiator::gladiatorListToArray(gladiators,*numOfGladiator,&dll);
		return SUCCESS;
}

	int UpgradeGladiatorCol(int gladiatorID,int upgradedID){ //logn
		if(gladiatorID<=0 || upgradedID<=0)
			return INVALID_INPUT;
		if(upgradedID == *(ID_Tree->Find(&upgradedID)->key))
			return FAILURE;
		int x = gladiatorID;
		int* id_pt = new int(x);
		if(ID_Tree->root->key && gladiatorID == *(ID_Tree->Find(id_pt)->key))
		{
			Splay<int*>::Node* gld_node = ID_Tree->Find(id_pt);
			int lvl = *(gld_node->data);
			int* lv_pt = &lvl;
			Trainer::Gladiator gld_old(NULL,gladiatorID,lvl);
			Splay<Trainer::Gladiator*>::Node* gl =
					Level_Tree->Find(&gld_old);
			Trainer* tr = gl->key->trainer;
			Trainer::Gladiator* gld_new =
					new Trainer::Gladiator(tr,upgradedID,lvl);
			//delete
			int signal = FreeGladiatorCol(gladiatorID);
			if(signal != SUCCESS){
				delete id_pt;
				delete lv_pt;
				return signal;
			}
			//insertion
			ID_Tree->Insert(id_pt,lv_pt);
			Level_Tree->Insert(gld_new);
			tr->trainerGladiators->Insert(gld_new);
			tr->updateStrongest();
		}
		else if (!ID_Tree->root->key ||
				gladiatorID != *(ID_Tree->Find(id_pt)->key)){
			delete id_pt;
			return FAILURE;
		}
		return SUCCESS;
	}

	int UpdateLevelsCol(int stimulantCode,int stimulantFactor){
		if(stimulantCode<1 || stimulantFactor<1)
			return INVALID_INPUT;
		DoubleLinkedList<Trainer::Gladiator*> Influenced;
		DoubleLinkedList<Trainer::Gladiator*> Uninfluenced;
		Trainer::Gladiator::sortInfluenced(Level_Tree->root,&Influenced,
										   &Uninfluenced,stimulantCode,
										   stimulantFactor);
		DoubleLinkedList<Trainer::Gladiator*> merged;
		DoubleLinkedList<Trainer::Gladiator*>::mergeLists(&Influenced,&Uninfluenced,&merged);
		Level_Tree->DeleteTree();
		DoubleLinkedList<Trainer::Gladiator*>::Node* tmp = merged.head;
		Splay<Trainer::Gladiator*>::Node* splay_node = Level_Tree->root;
		while(tmp){
			splay_node->key = tmp->key;
			splay_node->right = new Splay<Trainer::Gladiator*>::Node();
			splay_node = splay_node->right;
			tmp = tmp->next;
		}
		Trainer::Gladiator::inorderID(ID_Tree->root,stimulantCode,stimulantFactor);
		DoubleLinkedList<Trainer*>::Node* coach = TrainersList->head;
		while(coach){
			coach->key->trainerGladiators->Splay::DeleteTree();
			coach->key->strongest = NULL;
			coach = coach->next;
		}
		DoubleLinkedList<Trainer::Gladiator*>::Node* gld_node = merged.head;
		while(gld_node){
			Splay<Trainer::Gladiator*>::Node g_node;
			g_node.key = gld_node->key;
			Trainer* tr = gld_node->key->trainer;
			Splay<Trainer::Gladiator*>::Node* new_node = new Splay<Trainer::Gladiator*>::Node(g_node);
			if(!tr->trainerGladiators->root->key)
				tr->trainerGladiators->root = new_node;
			else {
				new_node->left = tr->trainerGladiators->root;
				tr->trainerGladiators->root = new_node;
			}
			gld_node = gld_node->next;
		}
		DoubleLinkedList<Trainer*>::Node* trainer_tmp = TrainersList->head;
		while(trainer_tmp){
			trainer_tmp->key->updateStrongest();
			trainer_tmp = trainer_tmp->next;
		}
		return SUCCESS;
	}



// INTERNAL USE
	void printColosseum(){
		if(!TrainersList->head->key)
			return;
		DoubleLinkedList<Trainer*>::Node* tmp = TrainersList->head;
		cout << "Trainers List :"<< endl;
		while(tmp->next) {
			cout << "-------------------------" << endl;
			cout << "Trainer: " << *(tmp->key) << endl;
			if(tmp->key->strongest)
				cout << "Strongest Gladiator: " << *(tmp->key->strongest) << endl;
			cout << "Trainer Gladiators:";
			if(tmp->key->trainerGladiators->root->key)
				tmp->key->trainerGladiators->Splay::printTree();
			tmp = tmp->next;
		}
		cout << "-------------------------" << endl;
		cout << "Trainer: " << *(tmp->key) << endl;
		if(tmp->key->strongest)
			cout << "Strongest Gladiator: " << *(tmp->key->strongest) << endl;
		cout << "Trainer Gladiators:";
		if(tmp->key->trainerGladiators->root->key)
			tmp->key->trainerGladiators->Splay::printTree();
		cout << "--------" << endl;
		cout << "ID Tree :" << endl;
		if(ID_Tree->root->key)
			ID_Tree->Splay<int*>::printTree();
		cout << "--------" << endl;
		cout << "Level Tree :" << endl;
		if(Level_Tree->root->key)
			Level_Tree->Splay<Trainer::Gladiator*>::printTree();
		cout << "-------------------------" << endl;
		return;
	}


};


std::ostream& operator<<(std::ostream& output, const Colosseum::Trainer::Gladiator& gladiator);
std::ostream& operator<<(std::ostream& output, const Colosseum::Trainer& trainer);