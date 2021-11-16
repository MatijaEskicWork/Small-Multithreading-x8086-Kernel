/*
 * event.cpp
 *
 *  Created on: Feb 11, 2021
 *      Author: OS1
 */
//#include "event.h"
#include "kernev.h"
#include "event.h"

Event::Event (IVTNo ivtNo){
	myImpl = new KernelEv(ivtNo, this);
}

Event::~Event (){
	if (myImpl != 0) delete myImpl;
	myImpl = 0;
}

void Event::wait (){
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}

