/*
 * thread.h
 *
 *  Created on: Feb 4, 2021
 *      Author: OS1
 */
#ifndef THREAD_H_
#define THREAD_H_


#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;

typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms

typedef void (*SignalHandler)();
typedef unsigned SignalId;
//#include "SCHEDULE.H"
//#include "defines.h"
typedef int ID;
class PCB; // Kernel's implementation of a user's thread


class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);

	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);

	static void blockSignalGlobally(SignalId signal);

	void unblockSignal(SignalId signal);

	static void unblockSignalGlobally(SignalId signal);

	//void sleep(unsigned time);
protected:
	friend class PCB;
 	 Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
 	 virtual void run() {}
private:
 	 PCB* myPCB;
 	 //unsigned changeContext = 0;
};
void dispatch ();
#endif



#endif /* THREAD_H_ */
