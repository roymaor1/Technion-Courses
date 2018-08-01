/******************************************************/
					/* main.cc */
/******************************************************/

// Includes:

#include <pthread.h>
#include "account.h"
#include "atm.h"
#include "bank.h"
#include "logger.h"
#include <errno.h>

/******************************************************/

// Global Variables:
Bank* bank;
Logger *logger;

/******************************************************/

// Threads:

// atmOp thread.
// Gets lines from the file one by one and performs the operation.
void* atmOp(void* p_atm)
{
	Atm *p_curr_atm = (Atm*)(p_atm);
	char line[MAXLINE];

	while (fgets(line, MAXLINE, p_curr_atm->inputFile))
	{
		usleep(SLEEP_BETWEEN_OPS_uS);
		p_curr_atm->Operation(line);
	}

	pthread_exit(NULL);
}

// LogOp thread.
// While the atm threads are running (bank->work variable) this thread prints the status of all accounts
void* LogOp(void*)
{
	while(bank->work)
	{
		bank->PrintStatus();
		usleep(Bank::BANK_PRINT_uS);
	}
	pthread_exit(NULL);
}

// FeesOp thread.
// While the atm threads are running (bank->work variable) this thread charges commission from all accounts
void* FeesOp(void*)
{
	while(bank->work)
	{
		bank->TakeFees();
		sleep(Bank::BANK_FEE);
	}
	pthread_exit(NULL);
}

/******************************************************/

// Main Function:

int main(int argc, char **argv){
	if (argc < 3)
	{
		cout << "illegal arguments" << endl;
		return -ARG_FAIL;
	}
    
	int numOfAtms = atoi(argv[1]);
	int num_of_files = argc - 2;
	if (numOfAtms != num_of_files)
	{
		cerr << "illegal arguments" << endl;
		return -ARG_FAIL;
	}
    bank = new Bank(numOfAtms);
    logger = new Logger();

	pthread_t atm_threads[numOfAtms];
	pthread_t fee_thread;
	pthread_t log_thread;

	// Initiate random seed
	srand(time(NULL));

	// Add all atms
	for (int i=0; i<numOfAtms; i++)
	{
		Atm* p_new_atm = new Atm(i+1, argv[i+2]);
		bank->AddAtm(p_new_atm);
	}

	try
    {
		int ret = pthread_create(&fee_thread, NULL, FeesOp, (void*)NULL);
		if (ret != SUCCESS)
		{
			throw ret;
		}
		ret = pthread_create(&log_thread, NULL, LogOp, (void*)NULL);
		if (ret != SUCCESS)
		{
			throw ret;
		}

		for(int i=0; i<numOfAtms; i++)
		{
			int ret = pthread_create(&atm_threads[i], NULL, atmOp, (void*)(bank->GetAtm(i)));
			if (ret != SUCCESS)
			{
				throw ret;
			}
		}
	}

	catch (int& err)
    {
		cerr << "Error Code: " << err << endl;
		return FAILURE;
	}
	for (int i=0; i<numOfAtms; i++)
    {
		pthread_join(atm_threads[i], NULL);

    }

	bank->work = false;
    
	pthread_join(fee_thread, NULL);
	pthread_join(log_thread, NULL);

	return SUCCESS;
}
