/*
 ============================================================================
 Name        : SchedSim.c
 Author      : Ahmed Al-Wattar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "processors.h"
#include "reconfiguration.h"
#include "PlatformConfig.h"
#include "queue.h"
#include "dfgTemplate.c"
#include "schedulers.h"



int main(void) {

/*
 * Variable declarations
 */

 struct PEs pEs;

struct Processor *PRRs;
struct Processor *GPPs;
struct NodeData nd1, nd2,nd3,nd4,nd5;
int i,w;

static struct Counts Counters={0,0,0,0,0,0};
Queue ReadyQ;


/*
 * Initialization functions.
 */



	PRRs=InitProcessors(5, HW);
	GPPs=InitProcessors(1, SW);

pEs.HW->pe=PRRs;
pEs.HW->size=5;
pEs.SW->pe=GPPs;
pEs.SW->size =1;


	Reconfigure(&PRRs[0], 0, 12);
	LoadProcessor( &PRRs[0], nd1);

	LoadProcessor( &PRRs[1], nd2);

	LoadProcessor( &PRRs[2], nd3);

	LoadProcessor( &PRRs[3], nd4);

	LoadProcessor( &PRRs[4], nd5);

	for (i=0; i<60; i++)
	{  printf("[%d]",i+1);

	TickAllProcessors(PRRs, 5);
	TickAllProcessors(GPPs, 1);

	TickConfig(PRRs);

	if (i==14)
		LoadProcessor( &PRRs[0], nd1);
		printf("\n");
	}






		/*
		 * Init QUEUES
		 */
		/* TODO check if I Am freeing the queue */
		ReadyQ=CreateQueue(MAX_QUEUE_TASKS);





		/* start real work */
	Init_TasksTypes();
		printf("\r\n*******************************************************************************\r\n");
		printf("*******************************************************************************\r\n");
		//printf("***PLATFORM PRRS[%d] , SCHED [%s] , REUSE[YES], SWPE[%s] ****\r\n", AVAILABLE_PRR,
	//			SCHED_III_EN? "III": SIMPLE_SCHED_II? "II":"I",SW_HW_MIG?"YES":"NO");
		printf("*******************************************************************************\r\n");
		printf("*******************************************************************************\r\n\r\n");

		for (i=0;i<NO_OF_DFGS;i++)
		{
			printf("\r\n\r\n\r\n*******************************************************************************\r\n");
		//	printf("Processing: DFG[%d] with [%d] nodes please wait .....\r\n", i, DFGArray[i].size);
			printf("*******************************************************************************\r\n");
	#if INDEPENDENT_DFGS


	#endif
			for (w=0;w<NO_OF_DFG_REP;w++)
			{
	#if  SCHED_I_EN
				Init_Rand_Prr(AVAILABLE_PRR);
	#endif
				printf("\r\n Processing: DFG[%d] with [%d] nodes ITERATION [%d] \r\n", i, DFGArray[i].size,w);
			//	ResetTimer();
				MakeEmpty(ReadyQ);
				dfg1=DFGArray[i].dfg;
				/* Reset Configuration Counter */
			//	ResetConfigCount();
				setTaskCounter(DFGArray[i].size);
				reinitTasksTable(DFGArray[i].size);


				rstCounters(&Counters);
				/*init seed*/
				//srand(totalTV.Value);


				do {
					switch(State)
					{
					case CfgDone:
	#if  SCHED_I_EN
						RunTaskSI(ReadyQ,&Counters;
	#elif SCHED_III_EN
						RunTaskSIII(ReadyQ,&Counters);
	#elif SIMPLE_SCHED_II
						RunTask(ReadyQ,&Counters);
	#endif
						State=TaskDone;
						break;
					case TaskDone:
						SchedSimple(ReadyQ,DFGArray[i].size);
						//SchedSimpleRc();


	#if  SCHED_I_EN
						RunTaskSI(ReadyQ,&Counters);
	#elif SCHED_III_EN

						RunTaskSIII(ReadyQ,&Counters);
	#elif SIMPLE_SCHED_II
						RunTask(ReadyQ,&Counters);
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
						printf("ERROR: Unknown state ...\r\n");
						break ;

					}


					//}while (dfg1[LAST_NODE].Done==NO );
				}while (getTaskCounter() );



			//	print_DFG( );

				//printf("Process complete in {%2lu:%.3lu} Second \r\n",tmpT.Sec,tmpT.ThSec );
//				printf ("Number of configuration= %lu SW Busy [%lu] HW Busy [%lu]\r\n",GetConfigCount(),Counters.busyCounterSW,Counters.busyCounterHW);
//				printf("SW2HW MIG [%lu]  HW2SW Mig [%lu] #of Reuse [%lu]  #SW tasks [%lu] \r\n",Counters.SW2HWMig,Counters.HW2SWMig,Counters.ReuseCounter,Counters.SWTasksCounter);
			}
		}
		printf("Cleaning up ....\r\n\r\n\r\n");


		freeTasksTable();
		FreeProcessors(PRRs);

		return 0;
	}

	///////////////////////////////////


