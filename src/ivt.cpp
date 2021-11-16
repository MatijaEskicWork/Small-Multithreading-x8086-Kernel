/*
 * ivt.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: OS1
 */
#include "ivt.h"
#include <dos.h>
#include "kernev.h"

IVTEntry* IVTEntry::ivt[256];


IVTEntry::IVTEntry(IVTNo num, pInterrupt newRoutine){
	entryNum = num;
	oldRoutine = getvect(entryNum);
	setvect(entryNum, newRoutine);
	event = 0;
	ivt[entryNum] = this;
}

IVTEntry::~IVTEntry(){
	setvect(entryNum, oldRoutine);
	ivt[entryNum] = 0;
}

void IVTEntry::callOlder() {
	if (oldRoutine != 0) oldRoutine();
}

void IVTEntry::signal() {
	event->signal();
}
