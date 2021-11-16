/*
 * slist.cpp
 *
 *  Created on: Feb 16, 2021
 *      Author: OS1
 */
#include "slist.h"
#include "pcb.h"

SignalList::SignalList(){
	first = 0;
	last = 0;
}

SignalList::~SignalList(){
	deleteList();
}

void SignalList::push(Node* nd){
	if (last == 0) {
		last = nd;
		first = last;
		return;
	}
	last->next = nd;
	last = last->next;
}

SignalId SignalList::take(){
	if(first == 0) return 0;
	Node* tmp = first;
	first = first->next;
	if(first == 0)
		last = 0;
	return tmp->id;
}

void SignalList::deleteList() {
	Node* tmp = first;
	Node* prev = 0;
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



