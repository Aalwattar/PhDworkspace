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

static int st[200];

static unsigned long ConfigTime[50];

//int CreatePRRConfigTimeArray(int noPRR)
//{
//	if(noPRR<0)
//	{
//		fprintf(stderr,"ERROR [CreatePRRConfigTimeArray] size is negative \n");
//		exit(EXIT_FAILURE);
//	}
//	ConfigTime=malloc(sizeof(unsigned int) * noPRR);
//
//	if (!ConfigTime)
//	{
//		fprintf(stderr,"ERROR [CreatePRRConfigTimeArray] Cannot allocate Memroy  \n");
//		exit(EXIT_FAILURE);
//	}
//	return EXIT_SUCCESS;
//
//}
//void CleanConfigTimeArray(void)
//{
//	free(ConfigTime);
//
//}
int InitPRRConfigTime(int id, unsigned int value )
{
	if(value<0)
	{
		fprintf(stderr,"ERROR [InitPRRConfigTime] Configuration time cannot be  negative \n");
		exit(EXIT_FAILURE);
	}

	ConfigTime[id]=value;
	return id;
}

inline int CanRun(unsigned int mask, unsigned int prr) {
	return (mask & (((unsigned int) 1) << prr));
}

int ReusePRR_V2(int module, struct PE *pRRs) {

	int i;
	if (module<0)
		return -1;
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
int IsNodeReady(unsigned int id, struct node *dFG) {
	if (IsNodeOp1Address(dFG,id) == NO && IsNodeOp2Address(dFG,id) == NO) {
		return YES;
	}

	if (IsNodeOp1Address(dFG,id) == YES &&   IsNodeOp2Address(dFG,id) == YES) {
		return (isTaskDone(GetNodeOp1Value(dFG,id)) && isTaskDone(GetNodeOp2Value(dFG,id)));
	}

	if (IsNodeOp1Address(dFG,id)== YES) {
		return isTaskDone(GetNodeOp1Value(dFG,id));
	}
	return isTaskDone(GetNodeOp2Value(dFG,id));
}

int MoveDependentTask2TheFront(Queue readyQ,struct node *dFG, int qSize , int t)
{

	Queue qtmp;
	qtmp = CreateQueue(qSize);
	ElementType tmp, task1 = -1, task2 = -1;
	int found1 = NO;
	int found2 = NO;
	int NoAdd1 = NO;
	int NoAdd2 = NO;

	if (st[t])
		return 0;
	while (!IsEmpty(readyQ)) {
		tmp = FrontAndDequeue(readyQ);
		NoAdd1 = NoAdd2 = NO;
		if (!found1) {
			if (IsNodeOp1Address(dFG,t) && !isTaskDone(GetNodeOp1Value(dFG,GetNodeOp1Value(dFG,t)))) {
				if (tmp == GetNodeOp1Value(dFG,t)) {
					st[t] = 1;
					task1 = tmp;
					found1 = YES;
					NoAdd1 = YES;

				}
			}
		}

		if (!found2) {
			if (IsNodeOp2Address(dFG,t)&& !isTaskDone(GetNodeOp2Value(dFG,GetNodeOp2Value(dFG,t)))) {
				if (tmp == GetNodeOp2Value(dFG,t) ){
					st[t] = 1;
					task2 = tmp;
					found2 = YES;
					NoAdd2 = YES;

				}
			}
		}

		if (!NoAdd1 && !NoAdd2) {
			Enqueue(tmp, qtmp);
		}

	}

	MakeEmpty(readyQ);
	if (task1 >= 0) {
		Enqueue(task1, readyQ);
	}
	if (task2 >= 0) {
		Enqueue(task2, readyQ);
	}
	while (!IsEmpty(qtmp)) {
		tmp = FrontAndDequeue(qtmp);
		Enqueue(tmp, readyQ);

	}

	DisposeQueue(qtmp);
	return found1 || found2;

}



/*
 * AddTask2Queue
 */
int AddTask2Queue(Queue ReadyQ,struct node *dFG, int size) {
	int i = 0;

	do {
		if(IsFull(ReadyQ)){

			break;
		}
		if (IsNodeReady(i,dFG) == NO || isTaskQed(i) == YES) {
		//	MoveDependentTask2TheFront(ReadyQ,MAX_QUEUE_TASKS,i);
			i++;
			continue;
		}
		setTaskMode(i, GetNodeMode(dFG,i));
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

 setTaskTypeCanRun( GetNodeTaskType(dFG,i) ,GetNodeCanRun(dFG,i));

#if DEBUG_PRINT
			fprintf(stderr,"Enqueue %d \r\n",i);
#endif
			break;

		case CustomHW:
		case CustomHWnSW:
		default:
			fprintf(stderr, "ERROR [SchedSimple] Unsupported mode[%d] "
					"for task [%d] check you DFG file .. Exiting\n",
					GetNodeMode(dFG,i) , i);
			return EXIT_FAILURE;
		}
		i++;

	} while (i<size);

	return EXIT_SUCCESS;
}
;

/*
 * Run task SII
 */
int RCSchedII(Queue ReadyQ, struct Counts *counters, struct PEs *pes, struct node *dFG) {
	int task,tmp;

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

		nd.ExecCount = (unsigned int) GetNodeEmulationSWdelay(dFG,task);
		nd.Module = GetNodeTaskType(dFG,task);
		nd.TaskID = task;
		 setTaskSimPrrUsed(task,freeGPP+pes->HWPE->size);
		LoadProcessor(pes->SWPE->pe+freeGPP, nd);
		counters->SWTasksCounter++;
		break;

	case HybHW:
	case HWOnly:
		if ((tmp=SearchReuse(ReadyQ,pes->HWPE,MAX_QUEUE_TASKS,dFG))>=0)
		{
			task=tmp;
		}
		nd.ExecCount = (unsigned int) GetNodeEmulationHWdelay(dFG,task);
		nd.Module = GetNodeTaskType(dFG,task);
		nd.TaskID = task;

//		if ((task=SearchReuse(ReadyQ,pes->HWPE,MAX_QUEUE_TASKS))>=0)
//						{
//							nd.ExecCount = (unsigned int) GetNodeEmulHWDelay(dFG,task);
//							nd.Module = GetNodeType(dFG,task);
//							nd.TaskID = task;
//
//						}

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
					getTaskTypeCanRun(GetNodeTaskType(dFG,task)), pes->HWPE)) < 0)
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
int RCSchedIII(Queue ReadyQ, struct Counts *counters, struct PEs *pes, struct node *dFG) {
	int task,tmp;


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

		nd.ExecCount = (unsigned int) GetNodeEmulationSWdelay(dFG,task);
		nd.Module = GetNodeTaskType(dFG,task);
		nd.TaskID = task;
		 setTaskSimPrrUsed(task,freeGPP+pes->HWPE->size);
		LoadProcessor(pes->SWPE->pe+freeGPP, nd);
		counters->SWTasksCounter++;
		break;

	case HybHW:
	case HWOnly:

		nd.ExecCount = (unsigned int) GetNodeEmulationHWdelay(dFG,task);
		nd.Module = GetNodeTaskType(dFG,task);
		nd.TaskID = task;

#if SW_HW_MIG
		if (TasksTypes[GetNodeTaskType(dFG,task)].SWPriority == 0
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

		if ((tmp=SearchReuse(ReadyQ,pes->HWPE,MAX_QUEUE_TASKS, dFG))>=0)
				{task=tmp;
					nd.ExecCount = (unsigned int) GetNodeEmulationHWdelay(dFG,task);
					nd.Module = GetNodeTaskType(dFG,task);
					nd.TaskID = task;

				}


		if ((freePRR = ReusePRR_V2(nd.Module, pes->HWPE)) < 0) {
			if (IsReconfiguring()) {
				return 5;
			}
			if ((freePRR = FindFreePRRPrio(
					getTaskTypeCanRun(GetNodeTaskType(dFG,task)), pes->HWPE)) < 0) {
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
			else if (TasksTypes[GetNodeTaskType(dFG,task)].SWPriority <= freePRR
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

int SearchReuse(Queue readyQ,  struct PE *pRRs, int qSize, struct node *dFG )
{

	Queue qtmp;
	qtmp=CreateQueue(qSize);
	ElementType tmp,task=-1;
	int found=NO;

	while(!IsEmpty(readyQ))
	{
		tmp=FrontAndDequeue(readyQ);
		if ( (ReusePRR_V2(GetNodeTaskType(dFG,tmp), pRRs)) >=0 && !found)
		{
			task=tmp;
			found=YES;
		}else
		{
			Enqueue(tmp,qtmp);
		}



	}

	MakeEmpty(readyQ);
	if (task>=0){
		Enqueue(task,readyQ);
	}
	while(!IsEmpty(qtmp))
		{
			tmp=FrontAndDequeue(qtmp);
			Enqueue(tmp,readyQ);

		}

	DisposeQueue(qtmp);
	return task;



}
