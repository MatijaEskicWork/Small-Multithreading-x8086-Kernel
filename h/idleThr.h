/*
 * idleThr.h
 *
 *  Created on: Feb 5, 2021
 *      Author: OS1
 */

#ifndef IDLETHR_H_
#define IDLETHR_H_

#include "thread.h"
#include <iostream.h>
#include <stdio.h>

#include <stdarg.h>
int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	lock()
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock()
		return res;
}

class IdleThread: public Thread{

public:
	IdleThread() : Thread(1024, 1) {

	}
	void run() {
		while(1){
			//syncPrintf("IDLE\n");
		}
	}
};



#endif /* IDLETHR_H_ */
