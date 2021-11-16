/*
 * pcbslls.cpp
 *
 *  Created on: Feb 11, 2021
 *      Author: OS1
 */
#include "pcbslls.h"
#include "pcb.h"
#include "SCHEDULE.H"
PCBSlList::PCBSlList(){
	first = 0;
	last = 0;
}

PCBSlList::~PCBSlList(){
	SlNode *tmp = first;
	SlNode *prev = 0;
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

void PCBSlList::push(PCB* el, int time){
	//ubacivanje u praznu listu
	if (first == 0) {
		last = new SlNode(el, time);
		first = last;
		return;
	}
	SlNode * newNode = new SlNode(el, time);
	SlNode * tmp = first;
	SlNode * prev = 0;
	while (tmp != 0) {
		//ubacivanje klasicno (treba updatovati sledbenika tmp-a i sebe
		if (tmp->t >= time) {
			//ubacivanje na pocetak
			if (prev == 0) {
				newNode->next = first;
				newNode->t = time;
				first->t = first->t - time;
				first = newNode;
				return;
			}
			//ubacivanje negde
			else {
				newNode->next = tmp;
				newNode->t = time;
				prev->next = newNode;
				//prev->t = time - prev->t;
				tmp->t = tmp->t - time;
				return;
			}
		}
		time -= tmp->t;
		prev = tmp;
		tmp = tmp->next;
	}
	//ubacivanje na kraj
	newNode->t = time;
	last->next = newNode;
	last = newNode;
	//size++;
}

//void PCBSlList::deleteEl(ID id){
//	return;
//}

void PCBSlList::decrease(){
	if (first == 0) return;
	(first->t)--;
	SlNode * tmp = first;
	while (tmp != 0 && tmp->t == 0) {
		tmp->info->state = PCB::READY;
		first=first->next;
		Scheduler::put(tmp->info);
		tmp=first;
	}
}

PCB* PCBSlList::take(){
	if(first == 0) return 0;
	SlNode* tmp = first;
	first = first->next;
	if(first == 0)
		last = 0;
	return tmp->info;
}
