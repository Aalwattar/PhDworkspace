/*
 * schedulers.h
 *
 *  Created on: May 30, 2012
 *      Author: aalwatta
 */

#ifndef SCHEDULERS_H_
#define SCHEDULERS_H_
#include "headers.h"

#include "queue.h"

#define BUSY 1
#define QEmpty 3
#define ERROR 4
#define SUCCESS 0

 struct Counts
{
	 u32 busyCounterSW;
	 u32 busyCounterHW;
	 u32 SW2HWMig;
	 u32 HW2SWMig;
	 u32 ReuseCounter;
	 u32 SWTasksCounter;
};


int IsNodeReady(u32 id );
int ReusePRR(int module);
int FindFreePRR();
int   RunTaskSI(Queue ReadyQ , XMbox *Mbox, struct Counts *Counters,XHwIcap *HwIcap);
int     RunTask(Queue ReadyQ , XMbox *Mbox, struct Counts *Counters,XHwIcap *HwIcap);
int RunTaskSIII(Queue ReadyQ , XMbox *Mbox, struct Counts *Counters,XHwIcap *HwIcap);
void Init_Rand_Prr(int no);
int SchedSimple(Queue ReadyQ,int size);
void rstCounters(struct Counts* counters);
#endif /* SCHEDULERS_H_ */
