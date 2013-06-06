/*
 * schedulers.c
 *
 *  Created on: May 30, 2012
 *      Author: aalwatta
 */
#include "schedulers.h"
#include "queue.h"
#include "PlatformConfig.h"
#include "reconfiguration.h"
#include "processors.h"
#include "data.h"

static unsigned long ConfigTime[] = { PRR_0_CONFIG_TIME, PRR_1_CONFIG_TIME,
		PRR_2_CONFIG_TIME, PRR_3_CONFIG_TIME, PRR_4_CONFIG_TIME };

inline int CanRun(unsigned int mask, unsigned int prr) {
	return (mask & (((unsigned int) 1) << prr));
}

int ReusePRR_V2(int module, struct PE *pRRs) {

	int i;
	for (i = 0; i < pRRs->size; i++) {
		if (pRRs->pe[i].CurrentModule == module
				&& !IsProcessorBusy(&pRRs->pe[i])) {
#if DEBUG_PRINT
			fprintf(stderr,"found module %d, reusing %d\n",module,i);
#endif

			return i;
		}
	}

	return -1;
}

int FindFreePRRPrio(unsigned int mask, struct PE *pRRs) {
	int count = 0;
	int i;
	for (i = count; i < pRRs->size; i++) {
		if (!IsProcessorBusy(&pRRs->pe[i]) && CanRun(mask, i)) {
			count++;
#if DEBUG_PRINT
			fprintf(stderr,"found free PRR %d\n",i);
#endif

			return i;
		}
	}

	return -1;
}

int FindFreeGPP(unsigned int mask, struct PE *GPPs) {

	int i;
	for (i = 0; i < GPPs->size; i++) {
		if (!IsProcessorBusy(&GPPs->pe[i]) && CanRun(mask, i)) {
#if DEBUG_PRINT
			fprintf(stderr,"found free GPP %d\n",i);
#endif

			return i;
		}
	}

	return -1;
}


int FindFreePRRBestCase(unsigned int mask, struct PE *pRRs) {
	static int count = 0;

	int i;
	for (i = count; i < pRRs->size; i++) {
		if (!IsProcessorBusy(&pRRs->pe[i]) && CanRun(mask, i)) {
#if DEBUG_PRINT
			fprintf(stderr,"found free PRR %d\n",i);
#endif

			return i;
		}
	}

	if (count >= pRRs->size - 1) {

		count = 0;
	}
	//fprintf(stderr,"NOT found free PRR B\n");
	return -1;
}

/*****************************************************************************
 * This Test if the node is ready by checking it's dependency .
 * If all the dependency are are ready the it's ready .
 *
 * @param	unsigned int Node ID
 *
 * @return		True/False (Ready or not )
 *
 * @note		None.
 *
 ******************************************************************************/
int IsNodeReady(unsigned int id) {
	if (dfg1[id].D.isAdd_op1 == NO && dfg1[id].D.isAdd_op2 == NO) {
		return YES;
	}

	if (dfg1[id].D.isAdd_op1 == YES && dfg1[id].D.isAdd_op2 == YES) {
		return (isTaskDone(dfg1[id].D.op1) && isTaskDone(dfg1[id].D.op2));
	}

	if (dfg1[id].D.isAdd_op1 == YES) {
		return isTaskDone(dfg1[id].D.op1);
	}
	return isTaskDone(dfg1[id].D.op2);
}

/*
 * AddTask2Queue
 */
int AddTask2Queue(Queue ReadyQ, int size) {
	int i = 0;

	do {
		if (IsNodeReady(i) == NO || isTaskQed(i) == YES) {
			i++;
			continue;
		}
		setTaskMode(i, dfg1[i].mode);
		switch (getTaskMode(i)) {

		case HybSW:
		case SWOnly:

			Enqueue(i, ReadyQ);
			taskQed(i);
			break;
		case HWOnly:
		case HybHW:

			Enqueue(i, ReadyQ);
			taskQed(i);
			/*TODO add some error checking here */

			TasksTypes[dfg1[i].TypeID].CanRun = dfg1[i].CanRun;
#if DEBUG_PRINT
			fprintf(stderr,"Enqueue %d \r\n",i);
#endif
			break;

		case CustomHW:
		case CustomHWnSW:
		default:
			fprintf(stderr, "ERROR [SchedSimple] Unsupported mode[%d] "
					"for task [%d] check you DFG file .. Exiting\n",
					dfg1[i].mode, i);
			return EXIT_FAILURE;
		}
		i++;
	} while (i < size);
	return EXIT_SUCCESS;
}
;

/*
 * Run task SII
 */
int RCSchedII(Queue ReadyQ, struct Counts *counters, struct PEs *pes) {
	int task;

	struct NodeData nd;
	int freePRR = 0;
	int freeGPP=0;
	if (IsEmpty(ReadyQ))
		return QEmpty;
	task = Front(ReadyQ);

	switch (getTaskMode(task)) {

	case HybSW:

	case SWOnly:
		/*
		 * FIXME this has to be changed to accommodate more than one GPPs
		 */
		if ((freeGPP=FindFreeGPP(0xFF,pes->SWPE))<0) {

#if SW_HW_MIG
			if (getTaskMode(task) == HybSW) {
				setTaskMode(task, HybHW);
				counters->SW2HWMig++;
#if DEBUG_PRINT
				fprintf(stderr,"Task %d migrate from SW to Any\n",task);
#endif
			} else {
#endif
				counters->busyCounterSW++;
				return BUSY;
			}

#if SW_HW_MIG
		}
#endif
		Dequeue(ReadyQ);

		nd.ExecCount = (unsigned int) dfg1[task].Emu.SWdelay;
		nd.Module = dfg1[task].TypeID;
		nd.TaskID = task;
		 setTaskSimPrrUsed(task,freeGPP+pes->HWPE->size);
		LoadProcessor(pes->SWPE->pe+freeGPP, nd);
		counters->SWTasksCounter++;
		break;

	case HybHW:
	case HWOnly:

		nd.ExecCount = (unsigned int) dfg1[task].Emu.HWdelay;
		nd.Module = dfg1[task].TypeID;
		nd.TaskID = task;
		if ((freePRR = ReusePRR_V2(nd.Module, pes->HWPE)) < 0) {
			if (IsReconfiguring()) {

				return 5;
			}
#if SCHED_II_WORSTCASE
			if((freePRR=FindFreePRROrig(PRR_T.CanRun))<0)
#elif  SCHED_II_RANDOM
			if((freePRR=FindFreePRR(PRR_T.CanRun))<0)
#else
			if ((freePRR = FindFreePRRBestCase(
					TasksTypes[dfg1[task].TypeID].CanRun, pes->HWPE)) < 0)
#endif
					{
#if SW_HW_MIG
				if ( FindFreeGPP(0xFF,pes->SWPE)>=0
						&& getTaskMode(task) == HybHW) {
					setTaskMode(task, HybSW);
					counters->HW2SWMig++;
#if DEBUG_PRINT
					fprintf(stderr,"Task %d migrate from HW to SW\n",task);
#endif
					return 0;
				} else {
#endif

					counters->busyCounterHW++;
					return BUSY;
#if SW_HW_MIG
				}
#endif
			}

			Dequeue(ReadyQ);
	        setTaskSimPrrUsed(task,freePRR);
	        setTaskSimReused(task,NO);
	        setTaskSimConfTimeStart(task,GetTimer());


			ReconfignLoad(pes->HWPE->pe + freePRR, freePRR, ConfigTime[freePRR],
					nd);

			break;

		} else

		{
			counters->ReuseCounter++;
			 setTaskSimPrrUsed(task,freePRR);
		     setTaskSimReused(task,YES);

			Dequeue(ReadyQ);
		}
#if DEBUG_PRINT
		fprintf(stderr,"Using PRR MATH%d for task [%d]\n",freePRR,task);
#endif

		LoadProcessor(pes->HWPE->pe + freePRR, nd);
		break;

	case CustomHW:
	case CustomHWnSW:
	default:
		fprintf(stderr,
				"ERROR [RunTask] Unsupported mode check your DFG file .. Exiting\n");
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}

/*
 * rstCounter
 */
void RstCounters(struct Counts* counters) {
	counters->busyCounterSW = 0;
	counters->busyCounterHW = 0;
	counters->SW2HWMig = 0;
	counters->HW2SWMig = 0;
	counters->ReuseCounter = 0;
	counters->SWTasksCounter = 0;
}

/*
 * Run task SIII
 */
int RCSchedIII(Queue ReadyQ, struct Counts *counters, struct PEs *pes) {
	int task;

	struct NodeData nd;

	int freePRR = 5;
	int freeGPP=0;
	if (IsEmpty(ReadyQ))
		return QEmpty;
	task = Front(ReadyQ);

	switch (getTaskMode(task)) {

	case HybSW:

	case SWOnly:
		/*
		 * FIXME this has to be changed to accommodate more than one GPPs
		 */
		if ((freeGPP=FindFreeGPP(0xFF,pes->SWPE))<0) {

#if SW_HW_MIG
			if (getTaskMode(task) == HybSW) {
				setTaskMode(task, HybHW);
				counters->SW2HWMig++;
#if DEBUG_PRINT
				fprintf(stderr,"Task %d migrate from SW to Any\n",task);
#endif
			} else {
#endif
				counters->busyCounterSW++;
				return BUSY;
			}

#if SW_HW_MIG
		}
#endif
		Dequeue(ReadyQ);

		nd.ExecCount = (unsigned int) dfg1[task].Emu.SWdelay;
		nd.Module = dfg1[task].TypeID;
		nd.TaskID = task;
		 setTaskSimPrrUsed(task,freeGPP+pes->HWPE->size);
		LoadProcessor(pes->SWPE->pe+freeGPP, nd);
		counters->SWTasksCounter++;
		break;

	case HybHW:
	case HWOnly:

		nd.ExecCount = (unsigned int) dfg1[task].Emu.HWdelay;
		nd.Module = dfg1[task].TypeID;
		nd.TaskID = task;

#if SW_HW_MIG
		if (TasksTypes[dfg1[task].TypeID].SWPriority == 0
				&& FindFreeGPP(0xFF,pes->SWPE)>=0
				&& getTaskMode(task) == HybHW) {
			setTaskMode(task, HybSW);
			counters->HW2SWMig++;
#if DEBUG_PRINT
			fprintf(stderr,"Task %d migrate from HW to SW highest prio \n",task);
#endif
			return EXIT_SUCCESS;
		}
#endif

		if ((freePRR = ReusePRR_V2(nd.Module, pes->HWPE)) < 0) {
			if (IsReconfiguring()) {
				return 5;
			}
			if ((freePRR = FindFreePRRPrio(
					TasksTypes[dfg1[task].TypeID].CanRun, pes->HWPE)) < 0) {
#if SW_HW_MIG
				if (FindFreeGPP(0xFF,pes->SWPE)>=0
						&& getTaskMode(task) == HybHW) {
					setTaskMode(task, HybSW);
					counters->HW2SWMig++;
#if DEBUG_PRINT
					fprintf(stderr,"Task %d migrate from HW to SW\n",task);
#endif
					return EXIT_SUCCESS;
				} else {
#endif
					counters->busyCounterHW++;
					return BUSY;

#if SW_HW_MIG
				}
#endif
			}
#if SW_HW_MIG
			else if (TasksTypes[dfg1[task].TypeID].SWPriority <= freePRR
					&& FindFreeGPP(0xFF,pes->SWPE)>=0
					&& getTaskMode(task) == HybHW) {
				setTaskMode(task, HybSW);
				counters->HW2SWMig++;
				return EXIT_SUCCESS;
#if DEBUG_PRINT
				fprintf(stderr,"tasks [%d] moved to software due to priority \n",task);
#endif
			}
#endif
			Dequeue(ReadyQ);
			 setTaskSimPrrUsed(task,freePRR);
			 setTaskSimReused(task,NO);
			 setTaskSimConfTimeStart(task,GetTimer());

			ReconfignLoad(pes->HWPE->pe + freePRR, freePRR, ConfigTime[freePRR],
					nd);

			break;

		} else {
			counters->ReuseCounter++;
			 setTaskSimPrrUsed(task,freePRR);
			setTaskSimReused(task,YES);

			Dequeue(ReadyQ);
		}
#if DEBUG_PRINT
		fprintf(stderr,"Using PRR MATH%d for task [%d]\n",freePRR,task);
#endif

		LoadProcessor(pes->HWPE->pe + freePRR, nd);
		break;

	case CustomHW:
	case CustomHWnSW:
	default:
		fprintf(stderr,
				"ERROR [RunTask] Unsupported mode check your DFG file .. Exiting\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

