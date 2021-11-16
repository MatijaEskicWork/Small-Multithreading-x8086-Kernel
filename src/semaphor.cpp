/*
 * semaphor.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: OS1
 */

#include "semaphor.h"
#include "kernsem.h"

Semaphore::Semaphore (int init) {
	myImpl = new KernelSem(init);
}


Semaphore::~Semaphore () {
	if (myImpl != 0) delete myImpl;
	myImpl = 0;
}

int Semaphore::wait (Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::val() const {
	return myImpl->val();
}// Returns the current value of the semaphore. NEMOJ ZEZATI DA TO RADI :O


