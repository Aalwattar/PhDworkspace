/*
 * schedulers.h
 *
 *  Created on: May 30, 2012
 *      Author: aalwatta
 */

#ifndef SCHEDULERS_H_
#define SCHEDULERS_H_


#include "queue.h"
#define MAX_QUEUE_TASKS 256
#define BUSY 1
#define QEmpty 3
#define ERROR 4
#define SUCCESS 0

 struct Counts
{
	 unsigned int busyCounterSW;
	 unsigned int busyCounterHW;
	 unsigned int SW2HWMig;
	 unsigned int HW2SWMig;
	 unsigned int ReuseCounter;
	 unsigned int SWTasksCounter;
};


int IsNodeReady(unsigned int id );
int ReusePRR(int module);
int FindFreePRR();
int   RunTaskSI(Queue ReadyQ ,  struct Counts *Counters);
int     RunTask(Queue ReadyQ , struct Counts *Counters);
int RunTaskSIII(Queue ReadyQ ,  struct Counts *Counters);
void Init_Rand_Prr(int no);
int SchedSimple(Queue ReadyQ,int size);
void rstCounters(struct Counts* counters);
#endif /* SCHEDULERS_H_ */
