/*
 *	Author: Diego Ochando Torres
 *  Date: 18/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

 // ------------------------------------------------------------------------- //

#include <winsock2.h>
#include <stdio.h>

// ------------------------------------------------------------------------- //

#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

class NetworkManager {
public:
	NetworkManager();
	~NetworkManager();

	int initWinsock();

	void closeWinsock();

};


static SRWLOCK mutex;

static void lockMutex(void) {

	AcquireSRWLockExclusive(&mutex);

}

static void unlockMutex(void) {

	ReleaseSRWLockExclusive(&mutex);

}


// ------------------------------------------------------------------------- //

#endif // __NETWORK_MANAGER_H__
