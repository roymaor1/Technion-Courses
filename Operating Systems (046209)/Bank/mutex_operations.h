
#ifndef MUTEX_OPERATIONS_H_
#define MUTEX_OPERATIONS_H_

/*************************************************/
			/* mutex_operations.h */
/*************************************************/

// Includes:

#include <pthread.h>

/*************************************************/

// Functions:

// Function Name: LockMutex
// Locks a given mutex and throws an error if locking fails.
void LockMutex(pthread_mutex_t *p_mutex)
{
	if (p_mutex == NULL)
		throw;
	int err = pthread_mutex_lock(p_mutex);
	if (err != SUCCESS)
	{
		throw err;
	}
}

// Function Name: UnlockMutex
// Unlocks a given mutex and throws an error if unlocking fails.
void UnlockMutex(pthread_mutex_t *p_mutex)
{
	if (p_mutex == NULL)
		throw;
	int err = pthread_mutex_unlock(p_mutex);
	if (err != SUCCESS)
	{
		throw err;
	}
}

// Function Name: InitMutex
// Initializes a given mutex and throws an error if initializing fails.
void InitMutex(pthread_mutex_t *p_mutex)
{
	if (p_mutex == NULL)
		throw;
	int err = pthread_mutex_init(p_mutex, NULL);
	if (err != SUCCESS)
	{
		throw err;
	}
}

// Function Name: DestroyMutex
// Destroys a given mutex and throws an error if destroying fails.
void DestroyMutex(pthread_mutex_t *p_mutex)
{
	if (p_mutex == NULL)
		throw;
	int err = pthread_mutex_destroy(p_mutex);
	if (err != SUCCESS)
	{
		throw err;
	}
}

// Function Name: ReadLock
// Performs a read lock according to the readers-writers mechanism
void ReadLock(pthread_mutex_t *p_read_mutex, pthread_mutex_t *p_write_mutex, int *p_read_count)
{
	LockMutex(p_read_mutex);
	(*p_read_count)++;
	if ((*p_read_count) == 1)
	{
		LockMutex(p_write_mutex);
	}
	UnlockMutex(p_read_mutex);
}

// Function Name: ReadUnlock
// Performs a read unlock according to the readers-writers mechanism
void ReadUnlock(pthread_mutex_t *p_read_mutex, pthread_mutex_t *p_write_mutex, int *p_read_count)
{
	LockMutex(p_read_mutex);

	(*p_read_count)--;
	if ((*p_read_count) == 0)
	{
		UnlockMutex(p_write_mutex);
	}
	UnlockMutex(p_read_mutex);
}

// Function Name: WriteLock
// Performs a write lock according to the readers-writers mechanism
void WriteLock(pthread_mutex_t *p_write_mutex)
{
	LockMutex(p_write_mutex);
}

// Function Name: WriteLock
// Performs a write unlock according to the readers-writers mechanism
void WriteUnlock(pthread_mutex_t *p_write_mutex)
{
	UnlockMutex(p_write_mutex);
}



#endif /* MUTEX_OPERATIONS_H_ */
