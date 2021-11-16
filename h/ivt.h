/*
 * ivt.h
 *
 *  Created on: Feb 12, 2021
 *      Author: OS1
 */

#ifndef IVT_H_
#define IVT_H_


#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
	newEntry##numEntry.signal();\
	if(callOld == 1)\
		newEntry##numEntry.callOlder();\
}

#include "event.h"

typedef void interrupt (*pInterrupt)(...);

class KernelEv;

class IVTEntry {
public:
	IVTEntry(IVTNo num, pInterrupt newRoutine);

	~IVTEntry();

	void callOlder();

	void signal();

	static IVTEntry* ivt[256];
	KernelEv *event;
	pInterrupt oldRoutine;
	IVTNo entryNum;


};



#endif /* IVT_H_ */
