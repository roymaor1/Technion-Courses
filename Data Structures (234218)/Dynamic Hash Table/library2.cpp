#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "elements.h"
#include "library2.h"

void * init(int n, int * trainingGroupsIDs)
{
	try {
		if ((n < 2) || (!trainingGroupsIDs))
			return NULL;
		return new Arena(n, trainingGroupsIDs);
	} catch (Arena::ArenaAllocFailed&) {
		return NULL;
	}
}

StatusType addTrainingGroup(void * DS, int trainingGroupID)
{
	if ((!DS) || (trainingGroupID < 0))
		return INVALID_INPUT;
	try {
		((Arena*)(DS))->AddGroup(trainingGroupID);
	}
	catch (Arena::ArenaAllocFailed&) {
		return ALLOCATION_ERROR;
	}
	catch (Arena::ArenaFAILURE&) {
		return FAILURE;
	}
	return SUCCESS;
}

StatusType addGladiator(void * DS, int gladiatorID, int score, int trainingGroup)
{
	if ((!DS) || (gladiatorID < 0) || (score < 0) || (trainingGroup < 0))
		return INVALID_INPUT;
	try {
		((Arena*)(DS))->AddGladiator(gladiatorID, score, trainingGroup);
	}
	catch (Arena::ArenaAllocFailed&) {
		return ALLOCATION_ERROR;
	}
	catch (Arena::ArenaFAILURE&) {
		return FAILURE;
	}
	return SUCCESS;
}

StatusType trainingGroupFight(void * DS, int trainingGroup1, int trainingGroup2, int k1, int k2)
{
	if ((!DS) || (trainingGroup1 < 0) || (trainingGroup2 < 0) || (k1 <= 0) || (k2 <= 0))
		return INVALID_INPUT;
	try {
		((Arena*)(DS))->Fight(trainingGroup1, k1, trainingGroup2, k2);
	}
	catch (Arena::ArenaAllocFailed&) {
		return ALLOCATION_ERROR;
	}
	catch (Arena::ArenaFAILURE&) {
		return FAILURE;
	}
	return SUCCESS;
}

StatusType getMinTrainingGroup(void * DS, int * trainingGroup)
{
	if ((!DS) || (!trainingGroup)) {
		trainingGroup = NULL;
		return INVALID_INPUT;
	}
	try {
		*trainingGroup = ((Arena*)(DS))->GetMinGroup();
	}
	catch (Arena::ArenaAllocFailed&) {
		trainingGroup = NULL;
		return ALLOCATION_ERROR;
	}
	catch (Arena::ArenaFAILURE&) {
		trainingGroup = NULL;
		return FAILURE;
	}
	return SUCCESS;
}

void quit(void ** DS)
{
	if (!DS) return;
	try {
		delete (Arena*)(*DS);
	}
	catch (std::exception&) {}
	*DS = NULL;
}
