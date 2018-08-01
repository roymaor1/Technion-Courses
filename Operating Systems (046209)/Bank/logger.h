#ifndef __LOGGER_H__
#define __LOGGER_H__

/*************************************************/
/*						logger.h				 */
/*************************************************/

// Includes:
#include <pthread.h>
#include <iostream>
#include <string>
#include <cstdarg>
#include "account.h"

using namespace std;
/*************************************************/

// Constants:


#define MAXLINE 100
/*************************************************/

class Logger
{
	public:
	Logger();
	~Logger();




	void LogMsg(const char* str, ...);
    FILE* logFile;
    pthread_mutex_t logMutex;
};





#endif //__BANK_H__

