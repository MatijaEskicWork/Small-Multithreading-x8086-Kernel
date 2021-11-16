/*
 * pcblist.h
 *
 *  Created on: Feb 4, 2021
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_
#include "thread.h"
class PCB;
//class Thread;

class PCBList{
public:
	struct PNode {
			PCB* info;
			PNode* next;
			PNode(PCB* _info) {
				info = _info;
				next = 0;
			}
		};
	PNode *last;
	PNode *first;
	PCBList();

	~PCBList();

	//pushes to end
	void push(PCB* el);

	void deleteEl(ID id);

	PCB* findEl(ID id);

	void doWrapping();

	unsigned getSize();

	PCB* take();

private:
	unsigned size;


};




#endif /* PCBLIST_H_ */
