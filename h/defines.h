/*
 * defines.h
 *
 *  Created on: Feb 4, 2021
 *      Author: OS1
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <dos.h>

extern PCB* mainPCB;
extern unsigned globCounter;
extern unsigned changeContext;
#define lock() asm{ pushf; cli; }
#define unlock() asm{ popf; }


#endif /* DEFINES_H_ */
