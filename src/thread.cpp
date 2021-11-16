/*
 * thread.cpp
 *
 *  Created on: Feb 4, 2021
 *      Author: OS1
 */
#include "pcb.h"
//#include "thread.h"

Thread::Thread (StackSize stackSize, Time timeSlice){
	myPCB = new PCB(stackSize, timeSlice, this);
	//treba je smestiti u globalnu listu pcbova
}

void Thread::start() {
	lock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
	myPCB->start();
	unlock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
}

void Thread::waitToComplete(){
	//treba mi lista svih pcbova koji cekaju na zavrsetak niti u cijem se
	//objektu nalaze
	lock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
	myPCB->waitToComplete();
	unlock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
}

Thread::~Thread() {
	//lock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
	if (myPCB != 0) {
		delete myPCB;
	}
	myPCB = 0;
	//unlock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
	//obrisati iz niza i staviti na 0
}

ID Thread::getId() {
	//lock()
	ID ret = myPCB->getId();
	//unlock()
	return ret;
}

ID Thread::getRunningId(){
	//lock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
	ID ret = PCB::running->getId();
	//unlock()//MNOGO LOCK-OVA I UNLOCK-OVA IMAS, SJEBACE TI SEMAFORE
	return ret;

}

Thread * Thread::getThreadById(ID id){
	//lock()
	Thread * thr = PCB::globalList->findEl(id)->getThread();
	//unlock()
	return thr;
}

void Thread::signal(SignalId signal) {
	myPCB->signal(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler){
	myPCB->registerHandler(signal, handler);
}

void Thread::unregisterAllHandlers(SignalId id){
	myPCB->unregisterAllHandlers(id);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	myPCB->swap(id, hand1, hand2);
}

void Thread::blockSignal(SignalId signal){
	myPCB->blockSignal(signal);
}

void Thread::blockSignalGlobally(SignalId signal){
	PCB::blockSignalGlobally(signal);
}

void Thread::unblockSignal(SignalId signal){
	myPCB->unblockSignal(signal);
}

void Thread::unblockSignalGlobally(SignalId signal){
	PCB::unblockSignalGlobally(signal);
}

//BESPOTREBNO OVDE, TO U SEMAFORU RADIS SA STATIC LISTOM SVIH SEMAFORA :)
//void Thread::sleep(unsigned time) {
	//ubacujem u sleep listu,
//}

