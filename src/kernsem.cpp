/*
 * kernsem.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: OS1
 */
#include "kernsem.h"
#include "SCHEDULE.H"

SemList* KernelSem::globalSemList = new SemList();
int KernelSem::statId = 0;

KernelSem::KernelSem(int init){
	globalSemList->push(this);
	value = init;
	id = statId++;
	blockedList = new PCBList();
	sleepList = new PCBSlList();
}

KernelSem::~KernelSem (){
	value = 0;
	globalSemList->deleteEl(id);
	delete sleepList;
	delete blockedList;
}

int KernelSem::wait (Time maxTimeToWait){//Jel drzis casove? Duhovitooo
	//syncPrintf("WAIT %d\n", maxTimeToWait);
	//if (maxTimeToWait % 55 == 0) maxTimeToWait /= 55;
	int retVal = 1;
	if(value-- <= 0) {
		if (maxTimeToWait == 0) {
			//PCB::running->blockedInfinite = 1;
			PCB::running->state = PCB::BLOCKED;
			blockedList->push(PCB::running);
		}
		else {
			//PCB::running->blockTime = maxTimeToWait;
			PCB::running->state = PCB::BLOCKED;
			sleepList->push(PCB::running, maxTimeToWait);
		}
		dispatch();
		if (maxTimeToWait != 0) {
			retVal = PCB::running->signalx;
		}
		PCB::running->signalx = 0;
	}
	return retVal;
}

int KernelSem::signal(int n){
	//syncPrintf("SIGNAL %d\n", n);
	//little help for little Eskic
	if(n < 0)
		return n;
	if(n == 0){//Standardni nacin
		if(value++ < 0){
			PCB* tmp = blockedList->take();
			if(tmp == 0) {
				//return 0;
				tmp = sleepList->take();
				if (tmp == 0) return 0;
				tmp->state = PCB::READY;
				tmp->signalx = 1;
				Scheduler::put(tmp);
				return 0;//Zato sto uvek vraca 0
			}
			tmp->state = PCB::READY;
			tmp->signalx = 1;
			Scheduler::put(tmp);
			return 0;//Posto je value>0 i n==0 tj standardan nacin, odblokira samo 1
		}
		return 0;//Vraca broj odblokiranih niti, ovde nije odblokirao jer !(value>0)
	}
	else{
		int i = 0;
		PCB* tmp = 0;
		if (value < 0) {
			value += n;
			for (i = 0;i < n; i++) {
				tmp = blockedList->take();
				if (tmp == 0) {
					for (int j = i; j < n; j++) {
						tmp = sleepList->take();
						if (tmp == 0) return j;
						tmp->state = PCB::READY;
						tmp->signalx = 1;
						Scheduler::put(tmp);
					}
				}
				tmp->state = PCB::READY;
				tmp->signalx = 1;
				Scheduler::put(tmp);
			}
			return n;
		}
		else {
			value += n;
			return n;
		}
	}

	return 0;
}

int KernelSem::val () const {
	return value;
}

unsigned KernelSem::getId() {
	return id;
}

