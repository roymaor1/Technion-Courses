/******************************************************/
					/* atm.cc */
/******************************************************/

// Includes:

#include "atm.h"
#include "bank.h"
#include <errno.h>
extern Bank* bank;
extern Logger *logger;

/******************************************************/

// Constants:

static const char* accountExists = "Error %d: Your transaction failed – account with the same id exists\n";
static const char* openSuccess = "%d: New account id is %d with password %d and initial balance %d\n";
static const char* wrongPass = "Error %d: Your transaction failed – password for account id %d is incorrect\n";
static const char* accountNotFound = "Error %d: Your transaction failed – account id %d does not exist\n";
static const char* depositSuccess = "%d: Account %d new balance is %d after %d $ was deposited\n";
static const char* withdrawOvershoot = "Error %d: Your transaction failed – account id %d balance is lower than %d\n";
static const char* withdrawSuccess = "%d: Account %d new balance is %d after %d $ was withdrew\n";
static const char* getBalanceSuccess = "%d: Account %d balance is %d\n";
static const char* transferSuccess = "%d: Transfer %d from account %d to account %d new account balance is %d new target account balance is %d\n";

/******************************************************/

// Functions:

// Function Name: Atm
// Atm constructor. gets an atm id and an input file name and initiates the corresponding members
Atm::Atm(int atm_id, char* input_file_name)
{
	atmId = atm_id;
	inputFile = fopen(input_file_name, "r");
	if (inputFile == NULL)
	{
		throw errno;
	}
	bank->AddAtm(this);
}

// Function Name: Operation
// This function gets a line from the atm file (string) and performs the operation it describes.
void Atm::Operation(char operation[MAXLINE])
{
	char* p_end;
	int args[MAX_ARG];
	args[0] = strtol(&operation[2], &p_end, 10);
	args[1] = strtol(p_end, &p_end, 10);
	args[2] = strtol(p_end, &p_end, 10);
	args[3] = strtol(p_end, &p_end, 10);
	int account_num = args[0];
	int pass = args[1];
	char op_letter = operation[0];
	switch (op_letter)
	{
		case 'O':
		{
			int amount = args[2];
			int ret = OpenAccount(account_num, pass, amount);
			switch (ret)
			{
				case SUCCESS:
				{
					logger->LogMsg(openSuccess, atmId, account_num, pass, amount);
					break;
				}
				case -ACCOUNT_EXISTS:
				{
					logger->LogMsg(accountExists, atmId);
					break;
				}
			}
			break;
		}
		case 'L':
		{
			int ret = MakeVip(account_num, pass);
			switch (ret)
			{
                case -ACCOUNT_NOT_FOUND:
                {
                	logger->LogMsg(accountNotFound, atmId, account_num);
                    break;
                }
                
                case -WRONG_PASSWORD:
                {
                	logger->LogMsg(wrongPass, atmId, account_num);
                    break;
                }
			}
			break;
		}
		case 'D':
		{
			int amount = args[2];
			int ret = Deposit(account_num, pass, amount);
			if (ret > 0)
			{
				logger->LogMsg(depositSuccess, atmId, account_num, ret, amount);
				break;
			}
			switch (ret)
			{
                case -ACCOUNT_NOT_FOUND:
                {
                	logger->LogMsg(accountNotFound, atmId, account_num);
                    break;
                }
                
				case -WRONG_PASSWORD:
				{
					logger->LogMsg(wrongPass, atmId, account_num);
					break;
				}
			}
			break;
		}
		case 'W':
		{
			int amount = args[2];
			int ret = Withdraw(account_num, pass, amount);
			if (ret > 0)
			{
				logger->LogMsg(withdrawSuccess, atmId, account_num, ret, amount);
				break;
			}
			switch (ret)
			{
                case -ACCOUNT_NOT_FOUND:
                {
                	logger->LogMsg(accountNotFound, atmId, account_num);
                    break;
                }
                
				case -WRONG_PASSWORD:
				{
					logger->LogMsg(wrongPass, atmId, account_num);
					break;
				}
				case -WITHDRAW_ERROR:
				{
					logger->LogMsg(withdrawOvershoot, atmId, account_num, amount);
					break;
				}
			}
			break;
		}
		case 'B':
		{
			int ret = GetBalance(account_num, pass);
			if (ret >= 0)
			{
				logger->LogMsg(getBalanceSuccess, atmId, account_num, ret);
				break;
			}
			switch (ret)
			{
                case -ACCOUNT_NOT_FOUND:
                {
                	logger->LogMsg(accountNotFound, atmId, account_num);
                    break;
                }
                
                case -WRONG_PASSWORD:
                {
                	logger->LogMsg(wrongPass, atmId, account_num);
                    break;
                }
                
			}
			break;
		}
		case 'T':
		{
			int target_account_num = args[2];
			int amount = args[3];
			int *p_source_balance = new int;
			(*p_source_balance) = 0;
			int *p_target_balance = new int;
			(*p_target_balance) = 0;
			int ret = Transfer(account_num, pass, target_account_num, amount, p_source_balance, p_target_balance);
            switch (ret)
			{
                case SUCCESS:
                {
                    logger->LogMsg(transferSuccess, atmId, amount, account_num, target_account_num, *p_source_balance, *p_target_balance);
                    break;
                }
                
                case -ACCOUNT_NOT_FOUND:
                {
                	logger->LogMsg(accountNotFound, atmId, account_num);
                    break;
                }
                
                case -TARGET_ACCOUNT_NOT_FOUND:
                {
                	logger->LogMsg(accountNotFound, atmId, target_account_num);
                	break;
                }

                case -WRONG_PASSWORD:
                {
                	logger->LogMsg(wrongPass, atmId, account_num);
                    break;
                }
                
                case -WITHDRAW_ERROR:
                {
                	logger->LogMsg(withdrawOvershoot, atmId, account_num, amount);
					break;
                }
                
			}
			break;
		}
	}

}


// Function Name: OpenAccount
// This function calls the bank function to add a new account to the bank.
// It also returns an error code if the operation failed
int Atm::OpenAccount(int account_id, int password, int initial_amount)
{
	return bank->AddAccount(account_id, password, initial_amount);
}

// Function Name: MakeVip
// This function makes the account whose id is account_id a VIP account.
// It also returns an error code if the operation failed
int Atm::MakeVip(int account_id, int password)
{
	Account* p_account = bank->GetAccount(account_id, WRITE);
	if (p_account == NULL)
	{
		return -ACCOUNT_NOT_FOUND;
	}
	int ret = p_account->MakeVip(password);
    sleep(ATM_SLEEP);
    bank->ReturnAccount(account_id, WRITE);
    return ret;
}

// Function Name: Deposit
// This function deposits amount to the account which id is account_id
// It also returns an error code if the operation failed
int Atm::Deposit(int account_id, int pass, int amount)
{
	Account* p_account = bank->GetAccount(account_id, WRITE);
	if (p_account == NULL)
	{
		return -ACCOUNT_NOT_FOUND;
	}
	int ret = p_account->Deposit(pass, amount);
    sleep(ATM_SLEEP);
    bank->ReturnAccount(account_id, WRITE);
    return ret;
}

// Function Name: Withdraw
// This function withdraws amount from the account which id is account_id
// It also returns an error code if the operation failed
int Atm::Withdraw(int account_id, int pass, int amount)
{
	Account* p_account = bank->GetAccount(account_id, WRITE);
	if (p_account == NULL)
	{
		return -ACCOUNT_NOT_FOUND;
	}
	int ret = p_account->Withdraw(pass, amount);
    sleep(ATM_SLEEP);
    bank->ReturnAccount(account_id, WRITE);
    return ret;
}

// Function Name: GetBalance
// This function returns the current balance of the account which id is account_id.
// It also returns an error code if the operation failed
int Atm::GetBalance(int account_id, int pass)
{
	Account* p_account = bank->GetAccount(account_id, READ);
	if (p_account == NULL)
	{
		return -ACCOUNT_NOT_FOUND;
	}
	int ret = p_account->GetBalance(pass);
	sleep(ATM_SLEEP);
	bank->ReturnAccount(account_id, READ);
	return ret;
}

// Function Name: Transfer
// This function transfers amount from source account to target account
// It also returns an error code if the operation failed
int Atm::Transfer(int source_account_id, int source_pass, int target_account_id, int amount, int *p_source_balance, int* p_target_balance)
{
	Account *p_source_account;
	Account *p_target_account;
	if ( source_account_id < target_account_id )
	{
		p_source_account = bank->GetAccount(source_account_id, WRITE);
		if (p_source_account == NULL)
		{
			return -ACCOUNT_NOT_FOUND;
		}
		p_target_account = bank->GetAccount(target_account_id, WRITE);
		if (p_target_account == NULL)
		{
			bank->ReturnAccount(source_account_id, WRITE);
			return -TARGET_ACCOUNT_NOT_FOUND;
		}
	}
	else
	{
		p_target_account = bank->GetAccount(target_account_id, WRITE);
		if (p_target_account == NULL)
		{
			return -TARGET_ACCOUNT_NOT_FOUND;
		}
		p_source_account = bank->GetAccount(source_account_id, WRITE);
		if (p_source_account == NULL)
		{
			bank->ReturnAccount(target_account_id, WRITE);
			return -ACCOUNT_NOT_FOUND;
		}
	}


	int ret = p_source_account->Transfer(source_pass, p_target_account, amount);
	(*p_source_balance) = p_source_account->Balance();
	(*p_target_balance) = p_target_account->Balance();
    sleep(ATM_SLEEP);

    if ( source_account_id < target_account_id )
	{
        bank->ReturnAccount(source_account_id, WRITE);
        bank->ReturnAccount(target_account_id, WRITE);
	}
	else
	{
		bank->ReturnAccount(target_account_id, WRITE);
		bank->ReturnAccount(source_account_id, WRITE);
	}
    return ret;
}


