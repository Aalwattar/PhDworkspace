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
#include "drawTaskAllocation.h"
#include "argparse.h"
#include "tmpInitData.h"

void initPRRsConfigTime(unsigned int * prrTime, int noPRRs);

int main(int argc, char **argv) {

	/*
	 * Variable declarations
	 */

	struct node *dFG;
	int dFGsize = 0;
	struct Draw ** graph;
	unsigned int graphtime;
	struct PEs pEs;
	int i = 0, w;
	FILE *fdGraph; // text file to show the allocations of task on each PE
	char fileNameST[255];
	static struct Counts counters = { 0, 0, 0, 0, 0, 0 };
	Queue ReadyQ;

	/*
	 * Initialization functions.
	 */
	/*
	 * TODO this function should load that Tasktypes directly
	 */

#if COMMAND_LINE_READ
	struct ArgData argdatar;
	InitArgdatar(&argdatar);
	parseArgs(argc, argv, &argdatar);
#endif

	CreateAllPEs(&pEs, NO_OF_PRRS, NO_OF_GPPS);
	//CreatePRRConfigTimeArray(NO_OF_PRRS);
	initPRRsConfigTime(&PRRConfigValues[0][0], 5);
	ReadyQ = CreateQueue(MAX_QUEUE_TASKS);
	Init_TasksTypes();
	/* Graph generation*/

	/*
	 * Start Program
	 */

#if COMMAND_LINE_READ
	if (argdatar.DFGNo < 0 || argdatar.DFGNo > NO_OF_DFGS) {
		perror("ERROR [main] No of DFG out of range");
		exit(EXIT_FAILURE);
	}

	i = argdatar.DFGNo;
	dFGsize = DFGArray[i].size;
	dFG = CreateDFG(dFGsize);
	CopyDFG(dFG, DFGArray[i].dfg, dFGsize);

	if (argdatar.TaskTypeFlag) {
		fprintf(stderr, "filling tasktype \n");
		int l;
		for (l = 0; l < dFGsize; l++) {
			SetNodeTaskType(dFG, l, argdatar.Tasktypes[l]);
		}
	}

#else
	for (i = 0; i < NO_OF_DFGS; i++) {
		//i=argdatar.DFGNo;
		dFGsize=DFGArray[i].size;
		dFG=CreateDFG(dFGsize);
		CopyDFG(dFG,DFGArray[i].dfg,dFGsize);

#endif

	fprintf(stdout,
			"\n*******************************************************************************\n");
	fprintf(stdout, "Processing: DFG[%d] with [%d] nodes please wait .....\n",
			i, dFGsize);
	fprintf(stdout,
			"*******************************************************************************\n");
#if INDEPENDENT_DFGS
	InitProcessors(pEs.HWPE->pe, pEs.HWPE->size, TypeHW);
	InitProcessors(pEs.SWPE->pe, pEs.SWPE->size, TypeSW);
	RstCounters(&counters);
#endif
	for (w = 0; w < NO_OF_DFG_REP; w++) {
#if  SCHED_I_EN
		Init_Rand_Prr(AVAILABLE_PRR);
#endif

		fprintf(stdout, "Processing: DFG[%d] with [%d] nodes ITERATION [%d] \n",
				i, dFGsize, w);
		/*
		 * Reset Things before every DFG
		 */

		MakeEmpty(ReadyQ);
		/* Reset Configuration Counter */
		ResetConfigCount();
		setTaskCounter(dFGsize);
		reinitTasksTable(dFGsize);
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
			switch (State) {
			case CfgDone:
#if  RCS_SCHED_I
				RunTaskSI(ReadyQ,&counters;
#elif RCS_SCHED_III
				RCSchedIII(ReadyQ, &counters, &pEs, dFG);
#elif RCS_SCHED_II
				RCSchedII(ReadyQ, &counters, &pEs,dFG);
#endif
				Ticker(&pEs, dFG);
				State = TaskDone;
				break;
			case TaskDone:
				AddTask2Queue(ReadyQ, dFG, dFGsize);

#if  RCS_SCHED_I
				RunTaskSI(ReadyQ,&counters);
#elif RCS_SCHED_III

				RCSchedIII(ReadyQ, &counters, &pEs, dFG);
#elif RCS_SCHED_II

				RCSchedII(ReadyQ, &counters, &pEs);
#endif
				Ticker(&pEs, dFG);
				State = TaskDone;
				break;
			case Start:
				AddTask2Queue(ReadyQ, dFG, dFGsize);

				State = TaskDone;
				break;
			case None:
				break;
			default:
				fprintf(stderr, "ERROR: Unknown state ...\n");
				break;

			}

		} while (getTaskCounter());

//			sprintf(fileNameST, "%s_dfg_%d_it_%d.txt", GRAPH_FILE, i, w);
//			fdGraph = fopen(fileNameST, "w");
//			if (!fdGraph)
//			{
//				fprintf(stderr,"ERROR[main] Cannot open File %s\n",fileNameST);
//				exit(EXIT_FAILURE);
//			}
//			graphtime = GetTimer() / SCALING;
//			graph = CreateDraw(graphtime+1, pEs.HWPE->size + pEs.SWPE->size);
//			GenerateGraph(graph, dFG, SCALING);
//			DrawGraph(graph, graphtime, fdGraph, pEs.HWPE->size, pEs.SWPE->size);
//			CleanDraw(graph, graphtime+1);
//			fclose(fdGraph);

		//	print_DFG( );

		fprintf(stdout, "Process complete in {%d} cycles \n", GetTimer());
		fprintf(stdout,
				"Number of configuration= %u SW Busy [%u] HW Busy [%u]\n",
				GetConfigCount(), counters.busyCounterSW,
				counters.busyCounterHW);
		fprintf(stdout,
				"SW2HW MIG [%u]  HW2SW Mig [%u] #of Reuse [%u]  #SW tasks [%u]\n",
				counters.SW2HWMig, counters.HW2SWMig, counters.ReuseCounter,
				counters.SWTasksCounter);

	}
	CleanDFG(dFG);
#if COMMAND_LINE_READ
#else
}
#endif
	/*
	 * Generate graphs for task allocations
	 *
	 */

	/*
	 * Clean Up
	 */
	fprintf(stderr, "Cleaning up .... \n");
	//CleanConfigTimeArray();
	CleanTasksTable();
	CleanAllPEs(&pEs);
	DisposeQueue(ReadyQ);

	return EXIT_SUCCESS;
}

void initPRRsConfigTime(unsigned int * prrTime, int noPRRs) {
	int i;
	for (i = 0; i < noPRRs; ++i) {

		InitPRRConfigTime(i, prrTime[i]);

	}
}

///////////////////////////////////

