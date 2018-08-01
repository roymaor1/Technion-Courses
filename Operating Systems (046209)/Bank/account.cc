/*************************************************/
				/* account.cc */
/*************************************************/

// Includes:

#include "account.h"

/*************************************************/

// Functions:

// Function Name: Account
// An Account constructor. Initializes its id, password and initial balance
Account::Account(int account_id, int pass, int initial_balance)
{
	accountId = account_id;
	password = pass;
	balance = initial_balance;
	isVip = false;
}


// Function Name: Withdraw
// Withdraws ammount from the account. If the password doesn't match or the ammount is higher than the balance, an error code is returned.
int Account::Withdraw(int pass, int ammount)
{
	if (pass != password)
	{
		return -WRONG_PASSWORD;
	}

	if (balance - ammount < 0)
	{
		return -WITHDRAW_ERROR;
	}
	balance -= ammount;
	return balance;
}

// Function Name: GetBalance
// returns the balance of the account or error code if password doesn't match
int Account::GetBalance(int pass)
{
	if (pass != password)
	{
		return -WRONG_PASSWORD;
	}
    return balance;
}

// Function Name: Balance
// balance getter (without error code)
int Account::Balance(void)
{
	return balance;
}

// Function Name: Balance
// password getter
int Account::Password(void)
{
	return password;
}

// Function Name: Balance
// accountId getter
int Account::AccountId(void)
{
	return accountId;
}

// Function Name: Balance
// isVip getter
int Account::IsVip(void)
{
	return isVip;
}

// Function Name: Transfer
// Transfers ammount to a given target account. Returns an error code if password is wrong or ammount is larger than balance
int Account::Transfer(int pass, Account* p_target_account, int amount)
{
    if (p_target_account == NULL)
    {
        throw;
    }
    
    if (pass != password)
	{
		return -WRONG_PASSWORD;
	}
    if (balance - amount < 0)
    {
    	return -WITHDRAW_ERROR;
    }
    
    balance -= amount;
    p_target_account->Deposit(p_target_account->Password(), amount);
    return SUCCESS;
}

// Function Name: Deposit
// Deposit ammount to the account. Returns an error code if password is wrong
int Account::Deposit(int pass, int ammount)
{
	if (pass != password)
	{
		return -WRONG_PASSWORD;
	}
	balance += ammount;
    return balance;
}

// Function Name: MakeVip
// Makes the account a VIP account. Returns an error code if password is wrong.
int Account::MakeVip(int pass)
{
	if (pass != password)
	{
		return -WRONG_PASSWORD;
	}
	isVip = true;
	return SUCCESS;
}
