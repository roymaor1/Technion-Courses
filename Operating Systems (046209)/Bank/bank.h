#ifndef __BANK_H__
#define __BANK_H__

/*************************************************/
/*						bank.h				     */
/*************************************************/

// Includes:
#include <pthread.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <random>
#include <map>
#include <math.h>
#include "account.h"
#include "logger.h"
#include "atm.h"
using namespace std;
/*************************************************/

// Constants:

typedef enum ACCESS_MODE
{
	READ,
	WRITE
} ACCESS_MODE;
/*************************************************/

class Bank
{
	public:
	static const int BANK_PRINT_uS = 500*1000;
	static const int BANK_FEE = 3;
	Bank(int num_atms);
	~Bank();

	Account* GetAccount(int account_id, ACCESS_MODE mode);
	void ReturnAccount(int account_id, ACCESS_MODE mode);
	int AddAtm(Atm* atm);
	Atm* GetAtm(int index);
	int AddAccount(int account_id, int pass, int initial_balance);
	void TakeFees();
	void PrintStatus();

	bool work;

	private:

	int bankBalance;
	int numOfAtms;
	pthread_mutex_t readMutex;
	pthread_mutex_t writeMutex;
	int readCount;

	map<int, Account*> accountList;
	map<int, pthread_mutex_t*> accountReadMutexList;
	map<int, pthread_mutex_t*> accountWriteMutexList;
	map<int, int*> accountReadCountList;
	vector<Atm*> atmVec;
};





#endif //__BANK_H__

