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







 int SearchReuse(Queue readyQ, struct PE *pRRs, int qSize );
 int RCSchedIIIEn(Queue ReadyQ, struct Counts *counters, struct PEs *pes);
int RCSchedII(Queue ReadyQ , struct Counts *Counters, struct PEs *pes);
int RCSchedIII(Queue ReadyQ , struct Counts *Counters, struct PEs *pes);
int AddTask2Queue(Queue ReadyQ,int size);
void RstCounters(struct Counts* counters);

#endif /* SCHEDULERS_H_ */
