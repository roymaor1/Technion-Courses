/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library1.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_
#define _234218_WET1_

#ifdef __cplusplus
extern "C" {
#endif

	/* Return Values
	* ----------------------------------- */
	typedef enum {
		SUCCESS = 0,
		FAILURE = -1,
		ALLOCATION_ERROR = -2,
		INVALID_INPUT = -3
	} StatusType;

	/* Required Interface for the Data Structure
	* -----------------------------------------*/

	/* Description:   Initiates the data structure.
	* Input:         DS - A pointer to the data structure.
	* Output:        None.
	* Return Values: A pointer to a new instance of the data structure - as a void* pointer.
	*/
	void* Init();

	/* Description:   Adds a new trainer.
	* Input:         DS - A pointer to the data structure.
	*                trainerID - The ID of the trainer to add.
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL or if trainerID <= 0.
	*                FAILURE - If trainerID is already in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType AddTrainer(void *DS, int trainerID);

	/* Description:   Adds a new gladiator to the system.
	* Input:         DS - A pointer to the data structure.
	*                gladiatorID - ID of the gladiator to add.
	*                trainerID - The ID of the gladiator's trainer
	*                level - The gladiator's level
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL, or if gladiatorID <=0, or if trainerID <=0, or if level <= 0
	*                FAILURE - If gladiatorID is already in the DS, or trainerID isn't in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType BuyGladiator(void *DS, int gladiatorID, int trainerID, int level);

	/* Description:   Removes an existing gladiator.
	* Input:         DS - A pointer to the data structure.
	*                gladiatorID - The ID of the gladiator to remove.
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL or if gladiatorID <= 0.
	*                FAILURE - If gladiatorID isn't in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType FreeGladiator(void *DS, int gladiatorID);

	/* Description:   Increases the level of a gladiator.
	* Input:         DS - A pointer to the data structure.
	*                gladiatorID - The ID of the gladiator.
	*		            levelIncrease - The increase in level.
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL, or if gladiatorID<=0, or if levelIncrease<=0
	*                FAILURE - If gladiatorID isn't in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType LevelUp(void *DS, int gladiatorID, int levelIncrease);

	/* Description:   Upgrades a gladiator, updating his ID, while maintaining his level.
	* Input:         DS - A pointer to the data structure.
	*                gladiatorID - The original ID of the gladiator.
	*                upgradedID - The new ID of the gladiator.
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL, or if gladiatorID<=0, or if upgradedID<=0.
	*                FAILURE - If gladiatorID isn't in the DS, or upgradedID is in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType UpgradeGladiator(void *DS, int gladiatorID, int upgradedID);

	/* Description:   Returns the gladiator with the highest level from trainerID
	* 			If trainerID < 0, returns the top gladiator in the entire DS.
	* Input:         DS - A pointer to the data structure.
	*                trainerID - The trainer that we'd like to get the data for.
	* Output:        gladiatorID - A pointer to a variable that should be updated to the ID of the top gladiator.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL, or if gladiatorID == NULL, or if trainerID == 0.
	*                SUCCESS - Otherwise.
	*/
	StatusType GetTopGladiator(void *DS, int trainerID, int *gladiatorID);

	/* Description:   Returns all the gladiators from trainerID sorted by their level.
	*           			If trainerID < 0, returns all the gladiators in the entire DS sorted by their level.
	* Input:         DS - A pointer to the data structure.
	*                trainerID - The trainer that we'd like to get the data for.
	* Output:        gladiators - A pointer to a to an array that you should update with the gladiators' IDs sorted by their level,
	*			          in case two gladiators have same level they should be sorted by their ID.
	*                numOfGladiator - A pointer to a variable that should be updated to the number of gladiators.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If any of the arguments is NULL or if trainerID == 0.
	*                SUCCESS - Otherwise.
	*/
	StatusType GetAllGladiatorsByLevel(void *DS, int trainerID, int **gladiators, int *numOfGladiator);

	/* Description:   Updates the level of the gladiators where gladiatorID % stimulantCode == 0.
	* 			          For each matching gladiator, multiplies its level by stimulantFactor.
	* Input:         DS - A pointer to the data structure.
	*                stimulantCode - The basis that the stimulant works on
	*          		  stimulantFactor - The multiply factor of the level
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL or if stimulantCode < 1 or if stimulantFactor <1
	*                SUCCESS - Otherwise.
	*/
	StatusType UpdateLevels(void *DS, int stimulantCode, int stimulantFactor);


	/* Description:   Quits and deletes the database.
	*                DS should be set to NULL.
	* Input:         DS - A pointer to the data structure.
	* Output:        None.
	* Return Values: None.
	*/


	void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET1_ */