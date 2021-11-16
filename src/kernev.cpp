/*
 * kernev.cpp
 *
 *  Created on: Feb 11, 2021
 *      Author: OS1
 */
#include "kernev.h"
#include "SCHEDULE.H"
#include "ivt.h"

KernelEv::KernelEv(IVTNo no, Event* ev){
	num = no;
	event = ev;
	thread = PCB::running;
	IVTEntry::ivt[num]->event = this;
}

KernelEv::~KernelEv(){
	thread = 0;
}

void KernelEv::wait(){
	if (thread == PCB::running) {
		num = 0;
		thread->state = PCB::BLOCKED;
		dispatch();
	}
}

void KernelEv::signal(){
	if (num == 0) {
		num = 1;
		thread->state = PCB::READY;
		Scheduler::put(thread);
	}
}

