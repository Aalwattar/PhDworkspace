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
#include "data.h"
#include "processors.h"
#include "reconfiguration.h"
#include "PlatformConfig.h"
#include "queue.h"
#include "schedulers.h"



int main(void) {

/*
 * Variable declarations
 */

 struct PEs pEs;

struct Processor *PRRs;
struct Processor *GPPs;

int i,w;

/* TODO change that to counters */
static struct Counts Counters={0,0,0,0,0,0};
unsigned int timer=0 ;
Queue ReadyQ;


pEs.HW=(struct PE*) malloc(sizeof(struct PE));
pEs.SW=(struct PE*) malloc(sizeof(struct PE));
/*
 * Initialization functions.
 */

printf("one");


	PRRs=InitProcessors(5, HW);
	GPPs=InitProcessors(1, SW);

	printf("one2");
pEs.HW->pe=PRRs;
pEs.HW->size=5;
pEs.SW->pe=GPPs;
pEs.SW->size =1;



printf("three");

		/*
		 * Init QUEUES
		 */
		/* TODO check if I Am freeing the queue */
		ReadyQ=CreateQueue(MAX_QUEUE_TASKS);





		/* start real work */
	Init_TasksTypes();





		for (i=0;i<NO_OF_DFGS;i++)
		{
			printf("\n*******************************************************************************\n");
			printf("Processing: DFG[%d] with [%d] nodes please wait .....\n", i, DFGArray[i].size);
			printf("*******************************************************************************\n");
	#if INDEPENDENT_DFGS

			rstCounters(&Counters);
	#endif
			for (w=0;w<NO_OF_DFG_REP;w++)
			{
	#if  SCHED_I_EN
				Init_Rand_Prr(AVAILABLE_PRR);
	#endif
				printf("\r\n Processing: DFG[%d] with [%d] nodes ITERATION [%d] \n", i, DFGArray[i].size,w);
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

					TickAllProcessors(PRRs, 5);
					TickAllProcessors(GPPs, 1);
					TickConfig(PRRs);
					printf(" %ud->",timer++);
					switch(State)
					{
					case CfgDone:
	#if  SCHED_I_EN
						RunTaskSI(ReadyQ,&Counters;
	#elif SCHED_III_EN
						RunTaskSIII(ReadyQ,&Counters);
	#elif SIMPLE_SCHED_II
						RunTask(ReadyQ,&Counters, &pEs);
	#endif
						State=TaskDone;
						break;
					case TaskDone:
						SchedSimple(ReadyQ,DFGArray[i].size);



	#if  SCHED_I_EN
						RunTaskSI(ReadyQ,&Counters);
	#elif SCHED_III_EN

						RunTaskSIII(ReadyQ,&Counters);
	#elif SIMPLE_SCHED_II

						RunTask(ReadyQ,&Counters, &pEs);
	#endif
						State=TaskDone;
						break;
					case Start:
						SchedSimple(ReadyQ,DFGArray[i].size);

						State=TaskDone;
						break;
					case None:
						break;
					default :
						printf("ERROR: Unknown state ...\n");
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
		printf("Cleaning up ....\n");


		freeTasksTable();
		FreeProcessors(PRRs);
		FreeProcessors(GPPs);
		DisposeQueue(ReadyQ);

		return 0;
	}

	///////////////////////////////////


