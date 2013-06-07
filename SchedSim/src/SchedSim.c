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
#include "displayResults.h"


int main(void) {

	/*
	 * Variable declarations
	 */

	struct PEs pEs;
	int i,w;
	static struct Counts counters={0,0,0,0,0,0};
	Queue ReadyQ;


	/*
	 * Initialization functions.
	 */

	CreateAllPEs(&pEs,NO_OF_PRRS,NO_OF_GPPS);
	ReadyQ=CreateQueue(MAX_QUEUE_TASKS);
	Init_TasksTypes();




 /*
  * Start Program
  */


	for (i=0;i<NO_OF_DFGS;i++)
	{
		fprintf(stdout,"\n*******************************************************************************\n");
		fprintf(stdout,"Processing: DFG[%d] with [%d] nodes please wait .....\n", i, DFGArray[i].size);
		fprintf(stdout,"*******************************************************************************\n");
#if INDEPENDENT_DFGS
		InitProcessors(pEs.HWPE->pe,pEs.HWPE->size,TypeHW);
		InitProcessors(pEs.SWPE->pe,pEs.SWPE->size,TypeSW);
		RstCounters(&counters);
#endif
		for (w=0;w<NO_OF_DFG_REP;w++)
		{
#if  SCHED_I_EN
			Init_Rand_Prr(AVAILABLE_PRR);
#endif

			fprintf(stdout,"Processing: DFG[%d] with [%d] nodes ITERATION [%d] \n", i, DFGArray[i].size,w);
/*
 * Reset Things before every DFG
 */

			MakeEmpty(ReadyQ);
			dfg1=DFGArray[i].dfg;
			/* Reset Configuration Counter */
			ResetConfigCount();
			setTaskCounter(DFGArray[i].size);
			reinitTasksTable(DFGArray[i].size);
			ResetTimer();
			RstCounters(&counters);
			/*init seed*/
			//srand(totalTV.Value);

/*
 * Start Processing DFG
 */

			do {
/*
 * TODO Cannot remember Why I have these three states!! Might be  duplicated
 * code!! Double check if they are necessary and rewrite the code if necessary.
 */
				switch(State)
				{
				case CfgDone:
#if  RCS_SCHED_I
					RunTaskSI(ReadyQ,&counters;
#elif RCS_SCHED_III
					RCSchedIII(ReadyQ,&counters, &pEs);
#elif RCS_SCHED_II
					RCSchedII(ReadyQ,&counters, &pEs);
#endif
					Ticker(&pEs);
					State=TaskDone;
					break;
					case TaskDone:
						AddTask2Queue(ReadyQ,DFGArray[i].size);


#if  RCS_SCHED_I
						RunTaskSI(ReadyQ,&counters);
#elif RCS_SCHED_III

						RCSchedIII(ReadyQ,&counters, &pEs);
#elif RCS_SCHED_II

						RCSchedII(ReadyQ,&counters, &pEs);
#endif
						Ticker(&pEs);
						State=TaskDone;
						break;
					case Start:
						AddTask2Queue(ReadyQ,DFGArray[i].size);

						State=TaskDone;
						break;
					case None:
						break;
					default :
						fprintf(stderr,"ERROR: Unknown state ...\n");
						break ;

				}


			}while (getTaskCounter() );



		//	print_DFG( );

			fprintf(stdout,"Process complete in {%d} cycles \n",GetTimer());
			fprintf (stdout,"Number of configuration= %u SW Busy [%u] HW Busy [%u]\n",GetConfigCount(),counters.busyCounterSW,counters.busyCounterHW);
			fprintf(stdout,"SW2HW MIG [%u]  HW2SW Mig [%u] #of Reuse [%u]  #SW tasks [%u]\n",counters.SW2HWMig,counters.HW2SWMig,counters.ReuseCounter,counters.SWTasksCounter);
		}
	}



	/*
	 * Clean Up
	 */
	fprintf(stderr,"Cleaning up .... \n");

	freeTasksTable();
	CleanAllPEs(&pEs);
	DisposeQueue(ReadyQ);

	return EXIT_SUCCESS;
}

///////////////////////////////////


