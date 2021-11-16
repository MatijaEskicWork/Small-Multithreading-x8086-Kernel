/*
 * pcb.cpp
 *
 *  Created on: Feb 4, 2021
 *      Author: OS1
 */
#include "pcb.h"
//#include "pcblist.h"
#include "SCHEDULE.h"
#include <iostream.h>
#include <stdio.h>
ID PCB::statID = 0;
PCBList* PCB::globalList = new PCBList();
PCB* PCB::running = new PCB(4096,2);
SignalId PCB::globallyBlocked[16];

void PCB::defaultSignal() {
	//lock()
	/*if (PCB::running->parent != 0)*/ //PCB::running->parent->signal(1);
	/*PCB::running->*/state = PCB::FINISHED;
	//delete waitingList;
	/*PCB::running->*/waitingList->doWrapping();
	//waitingList = 0;
	//unlock()
	dispatch();
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thr) {
	signalx = 0;
	myThread = thr;
	counter = timeSlice;
	id = statID++;
	if(id==0)
		parent=0;
	else{
		parent=PCB::running;
	}
	stackSize /= sizeof(unsigned);
	stack = new unsigned[stackSize];
	state = NEW;
	waitingList = new PCBList();
	handlers[0] = new SignalHandlerList();
	SignalHandlerList::Signal* hand = new SignalHandlerList::Signal(globalDefaultSignal);
	handlers[0]->push(hand);
	//parent = PCB::running;
	int i = 0;
	signals = new SignalList();
	if (parent != 0) {
		//cout << "Stize ovde1\n";
		SignalList::Node* nd = parent->signals->first;
		while (nd != 0) {
			signals->push(new SignalList::Node(nd->id));
			nd = nd->next;
		}
	}
	for (; i < 16; i++) {
		//cout << "Stize ovde2\n";
		localBlocked[i] = 0;
		if (i != 0) {
			//cout << "Stize ovde3\n";
			handlers[i] = new SignalHandlerList();
			if (parent != 0) {
				//cout << "Stize ovde 10\n";
				SignalHandlerList::Signal* nd = parent->handlers[i]->first;
				while (nd != 0) {
					handlers[i]->push(new SignalHandlerList::Signal(nd->handler));
					nd = nd->next;
				}
			}
		}
		if (parent == 0) continue;
		//cout << "Stize ovde 11\n";
		localBlocked[i] = parent->localBlocked[i];
	}
	//cout << "Stize ovde5\n";
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 1] = 0x200;
	stack[stackSize - 2] = FP_SEG(PCB::wrapper);
	stack[stackSize - 3] = FP_OFF(PCB::wrapper);
	ss = FP_SEG(stack + stackSize - 12);
	sp = FP_OFF(stack + stackSize - 12);
#endif
	bp = sp;
	PCB::globalList->push(this);
	if (id == 0) {
		state = READY;
	}
	//cout << "Stize ovde6\n";
}

ID PCB::getId() {
	return id;//IQ 200 :O
}

unsigned PCB::getSS() {
	return ss;
}
unsigned PCB::getSP(){
	return sp;
}
unsigned PCB::getBP(){
	return bp;
}

void PCB::setSP(unsigned _sp){
	sp = _sp;//NE TREBA
}

void PCB::setSS(unsigned xss){
	ss = xss;//NE TREBA
}

void PCB::setBP(unsigned _bp){
	bp = _bp;//NE TREBA
}

void PCB::start() {
	//lock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
	state = READY;
	Scheduler::put(this);
	//unlock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
}

Thread* PCB::getThread() {
	return myThread;
}

PCB::~PCB() {
	delete waitingList;
	if (stack != 0) delete stack;
	waitingList = 0;
	int i = 0;
	for (; i < 16; i++) delete handlers[i];
	stack = 0;
	PCB::globalList->deleteEl(id);
}

void PCB::waitToComplete() {
	//lock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
	if (this != PCB::running && state != FINISHED) {
		PCB::running->state = BLOCKED;
		waitingList->push(PCB::running);
		dispatch();
	}
	//unlock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE

}

void PCB::wrapper() {
	PCB::running->getThread()->run();
	//lock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
	if (PCB::running->parent != 0) PCB::running->parent->signal(1);
	PCB::running->state = FINISHED;
	PCB::running->waitingList->doWrapping();
	PCB::running->waitingList = 0;
	PCB::running->signal(2);
	/*if (PCB::globallyBlocked[2] == 0 && PCB::running->localBlocked[2] == 0) {
		SignalHandlerList::Signal * sig = PCB::running->handlers[2]->first;
		while (sig != 0) {
			sig->handler();
			sig = sig->next;
		}
	}*/
	//unlock()
	dispatch();//OVO TI JE GLUPOST, LOCK-OVAO SI PA POZVAO PROMENU KONTEKSTA, I POSLE SE VRACAS U NOVU NIT A JOS UVEK JE BIT I=1!!!
}

void PCB::signal(SignalId signal){
	//cout printf("Signal\n");
	if (state != PCB::FINISHED)
		signals->push(new SignalList::Node(signal));
}

void PCB::registerHandler(SignalId signal, SignalHandler handler){
	//cout << "Register " << signal;
	SignalHandlerList::Signal *nd = new SignalHandlerList::Signal(handler);
	handlers[signal]->push(nd);
}

void PCB::unregisterAllHandlers(SignalId id){
	handlers[id]->deleteList();
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	handlers[id]->swap(hand1, hand2);
}

void PCB::blockSignal(SignalId signal){
	//printf("Blocked %d\n", signal);
	//if (state != PCB::FINISHED && signal > 2)
		localBlocked[signal] = 1;
}

void PCB::blockSignalGlobally(SignalId signal){
	//if (signal > 2)
		PCB::globallyBlocked[signal] = 1;
}

void PCB::unblockSignal(SignalId signal){
	//if (state != PCB::FINISHED && signal > 2)
		localBlocked[signal] = 0;
}

void PCB::unblockSignalGlobally(SignalId signal){
	//if (signal > 2)
		PCB::globallyBlocked[signal] = 0;
}

void PCB::processHandlers() {
	SignalList::Node* tmp = signals->first;
	//cout << "Stize ovde1000\n";
	while (tmp != 0) {
		if (tmp->id == 0) {
			int i = 1;
			for (;i < 16; i++) {
					/*PCB::running->*/handlers[i]->deleteList();
			}
			SignalHandlerList::Signal *tmp1 = /*PCB::running->*/handlers[0]->first;
			while (tmp1 != 0) {
				tmp1->handler();
				tmp1 = tmp1->next;
			}
			signals->deleteList();
			break;
		}
		else {
			if (PCB::globallyBlocked[tmp->id] == 0 && /*PCB::running->*/localBlocked[tmp->id] == 0) {
				SignalHandlerList::Signal *tmp1 = /*PCB::running->*/handlers[tmp->id]->first;
				while (tmp1 != 0) {
					tmp1->handler();
					tmp1 = tmp1->next;
				}
			}
		}
		tmp = tmp->next;
	}
}

void globalDefaultSignal() {
	PCB::running->defaultSignal();
}
