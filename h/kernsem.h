/*
 * kernsem.h
 *
 *  Created on: Feb 7, 2021
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_
#include "semaphor.h"
#include "pcb.h"
#include "semlist.h"
#include "pcbslls.h"
#include "ivt.h"

class KernelSem {
public:
	KernelSem(int init=1);

	~KernelSem ();

	int wait (Time maxTimeToWait);

	int signal(int n=0);

	int val () const; // Returns the current value of the semaphore, :O

	unsigned getId();

	static SemList* globalSemList;//TREBA TI GLOBALNA LISTA SEMAFORA A NE KERNELSEM ALI MOZDA MOZE I NA TAJ NACIN, VIDECEMO
	PCBSlList* sleepList;
	PCBList* blockedList;

private:
	static int statId;
	unsigned id;
	int value;
};




#endif /* KERNSEM_H_ */
