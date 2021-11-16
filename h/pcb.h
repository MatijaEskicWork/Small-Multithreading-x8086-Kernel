/*
 * pcb.h
 *
 *  Created on: Feb 4, 2021
 *      Author: OS1
 */
#ifndef PCB_H_
#define PCB_H_
//#include "defines.h"
#include "thread.h"
#include "pcblist.h"
#include "defines.h"
#include "siglist.h"
#include "slist.h"
//#include "SCHEDULE.h"



class PCB {

public:
	PCB(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice, Thread* thr = 0);

	~PCB();

	enum State {NEW, READY, BLOCKED, FINISHED};

	unsigned getSS();
	unsigned getSP();
	unsigned getBP();
	ID getId();

	void setSS(unsigned _ss);
	void setSP(unsigned _sp);
	void setBP(unsigned _bp);

	void start();

	static void wrapper();

	Thread* getThread();

	void waitToComplete();

	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);

	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);

	static void blockSignalGlobally(SignalId signal);

	void unblockSignal(SignalId signal);

	static void unblockSignalGlobally(SignalId signal);

	void defaultSignal();

	void processHandlers();

	unsigned counter;
	static PCB* running;
	static PCB* stupidPCB;
	static PCBList *globalList;
	PCBList* waitingList;
	State state;
	PCB* parent;
	unsigned signalx;
	SignalId localBlocked[16];
	static SignalId globallyBlocked[16];
	SignalHandlerList* handlers[16];
	SignalList* signals;

	//TREBACE OVDE JOS DOSTA DA SE RADI

private:
	unsigned *stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned timeSlice;
	ID id;
	static ID statID;
	Thread* myThread;

};

void globalDefaultSignal();

#endif /* PCB_H_ */
