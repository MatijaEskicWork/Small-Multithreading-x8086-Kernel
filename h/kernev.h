/*
 * kernev.h
 *
 *  Created on: Feb 11, 2021
 *      Author: OS1
 */

#ifndef KERNEV_H_
#define KERNEV_H_
#include "event.h"
#include "pcb.h"

class KernelEv {
public:
	KernelEv(IVTNo no, Event* ev);

	~KernelEv();

	void wait();

	void signal();

private:
	Event* event;
	IVTNo num;
	PCB* thread;

};



#endif /* KERNEV_H_ */
