/*
 * pcblist.cpp
 *
 *  Created on: Feb 4, 2021
 *      Author: OS1
 */
//#include "pcblist.h"
#include "pcb.h"
#include "SCHEDULE.H"

PCBList::PCBList() {
	last = 0;
	first = 0;
	size = 0;
}

PCBList::~PCBList(){
	PNode *tmp = first;
	PNode *prev = 0;
	while (tmp != 0) {
		prev = tmp;
		tmp = tmp->next;
		delete prev;
		prev = 0;
	}
	delete first;
	delete last;
	first = 0;
	last = 0;
}

//pushes to end
void PCBList::push(PCB* el){
	if (last == 0) {
		last = new PNode(el);
		first = last;
		return;
	}
	last->next = new PNode(el);
	last = last->next;
	size++;
}

PCB* PCBList::findEl(ID id){
	PNode *tmp = first;
	while (tmp != 0 && tmp->info->getThread()->getId() != id) {
			tmp = tmp->next;
	}
	if(tmp==0)
		return 0;
	return tmp->info;
}

//little help for little Eskic
//kako si uspeo ovo odraditiiii???
PCB* PCBList::take(){
	if(first == 0) return 0;
	PNode* tmp = first;
	first = first->next;
	if(first == 0)
		last = 0;
	return tmp->info;
}

void PCBList::deleteEl(ID id){//MNOGO SI ZAKOMPLIKOVAO ALI MAKAR SE NECE POKLOPII SA DRUGIMA
							  // ZNAM
	PNode *prev = 0;
	PNode *tmp = first;
	if (first == 0) return;
	while (tmp != 0 && tmp->info->getId() != id) {
		prev = tmp;
		tmp = tmp->next;
	}
	if (size == 1) {
		last = 0;
		first = 0;
		size = 0;
		return;
	}
	if (prev == 0) {
		first = tmp->next;
		//delete tmp;
		tmp = 0;
		return;
	}
	prev->next = tmp->next;
	if (tmp == last) last = prev;
	//delete tmp;
	tmp = 0;
	size--;
}

void PCBList::doWrapping() {
	PNode * nd = first;
	while (nd != 0) {
		nd->info->state = PCB::READY;
		Scheduler::put(nd->info);
		nd=nd->next;
	}
}

unsigned PCBList::getSize(){
	return size;
}
