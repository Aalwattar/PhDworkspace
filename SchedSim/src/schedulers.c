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

inline int CanRun(unsigned int mask, unsigned int prr)
{
	return (mask & (((unsigned int)1) <<prr));
}




int ReusePRR_V2(int module, struct PE *pRRs)
{

	int i;
	for (i=0;i<pRRs->size;i++)
	{
		if (pRRs->pe[i].CurrentModule==module && !IsProcessorBusy(&pRRs->pe[i]))
		{
#if DEBUG_PRINT
			fprintf(stderr,"found module %d, reusing %d\n",module,i);
#endif

			return i;
		}
	}

	return -1;
}


int FindFreePRRBestCase(unsigned int mask, struct PE *pRRs)
{
	//static int count=0;
	int count=0;
	int i;
	for (i=count;i<pRRs->size;i++)
	{
		if (!IsProcessorBusy(&pRRs->pe[i])  && CanRun(mask,i))
		{
#if DEBUG_PRINT
			fprintf(stderr,"found free PRR %d\n",i);
#endif

			return i;
		}
	}


	if (count>=pRRs->size-1 )
	{

		count=0;
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
int IsNodeReady(unsigned int id )
{
	if(dfg1[id].D.isAdd_op1 == NO && dfg1[id].D.isAdd_op2 ==NO) {
		return YES;
	}

	if (dfg1[id].D.isAdd_op1 == YES && dfg1[id].D.isAdd_op2 ==YES) {
		return (isTaskDone(dfg1[id].D.op1) && isTaskDone(dfg1[id].D.op2)) ;
	}

	if (dfg1[id].D.isAdd_op1 ==YES) {
		return isTaskDone(dfg1[id].D.op1);
	}
	return isTaskDone(dfg1[id].D.op2);
}


/*
 * AddTask2Queue
 */
int AddTask2Queue(Queue ReadyQ, int size)
{
	int i=0;

	do {
		if (IsNodeReady(i)== NO || isTaskQed(i)==YES  )
		{i++;
		continue;
		}
		setTaskMode(i,dfg1[i].mode);
		switch (getTaskMode(i)) {

		case HybSW:
		case SWOnly:

			Enqueue(i,ReadyQ);
			taskQed(i);
			break;
		case HWOnly:
		case HybHW:

			Enqueue(i,ReadyQ);
			taskQed(i);
			/*TODO add some error checking here */

    		TasksTypes[dfg1[i].TypeID].CanRun=dfg1[i].CanRun;
#if DEBUG_PRINT
			fprintf(stderr,"Enqueue %d \r\n",i);
#endif
			break;

		case CustomHW:
		case CustomHWnSW:
		default:
			fprintf (stderr,"ERROR [SchedSimple] Unsupported mode[%d] "
					"for task [%d] check you DFG file .. Exiting\n",dfg1[i].mode,i);
			return EXIT_FAILURE;
		}
		i++;
	}while(i<size);
	return EXIT_SUCCESS;
};





/*
 * Run task SII
 */
int RCSchedII(Queue ReadyQ , struct Counts *counters, struct PEs *pes)
{
	int task;
	struct Simulation simtmp;
	struct NodeData nd;
#if SIMPLE_SCHED_SER
	static int i=0;
#endif
	int freePRR=5;

	if (IsEmpty(ReadyQ))
		return QEmpty;
	task =Front(ReadyQ);
	getTaskSimulation(task,&simtmp);

	//	PRR_T.Module= msgS[1] = dfg1[task].operation;
	//	PRR_T.TypeID=dfg1[task].TypeID;
	//	PRR_T.CanRun=TasksTypes[dfg1[task].TypeID].CanRun;
	//	msgS[2] = dfg1[task].D.isAdd_op1 ? getTaskResult(dfg1[task].D.op1)
	//			: dfg1[task].D.op1;
	//	msgS[3] = dfg1[task].D.isAdd_op2 ? getTaskResult(dfg1[task].D.op2)
	//			: dfg1[task].D.op2;
	//	msgS[4]=dfg1[task].Emu.HWdelay;


	switch (getTaskMode(task)) {

	case HybSW:

	case SWOnly:
		/*
		 * FIXME this has to be changed to accommodate more than one GPPs
		 */
		if (IsProcessorBusy(pes->SW->pe))
		{

#if SW_HW_MIG
			if(getTaskMode(task)==HybSW)
			{
				setTaskMode(task,HybHW);
				counters->SW2HWMig++;
#if DEBUG_PRINT
				fprintf(stderr,"Task %d migrate from SW to Any\n",task);
#endif
			} else
			{
#endif
				counters->busyCounterSW++;
				return BUSY;
			}

#if SW_HW_MIG
		}
#endif
		Dequeue(ReadyQ);

		nd.ExecCount=(unsigned int) dfg1[task].Emu.SWdelay;
		nd.Module=dfg1[task].TypeID;
		nd.TaskID=task;
		LoadProcessor( pes->SW->pe,nd);
		fprintf(stderr,"Software taskssss\n");
		counters->SWTasksCounter++;
		break;


	case HybHW:
	case HWOnly:

		nd.ExecCount=(unsigned int) dfg1[task].Emu.HWdelay;
		nd.Module=dfg1[task].TypeID;
		nd.TaskID=task;
		if ((freePRR=ReusePRR_V2(nd.Module, pes->HW)) <0)
		{
			if(IsReconfiguring())
			{

				return 5;
			}
#if SCHED_II_WORSTCASE
			if((freePRR=FindFreePRROrig(PRR_T.CanRun))<0)
#elif  SCHED_II_RANDOM
				if((freePRR=FindFreePRR(PRR_T.CanRun))<0)
#else
					if((freePRR=FindFreePRRBestCase(TasksTypes[dfg1[task].TypeID].CanRun, pes->HW))<0)
#endif
					{
#if SW_HW_MIG
						if (!IsProcessorBusy(pes->SW->pe) && getTaskMode(task)==HybHW)
						{
							setTaskMode(task,HybSW);
							counters->HW2SWMig++;
//#if DEBUG_PRINT
							fprintf(stderr,"Task %d migrate from HW to SW\n",task);
//#endif
							return 0;
						}
						else
						{
#endif

							counters->busyCounterHW++;
							return BUSY;
#if SW_HW_MIG
						}
#endif
					}

			Dequeue(ReadyQ);
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=NO;
			ReconfignLoad(pes->HW->pe+ freePRR,freePRR,CONFIG_TIME,nd);

			break;

		} else

		{
			counters->ReuseCounter++;
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=YES;
			Dequeue(ReadyQ);
		}
#if DEBUG_PRINT
		fprintf(stderr,"Using PRR MATH%d for task [%d]\n",freePRR,task);
#endif
		LoadProcessor( pes->HW->pe+freePRR,nd   );
		break;

	case CustomHW:
	case CustomHWnSW:
	default:
		fprintf (stderr,"ERROR [RunTask] Unsupported mode check your DFG file .. Exiting\n");
		return EXIT_FAILURE;
	}

	setTaskSimulation(task,&simtmp);
	return EXIT_SUCCESS;
}


void rstCounters(struct Counts* counters)
{
	counters->busyCounterSW=0;
	counters->busyCounterHW=0;
	counters->SW2HWMig=0;
	counters->HW2SWMig=0;
	counters->ReuseCounter=0;
	counters->SWTasksCounter=0;
}

