/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include "headers.h"
#include "timer.h"
#include "queue.h"
#include "schedulers.h"


//int TasksDoneCounter =LAST_NODE+1 ;
#define PROCESSING 0
#define RECON		1


static XMbox Mbox;
static XHwIcap HwIcap; /* The instance of the HWICAP device */

//Queue RecQ;

static struct Counts Counters={0,0,0,0,0,0};

int main(void)
{

	Queue ReadyQ;


	/* Variable definitions */
	XMbox_Config *ConfigPtr;
	XUartLite UartLite;

	static XIntc IntcInst;
	int i,w;

	int k ;
	//struct TimerValue tmpTV;
	struct TimerTime tmpT;
	struct TimerValue totalTV;


	int Status;
	XSysAce SysAce;
	XHwIcap_Config *ICAPConfigPtr;
















	///////////PR END//////////////////////////////////////
	/*
	 * TODO remove all init to a separate file
	 * TODO Remove all print/printf commands or add #define debug preprocessor
	 */





	/*
	 * Init Timers and enable the their corresponding interrupts
	 */
	Status= InitTimer(&IntcInst);

	if (Status != XST_SUCCESS) {

		print("Timer INIT in main failed .. \r\n");
		return XST_FAILURE;

	}

	/*
	 * Init QUEUES
	 */
	/* TODO check if I Am freeing the queue */
	ReadyQ=CreateQueue(MAX_QUEUE_TASKS);
	//  RecQ=CreateQueue(MAX_QUEUE_TASKS);
	//Init_DFG();

	ResetTimer();
	TimerStart();
	TickTimerStart();









	/* End initialization */

	/* start real work */
	Init_TasksTypes();
	printf("\r\n*******************************************************************************\r\n");
	printf("*******************************************************************************\r\n");
	printf("***PLATFORM PRRS[%d] , SCHED [%s] , REUSE[YES], SWPE[%s] ****\r\n", AVAILABLE_PRR,
			SCHED_III_EN? "III": SIMPLE_SCHED_II? "II":"I",SW_HW_MIG?"YES":"NO");
	printf("*******************************************************************************\r\n");
	printf("*******************************************************************************\r\n\r\n");

	for (i=0;i<NO_OF_DFGS;i++)
	{
		printf("\r\n\r\n\r\n*******************************************************************************\r\n");
		printf("Processing: DFG[%d] with [%d] nodes please wait .....\r\n", i, DFGArray[i].size);
		printf("*******************************************************************************\r\n");
#if INDEPENDENT_DFGS

			reInit_PRRs(PRR_NUMBER);
#endif
		for (w=0;w<NO_OF_DFG_REP;w++)
		{
#if  SCHED_I_EN
			Init_Rand_Prr(AVAILABLE_PRR);
#endif
			printf("\r\n Processing: DFG[%d] with [%d] nodes ITERATION [%d] \r\n", i, DFGArray[i].size,w);
			ResetTimer();
			MakeEmpty(ReadyQ);
			dfg1=DFGArray[i].dfg;
			/* Reset Configuration Counter */
			ResetConfigCount();
			setTaskCounter(DFGArray[i].size);
			reinitTasksTable(DFGArray[i].size);

			GetTime(&totalTV);
			/*reset Counters structure */

			rstCounters(&Counters);
			/*init seed*/
			srand(totalTV.Value);


			do {
				switch(State)
				{
				case CfgDone:
#if  SCHED_I_EN
					RunTaskSI(ReadyQ,&Mbox,&Counters, &HwIcap);
#elif SCHED_III_EN
					RunTaskSIII(ReadyQ,&Mbox,&Counters, &HwIcap);
#elif SIMPLE_SCHED_II
					RunTask(ReadyQ,&Mbox,&Counters, &HwIcap);
#endif
					State=TaskDone;
					break;
				case TaskDone:
					SchedSimple(ReadyQ,DFGArray[i].size);
					//SchedSimpleRc();


#if  SCHED_I_EN
					RunTaskSI(ReadyQ,&Mbox,&Counters, &HwIcap);
#elif SCHED_III_EN

					RunTaskSIII(ReadyQ,&Mbox,&Counters, &HwIcap);
#elif SIMPLE_SCHED_II
					RunTask(ReadyQ,&Mbox,&Counters, &HwIcap);
#endif
					State=TaskDone;
					break;
				case Start:
					SchedSimple(ReadyQ,DFGArray[i].size);
					// SchedSimpleRc();
					State=TaskDone;
					break;
				case None:
					break;
				default :
					print("ERROR: Unknown state ...\r\n");
					break ;

				}


				//}while (dfg1[LAST_NODE].Done==NO );
			}while (getTaskCounter() );

			//	printf("dfg last is [%s]\r\n",isTaskDone(LAST_NODE)==YES?"YES":"NO");
			GetElapsedTime(&totalTV,&tmpT);

			print_DFG( );

			printf("Process complete in {%2lu:%.3lu} Second \r\n",tmpT.Sec,tmpT.ThSec );
			printf ("Number of configuration= %lu SW Busy [%lu] HW Busy [%lu]\r\n",GetConfigCount(),Counters.busyCounterSW,Counters.busyCounterHW);
			printf("SW2HW MIG [%lu]  HW2SW Mig [%lu] #of Reuse [%lu]  #SW tasks [%lu] \r\n",Counters.SW2HWMig,Counters.HW2SWMig,Counters.ReuseCounter,Counters.SWTasksCounter);
		}
	}
	printf("Cleaning up ....\r\n\r\n\r\n");

	TimerStop();
	freeTasksTable();


	return 0;
}

///////////////////////////////////

