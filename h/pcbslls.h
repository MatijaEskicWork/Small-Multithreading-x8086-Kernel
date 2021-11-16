/*
 * pcbslls.h
 *
 *  Created on: Feb 11, 2021
 *      Author: OS1
 */

#ifndef PCBSLLS_H_
#define PCBSLLS_H_
//#include "pcblist.h"
class PCB;

class PCBSlList{
public :
	struct SlNode {
		SlNode (PCB* pcb, int _t) {
			info = pcb;
			next = 0;
			t = _t;
		}
		PCB* info;
		SlNode *next;
		int t;
	};

	PCBSlList();

	~PCBSlList();

	//pushes to end
	void push(PCB* el, int time);

	//void deleteEl(ID id);

	void decrease();

	PCB* take();

	SlNode *last;
	SlNode *first;

};


#endif /* PCBSLLS_H_ */
