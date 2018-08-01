//
// Created by ZOHAR on 28/11/2017.
//
#include "DoubleLinkedList.h"
#include "colosseum.h"



bool Colosseum::Trainer::Gladiator::operator<(const Gladiator& rhs){
    if(level < rhs.level)
        return true;
    if(level > rhs.level)
        return false;
    if(GladiatorID > rhs.GladiatorID)
        return true;
    return false;
}

bool Colosseum::Trainer::Gladiator::operator>(const Gladiator& rhs)
{
	if(level > rhs.level)
		return true;
	if(level < rhs.level)
		return false;
	if(GladiatorID < rhs.GladiatorID)
		return true;
	return false;
}

bool Colosseum::Trainer::operator==(const Trainer& rhs){
	if(trainerID == rhs.trainerID)
		return true;
	return false;
}

bool Colosseum::Trainer::Gladiator::operator==(const Gladiator& gladiator){
	if(GladiatorID == gladiator.GladiatorID)
		return true;
	return false;
}

bool Colosseum::Trainer::Gladiator::operator!=(const Gladiator &gladiator){
	return (!(*this==gladiator));
}

ostream& operator<<(ostream& output,
					const Colosseum::Trainer::Gladiator& gladiator){
				return output << "ID: " << gladiator.GladiatorID <<
							  " Level: " << gladiator.level;
}


ostream& operator<<(ostream& output, const Colosseum::Trainer& trainer){
	return output << "ID: " << trainer.trainerID;
}


void Colosseum::Trainer::updateStrongest(){
	Splay<Colosseum::Trainer::Gladiator*>::Node* strong =
			trainerGladiators->Find_Max();
	if(strong)
		strongest = strong->key;
	else
		strongest = NULL;

}

void Colosseum::Trainer::Gladiator::inorderGladiators (int** gladiators,
													   Splay<Gladiator*>::Node* p,
													   DoubleLinkedList<int*>* dll) {
	if (!p || !p->key)
		return;
	inorderGladiators(gladiators,p->left,dll);
	int x = p->key->GladiatorID;
	int* x_ptr = new int(x);
	(*dll).listInsert(x_ptr);
	inorderGladiators(gladiators,p->right,dll);
}

void Colosseum::Trainer::Gladiator::gladiatorListToArray (int** gladiators,int size,DoubleLinkedList<int*>* dll){
	if(!dll->head->key)
		return;
	DoubleLinkedList<int*>::Node* tmp = dll->head;
	for(int i=size-1; i>=0; i--){
		*(*gladiators+i) = *(tmp->key);
		tmp=tmp->next;
	}
	return;
}

void Colosseum::Trainer::Gladiator::sortInfluenced (Splay<Gladiator*>::Node* p,
													DoubleLinkedList<Gladiator*>* influenced,
													DoubleLinkedList<Gladiator*>* uninfluenced,
													int code, int factor){
	if (!p || !p->key)
		return;
	sortInfluenced(p->left,influenced,uninfluenced,code,factor);
	Trainer* tr = new Trainer(*p->key->trainer);
	if(p->key->GladiatorID % code == 0){
		Gladiator* gl = new Gladiator(tr,p->key->GladiatorID, (p->key->level)*factor);
		influenced->listInsert(gl);
	}
	else{
		Gladiator* gl = new Gladiator(tr,p->key->GladiatorID,p->key->level);
		uninfluenced->listInsert(gl);
	}
	sortInfluenced(p->right,influenced,uninfluenced,code,factor);
	return;
}


void Colosseum::Trainer::Gladiator::inorderID (Splay<int*>::Node* p,
													int code, int factor){
	if (!p || !p->key)
		return;
	inorderID(p->left,code,factor);
	int k = *(p->key);
	int lv =*(p->data);
	if(k%code==0){
		*(p->data) = lv*factor;
	}
	inorderID(p->right,code,factor);
	return;
}

