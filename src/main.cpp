/*
 * main.cpp
 *
 *  Created on: Feb 4, 2021
 *      Author: OS1
 */
#include "SCHEDULE.H"
#include "pcb.h"
#include "idleThr.h"
//#include "thread.h"
#include <iostream.h>
#include <stdio.h>
#include "kernsem.h"
//#include <DOS.H>
//#include <STDIO.H>
#include <stdarg.h>
#include <stdlib.h>
#include "event.h"
#include "ivt.h"
#include "user.h"

extern int syncPrintf(const char *format, ...);
extern void tick();
extern int userMain (int argc, char* argv[]);


//#include "defines.h"
FILE *fptr;
volatile unsigned tsp, tss, tbp;

unsigned globCounter = 1;
unsigned changeContext = 0;
PCB* mainPCB = 0;


void interrupt timer(){	// prekidna rutina
	if (!changeContext) {
		globCounter--;
		KernelSem::globalSemList->decreaseWaiting();
		tick();
		//asm int 60h
	}

	if (globCounter == 0 || changeContext) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->setSP(tsp);
		PCB::running->setSS(tss);
		PCB::running->setBP(tbp);

		if (PCB::running->state == PCB::READY) {
			Scheduler::put(PCB::running);
		}

		PCB::running= Scheduler::get();	// Scheduler
		globCounter = PCB::running->counter;
		tsp = PCB::running->getSP();
		tss = PCB::running->getSS();
		tbp = PCB::running->getBP();

		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}

		PCB::running->processHandlers();
	}

	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
if(!changeContext) asm int 60h;

	changeContext = 0;
}


void dispatch() {
	lock()
	changeContext = 1;
	timer();
	unlock()
}

unsigned oldTimerOFF, oldTimerSEG;

void init(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

class MainThread : public Thread {
public:
	MainThread(int _cnt, char** _arguments) : Thread(){
		cnt = _cnt;
		arguments = _arguments;
		ret = 0;
	}
	void run() {
		//syncPrintf("USLOOOO u run main-a\n");
		ret = userMain(cnt, arguments);
	}
	int ret;
private:
	int cnt;
	char** arguments;
};

int main(int argc, char** argv) {
	//globalThreadList = new ThreadList();
	//globalSemList = new SemList();
	init();
	fptr = fopen("sample.txt", "w");
	//LOS SI ESKICUUUU!!!
	mainPCB = new PCB(0);
	PCB::running = mainPCB;

	IdleThread* idleThr = new IdleThread();
	idleThr->start();

	MainThread* mainThread = new MainThread(argc, argv);
	mainThread->start();


	mainThread->waitToComplete();
	int ret = mainThread->ret;

	restore();

	delete mainPCB;
	delete mainThread;

    return ret;
}
