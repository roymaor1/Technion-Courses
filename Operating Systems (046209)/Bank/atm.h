#ifndef __ATM_H__
#define __ATM_H__

/*************************************************/
/*						atm.h				     */
/*************************************************/

// Includes:
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "account.h"
#include "logger.h"
using namespace std;
/*************************************************/

// Constants:
const int ATM_SLEEP = 1;
const int SLEEP_BETWEEN_OPS_uS = 100000;
const int MAX_ARG = 5;


/*************************************************/


class Atm
{
	public:

	Atm(int atm_id, char* input_file_name);
    
	void Operation(char str[MAXLINE]);
	int OpenAccount(int account_num, int password, int initial_amount);
	int MakeVip(int account_num, int pass);
	int Deposit(int account_num, int pass, int amount);
	int Withdraw(int account_num, int pass, int amount);
	int GetBalance(int account_num, int pass);
	int Transfer(int source_account_num, int source_pass, int target_account_num, int amount, int* p_source_balance, int* p_target_balance);

	int atmId;
	FILE *inputFile;

	private:
	void ReadLock(void);
	void ReadUnlock(void);
	void WriteLock(void);
	void WriteUnlock(void);
	void LockMutex(pthread_mutex_t *p_mutex);
	void UnlockMutex(pthread_mutex_t *p_mutex);
	void InitMutex(pthread_mutex_t *p_mutex);
	void DestroyMutex(pthread_mutex_t *p_mutex);
	// Logger
	pthread_mutex_t accessMutex;

};




#endif //__ATM_H__

