#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

/*************************************************/
/*					account.h				 	 */
/*************************************************/

// Includes:
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;
/*************************************************/

// Constants:

typedef enum RET_CODE
{
	SUCCESS,
	WRONG_PASSWORD,
    ACCOUNT_NOT_FOUND,
	TARGET_ACCOUNT_NOT_FOUND,
	WITHDRAW_ERROR,
	ACCOUNT_EXISTS,
	ARG_FAIL,
} RET_CODE;

#define FAILURE -1




/*************************************************/


// Account class
class Account
{
	public:

	Account(int account_num, int pass, int initial_balance);

	static const int OP_SLEEP = 1;

	int Withdraw(int pass, int ammount);
	int Deposit(int pass, int ammount);
	int MakeVip(int pass);
	int Transfer(int pass, Account* p_target_account, int amount);
	int GetBalance(int pass);

	// Getters and Setters
	int Password(void);
	int Balance(void);
	int AccountId(void);
	int IsVip(void);
	private:

	int accountId;
	int password;
	int balance;
	bool isVip;

};




#endif //__ACCOUNT_H_
