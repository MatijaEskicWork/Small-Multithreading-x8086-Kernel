/*
 * semlist.cpp
 *
 *  Created on: Feb 7, 2021
 *      Author: OS1
 */
#include "semlist.h"
#include "kernsem.h"


SemList::SemList() {
	last = 0;
	first = 0;
	size = 0;
}

SemList::~SemList(){
	SNode *tmp = first;
	SNode *prev = 0;
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
void SemList::push(KernelSem* el) {
	if (last == 0) {
			last = new SNode(el);
			first = last;
			return;
		}
		last->next = new SNode(el);
		last = last->next;
		size++;
}

void SemList::deleteEl(unsigned id){
	SNode *prev = 0;
	SNode *tmp = first;
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

KernelSem* SemList::findEl(unsigned id){
	SNode *tmp = first;
	while (tmp != 0 && tmp->info->getId() != id) {
			tmp = tmp->next;
	}
	if(tmp==0)
		return 0;
	return tmp->info;
}

void SemList::decreaseWaiting() {

	SNode *semNode = first;
	while (semNode != 0) {
		semNode->info->sleepList->decrease();
		semNode = semNode->next;
	}
}

/*void SemList::doWrapping(){

}*/

unsigned SemList::getSize(){
	return size;
}


