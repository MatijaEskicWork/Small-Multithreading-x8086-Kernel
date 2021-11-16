/*
 * siglist.h
 *
 *  Created on: Feb 16, 2021
 *      Author: OS1
 */

#ifndef SIGLIST_H_
#define SIGLIST_H_

class PCB;
#include "thread.h"

class SignalHandlerList {
public:
	struct Signal{
		SignalHandler handler;
		Signal* next;
		Signal(SignalHandler _handler) {
					handler = _handler;
					next = 0;
		}
	};

	SignalHandlerList();

	~SignalHandlerList();

	void push(Signal *s);

	SignalHandler pop();

	void deleteList();

	void swap(SignalHandler h1, SignalHandler h2);

	Signal* first;
	Signal* last;
	unsigned globalFlag;
	unsigned localFlag;
	//friend class PCB;
};



#endif /* SIGLIST_H_ */
