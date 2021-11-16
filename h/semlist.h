/*
 * semlist.h
 *
 *  Created on: Feb 7, 2021
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_

#include "pcblist.h"
#include "SCHEDULE.H"

class KernelSem;

class SemList {
public:
	struct SNode {
			KernelSem* info;
			SNode* next;
			SNode(KernelSem* _info) {
				info = _info;
				next = 0;
			}
		};
	SNode *last;
	SNode *first;

	SemList();

	~SemList();

	//pushes to end
	void push(KernelSem* el);

	void deleteEl(unsigned id);

	KernelSem* findEl(unsigned id);

	void decreaseWaiting();

	//void doWrapping();

	unsigned getSize();

private:
	unsigned size;

};



#endif /* SEMLIST_H_ */
