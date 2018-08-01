/*************************************************/
/*					logger.cc				 	 */
/*************************************************/

// Includes:

#include "logger.h"
#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>

/*************************************************/

// Functions:

// Function Name: Logger
// A Logger constructor. Opens the log file to writing and initializes an access mutex to the log.
Logger::Logger()
{
    logFile = fopen("log.txt", "w");
    if(logFile == NULL)
		throw;
    int ret = pthread_mutex_init(&logMutex, NULL);
    if (ret != 0)
	{
    	throw ret;
	}
}


// Function Name: Logger
// A Logger destructor. Closes the log file and destroys the mutex.
Logger::~Logger()
{
    if (fclose(logFile) != SUCCESS)
        throw;
    int ret = pthread_mutex_destroy(&logMutex);
    if (ret != 0)
	{
    	throw ret;
	}
}
// Function Name: LogMsg
// This function writes the string given as argument to the log file
// (additional arguments may be given as well to put in format string in str).
void Logger::LogMsg(const char* str, ...)
{
	int ret = pthread_mutex_lock(&logMutex);
	if (ret != 0)
	{
		throw ret;
	}
    va_list args;
    va_start(args, str);
    vfprintf (logFile, str, args);
    va_end(args);
    ret = pthread_mutex_unlock(&logMutex);
	if (ret != 0)
	{
		throw ret;
	}
}
