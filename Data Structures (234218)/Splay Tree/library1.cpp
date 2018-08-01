//
// Created by ZOHAR on 04/12/2017.
//

#include "library1.h"
#include "colosseum.h"


void* Init()
{
    Colosseum* DS = new Colosseum();
    return (void*)DS;
}

StatusType AddTrainer(void *DS, int trainerID){
		if(DS == NULL)
			return INVALID_INPUT;
		return (StatusType) ((Colosseum*)DS)->AddTrainerCol(trainerID);
}


StatusType BuyGladiator(void *DS, int gladiatorID, int trainerID, int level){
	if(DS == NULL)
		return INVALID_INPUT;
	return (StatusType) ((Colosseum *) DS)->BuyGladiatorCol(gladiatorID,
																trainerID,
																level);
}

StatusType FreeGladiator(void *DS, int gladiatorID){
	if(DS == NULL)
		return INVALID_INPUT;
	return (StatusType) ((Colosseum *) DS)->FreeGladiatorCol(gladiatorID);
}


StatusType LevelUp(void *DS, int gladiatorID, int levelIncrease) {
	if(DS == NULL)
		return INVALID_INPUT;
	return (StatusType) ((Colosseum *) DS)->LevelUpCol(gladiatorID,
														   levelIncrease);
}


StatusType GetTopGladiator(void *DS, int trainerID, int * gladiatorID){
	if(DS == NULL)
		return INVALID_INPUT;
	return (StatusType) ((Colosseum *) DS)->GetTopGladiatorCol(trainerID,
																   gladiatorID);
}

StatusType GetAllGladiatorsByLevel(void *DS, int trainerID, int **gladiators, int *numOfGladiator){
	if(DS == NULL)
		return INVALID_INPUT;
	return (StatusType) ((Colosseum *) DS)->GetAllGladiatorsByLevelCol(
				trainerID, gladiators, numOfGladiator);
}

StatusType UpgradeGladiator(void *DS, int gladiatorID, int upgradedID){
	if(DS == NULL)
		return INVALID_INPUT;
	return (StatusType) ((Colosseum *) DS)->UpgradeGladiatorCol(gladiatorID,
																	upgradedID);
}

StatusType UpdateLevels(void *DS, int stimulantCode, int stimulantFactor){
	if(DS == NULL)
		return INVALID_INPUT;
	return (StatusType) ((Colosseum *) DS)->UpdateLevelsCol(stimulantCode,
																stimulantFactor);
}

void Quit(void **DS){
	Colosseum** structure = (Colosseum**)DS;
	Colosseum* st = *structure;
	st->Level_Tree->DeleteTree();
	//delete st->Level_Tree->root;
	st->ID_Tree->DeleteTree();
	//delete st->ID_Tree->root;
	DoubleLinkedList<Colosseum::Trainer*>::Node* tr_node = st->TrainersList->head;
	tr_node->key->trainerGladiators->DeleteTree();
	delete tr_node->key->trainerGladiators->root;
	delete tr_node->key->strongest;
	tr_node = tr_node->next;

	while(tr_node){
		delete tr_node->previous;
		tr_node->key->trainerGladiators->DeleteTree();
		delete tr_node->key->trainerGladiators->root;
		delete tr_node->key->strongest;
		tr_node = tr_node->next;
	}
	st->TrainersList->tail->key->trainerGladiators->DeleteTree();
	delete st->TrainersList->tail->key->trainerGladiators->root;
	delete st->TrainersList->tail->key->strongest;
	*DS = NULL;
}