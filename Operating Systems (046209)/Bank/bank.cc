/*
 * bank.cc
 *
 *  Created on: May 26, 2018
 *      Author: roy
 */

#include "bank.h"
#include "atm.h"
#include "account.h"
#include "mutex_operations.h"


static const char* feeMsg = "Bank: commissions of %.02f %% were charged, the bank gained %d $ from account %d\n";
static const char* statusOpenMsg = "Current Bank Status";
//static const char* statusMidMsg = "Account %5d: Balance - %5d $ , Account Password - %4d\n";
//static const char* statusEndMsg = "The Bank Has %5d $\n";

extern Logger* logger;

/* Function Name: Bank
 * Bank constructor. It initializes the bank mutexes, balance and number of atms according to input. */
Bank::Bank(int num_atms)
{
	InitMutex(&readMutex);
	InitMutex(&writeMutex);
	readCount = 0;

	bankBalance = 0;
	numOfAtms = num_atms;
	work = true;
}
/* Function Name: ~Bank
 * Bank destructor. It deletes all accounts, their mutexes and all atms. */
Bank::~Bank()
{
	WriteLock(&writeMutex);

	// Delete Account and Account Mutexes
	for (map<int, pthread_mutex_t*>::iterator i = accountReadMutexList.begin(); i != accountReadMutexList.end(); ++i)
	{
		int account_id = i->first;
		WriteLock(accountWriteMutexList[account_id]);
		delete accountList[account_id];
		accountList[account_id] = NULL;

		WriteUnlock(accountWriteMutexList[account_id]);

		delete accountReadCountList[account_id];
		accountReadCountList[account_id] = NULL;

		DestroyMutex(accountWriteMutexList[account_id]);
		DestroyMutex(accountReadMutexList[account_id]);

		delete accountReadMutexList[account_id];
		delete accountWriteMutexList[account_id];
	}
	WriteUnlock(&writeMutex);

	// Delete read/write mutexes of bank
	DestroyMutex(&readMutex);
	DestroyMutex(&writeMutex);

	// Delete ATM vector
	for(vector<Atm*>::iterator i = atmVec.begin(); i != atmVec.end(); ++i)
	{
	    delete *i;
	}
}
/* Function Name: GetAccount
 * This function gets an Atm pointer and adds it to the atmVec */
Account* Bank::GetAccount(int account_id, ACCESS_MODE mode)
{
	ReadLock(&readMutex, &writeMutex, &readCount);
	if (accountList.find(account_id) != accountList.end()) // Account Exists
	{
		if (mode == READ)
		{
			ReadLock(accountReadMutexList[account_id], accountWriteMutexList[account_id], accountReadCountList[account_id]);
		}
		else // WRITE
		{
			WriteLock(accountWriteMutexList[account_id]);
		}

		Account* ret = accountList[account_id];
		WriteUnlock(&writeMutex);
		return ret;
	}
	ReadUnlock(&readMutex, &writeMutex, &readCount);
	return NULL;
}
/* Function Name: ReturnAccount
 * This function gets an Atm pointer and adds it to the atmVec */
void Bank::ReturnAccount(int account_id, ACCESS_MODE mode)
{
	ReadLock(&readMutex, &writeMutex, &readCount);
	if (mode == READ)
	{
		ReadUnlock(accountReadMutexList[account_id], accountWriteMutexList[account_id], accountReadCountList[account_id]);
	}
	else //WRITE
	{
		WriteUnlock(accountWriteMutexList[account_id]);
	}
	ReadUnlock(&readMutex, &writeMutex, &readCount);
}

/* Function Name: AddAtm
 * This function gets an Atm pointer and adds it to the atmVec */
int Bank::AddAtm(Atm* atm)
{
	atmVec.push_back(atm);
	return SUCCESS;
}
/* Function Name: GetAtm
 * This function returns a pointer to the atm which index is the index given as input */
Atm* Bank::GetAtm(int index)
{
	return atmVec[index];
}

/* Function Name: AddAccount
 * This function is called from the atmOp thread when an open account operation is requested.
 * It is called from the Atm::Operation function.
 * It creates a new account and stores its pointer in the account list (Also creates a reader mutex, writer mutex and readcount value
 * and stores them in their respective lists */
int Bank::AddAccount(int account_id, int pass, int initial_balance)
{
	WriteLock(&writeMutex);
	if (accountList.find(account_id) != accountList.end())
	{
		WriteUnlock(&writeMutex);
		return -ACCOUNT_EXISTS;
	}
	pthread_mutex_t* p_new_read_mutex = new pthread_mutex_t;
	pthread_mutex_t* p_new_write_mutex = new pthread_mutex_t;
	int *p_new_read_count = new int;
	*p_new_read_count = 0;


	accountReadMutexList[account_id] = p_new_read_mutex;
	accountWriteMutexList[account_id] = p_new_write_mutex;
	accountReadCountList[account_id] = p_new_read_count;

	InitMutex(accountReadMutexList[account_id]);
	InitMutex(accountWriteMutexList[account_id]);
	*(accountReadCountList[account_id]) = 0;

	Account* p_new_account = new Account(account_id, pass, initial_balance);
	accountList[account_id] = p_new_account;

	WriteUnlock(&writeMutex);
	return SUCCESS;
}

/* Function Name: TakeFees
 * This function is called by the FeesOp thread. It charges random fee percentage
 * from all accounts, according to the exercise instructions */
void Bank::TakeFees()
{
    WriteLock(&writeMutex);
    for (map<int, Account*>::iterator i = accountList.begin(); i != accountList.end(); ++i)
    {
    	int account_id = i->first;
    	WriteLock(accountWriteMutexList[account_id]);
    	Account *p_curr_account = i->second;
    	float percent = ((rand() % 201) + 200) / 10000.0; // In order to get 2 digits after decimal point
    	int taken = (int)(((float)p_curr_account->Balance()*percent)+0.5f);
    	if(!(p_curr_account->IsVip()))
        {
            p_curr_account->Withdraw(p_curr_account->Password(), taken); // Always successful
            bankBalance+=taken;
            logger->LogMsg(feeMsg,percent*100,taken,p_curr_account->AccountId());
		}
		else
		{
			p_curr_account->Deposit(p_curr_account->Password(), taken); // Always successful
			bankBalance-=taken;
			logger->LogMsg(feeMsg,percent*100,-taken,p_curr_account->AccountId());
		}
		WriteUnlock(accountWriteMutexList[account_id]);
	}
    WriteUnlock(&writeMutex);
}


/* Function Name: PrintStatus
 * This function is called by the LogOp thread. It takes a snapshot of the account status
 * and prints it according to the format specified in the exercise */
void Bank::PrintStatus()
{
	ReadLock(&readMutex, &writeMutex, &readCount);
	for (map<int, Account*>::iterator i = accountList.begin(); i != accountList.end(); ++i)
	{
		WriteLock(accountWriteMutexList[i->first]);
	}
	printf("\033[2J");
	printf("\033[1;1H");
	cout << statusOpenMsg << endl;
	for (map<int, Account*>::iterator i = accountList.begin(); i != accountList.end(); ++i)
	{
		Account* p_curr_account = i->second;
		cout << "Account " << p_curr_account->AccountId() <<": Balance - " << p_curr_account->Balance() <<"$ , Account Password - " << p_curr_account->Password() << endl;
	}

	for (map<int, Account*>::iterator i = accountList.begin(); i != accountList.end(); ++i)
	{
		WriteUnlock(accountWriteMutexList[i->first]);
	}

	cout << "The Bank Has " << bankBalance << "$" << endl;
	ReadUnlock(&readMutex, &writeMutex, &readCount);
}




