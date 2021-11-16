/*
 * siglist.cpp
 *
 *  Created on: Feb 16, 2021
 *      Author: OS1
 */

#include "siglist.h"
#include "pcb.h"

SignalHandlerList::SignalHandlerList(){
	first = 0;
	last = 0;
}

SignalHandlerList::~SignalHandlerList(){
	deleteList();
}

void SignalHandlerList::push(Signal* s) {
	if (first == 0) {
		first = s;
		last = first;
		return;
	}
	last->next = s;
	last = last->next;

}

SignalHandler SignalHandlerList::pop() {
	return 0;
}

void SignalHandlerList::deleteList() {
	Signal* tmp = first;
	Signal* prev = 0;
	while (tmp != 0) {
		prev = tmp;
		tmp = tmp->next;
		delete prev;
		prev = 0;
	}
	delete last;
	delete first;
	last = 0;
	first = 0;
}

void SignalHandlerList::swap(SignalHandler h1, SignalHandler h2){
	Signal* x1 = 0;
	Signal* x2 = 0;
	Signal* tmp = first;
	while (tmp != 0) {
		if (tmp->handler == h1) x1 = tmp;
		if (tmp->handler == h2) x2 = tmp;
		tmp = tmp->next;
	}
	if (x1 != 0 && x2 != 0) {
		x1->handler = h2;
		x2->handler = h1;
	}
}

