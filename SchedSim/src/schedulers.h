/*
 * schedulers.h
 *
 *  Created on: May 30, 2012
 *      Author: aalwatta
 */

#ifndef SCHEDULERS_H_
#define SCHEDULERS_H_


#include "queue.h"
#include "processors.h"

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



// int CreatePRRConfigTimeArray(int noPRR);
 void CleanConfigTimeArray(void);
 int InitPRRConfigTime(int id, unsigned int value );




 int SearchReuse(Queue readyQ, struct PE *pRRs, int qSize , struct node *);
 int RCSchedIIIEn(Queue ReadyQ, struct Counts *counters, struct PEs *pes, struct node *);
int RCSchedII(Queue ReadyQ , struct Counts *Counters, struct PEs *pes, struct node *);
int RCSchedIII(Queue ReadyQ , struct Counts *Counters, struct PEs *pes, struct node *);
int AddTask2Queue(Queue ReadyQ, struct node *,int size);
void RstCounters(struct Counts* counters);

#endif /* SCHEDULERS_H_ */
