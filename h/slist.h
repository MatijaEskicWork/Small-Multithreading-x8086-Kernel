/*
 * slist.h
 *
 *  Created on: Feb 16, 2021
 *      Author: OS1
 */

#ifndef SLIST_H_
#define SLIST_H_


class PCB;
#include "thread.h"

class SignalList {
public:
	struct Node {
		SignalId id;
		Node* next;
		Node(SignalId _id) {
			id = _id;
			next = 0;
		}
	};

	SignalList();

	~SignalList();

	void push(Node* nd);

	void deleteList();

	SignalId take();
	//friend class PCB;
	Node* first;
	Node* last;
};



#endif /* SLIST_H_ */
