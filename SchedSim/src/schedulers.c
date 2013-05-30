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



/*
 * This function differ from the ReusePRR in that it check the TypeID instead of module
 * to work with different emulated type ID
 */
int ReusePRR_V2(int module, struct PE *pRRs)
{

	int i;
	for (i=0;i<pRRs->size;i++,pRRs->pe++)
	{
	if (pRRs->pe->CurrentModule==module && !IsProcessorBusy(pRRs->pe))
	{
#if DEBUG_PRINT
		printf("found module %d, reusing %d\r\n",module,i);
#endif
		return i;
	}
	}

	return -1;
}


int FindFreePRRBestCase(unsigned int mask, struct PE *pRRs)
{
  static int count=0;
	int i;
	for (i=count;i<pRRs->size;i++,pRRs->pe++)
		{
			if (!IsProcessorBusy(pRRs->pe)  && CanRun(mask,i))
			{
#if DEBUG_PRINT
			printf("found free PRR %d\r\n",i);
#endif

			return i;
			}
		}


	if (count>=pRRs->size-1 )
	{

		count=0;
	}

	return -1;
}



/*****************************************************************************
 * This Test if the node is ready by checking it's dependency .
 * If all the dependancy are are ready the it's ready .
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
        //	 printf ("ID [%d]ready yes \r\n",id);
                return YES;
        }

        if (dfg1[id].D.isAdd_op1 == YES && dfg1[id].D.isAdd_op2 ==YES) {
                  return (isTaskDone(dfg1[id].D.op1) && isTaskDone(dfg1[id].D.op2)) ;
        }

        if (dfg1[id].D.isAdd_op1 ==YES) {

      // 	printf ("ID [%d] is ready third[%d] \r\n",id,isTaskDone(dfg1[id].D.op1));
       // 	print("not ready2\r\n");
                return isTaskDone(dfg1[id].D.op1);


        }
     //   printf ("ID [%d] is ready final [%d] \r\n",id,isTaskDone(dfg1[id].D.op2));

        return isTaskDone(dfg1[id].D.op2);
}


/*
 * schedSimple
 */
int SchedSimple(Queue ReadyQ, int size)
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
			/*FIXME This is not efficient , do it one somewhere else */

				TasksTypes[dfg1[i].TypeID].CanRun=dfg1[i].CanRun;

#if DEBUG_PRINT
			printf("Engueue %d \r\n",i);
#endif
			break;

		case CustomHW:
		case CustomHWnSW:
		default:

			printf ("Unsupported mode[%d] for task [%d] check you DFG file .. Exiting\r\n",dfg1[i].mode,i);
			return EXIT_FAILURE;
		}
	i++;
	}while(i<size);


 return EXIT_SUCCESS;
};





/*
 * Run task
 */
int RunTask(Queue ReadyQ , struct Counts *Counters, struct PEs *pes)
	{


	int task;
//	struct PRRMOD PRR_T;
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
 * FIXME this has to be changed to accomodate more than one GPPs
 */
		if (IsProcessorBusy(pes->SW->pe))
		{

#if SW_HW_MIG
			if(getTaskMode(task)==HybSW)
			{
			setTaskMode(task,HybHW);
			Counters->SW2HWMig++;
#if DEBUG_PRINT
			printf("Task %d migrate from SW to Any\r\n",task);
#endif
			} else
			{
#endif
				Counters->busyCounterSW++;
				return BUSY;
		}

#if SW_HW_MIG
		}
#endif
		Dequeue(ReadyQ);

	//	simtmp.PRRUsed=SW_PE_ID;

		nd.ExecCount=(unsigned int) dfg1[task].Emu.SWdelay;
		nd.Module=dfg1[task].TypeID;

		 LoadProcessor( pes->SW->pe,nd   );


		//MB1.DFGID=task;
		Counters->SWTasksCounter++;


		break;


	case HybHW:
	case HWOnly:

		nd.ExecCount=(unsigned int) dfg1[task].Emu.HWdelay;
		nd.Module=dfg1[task].TypeID;

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
				if (MB1.Busy==NO && getTaskMode(task)==HybHW)
				{
					setTaskMode(task,HybSW);
					Counters->HW2SWMig++;
#if DEBUG_PRINT
			printf("Task %d migrate from HW to SW\r\n",task);
#endif
					return 0;
				}
				else
				{
#endif
				Counters->busyCounterHW++;
				return BUSY;
#if SW_HW_MIG
				}
#endif
				}

			Dequeue(ReadyQ);
			//PRR_T.PRR_ID=freePRR;
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=NO;

			Reconfigure(pes->HW->pe+ freePRR,freePRR,CONFIG_TIME);

			break;


		} else

		{
			Counters->ReuseCounter++;
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=YES;
			Dequeue(ReadyQ);

		}





#if DEBUG_PRINT
		printf("Using PRR MATH%d for task [%d]\r\n",freePRR,task);
#endif





		//PRRs[PRR_T.PRR_ID].DFGID=task;



		 LoadProcessor( pes->HW->pe+freePRR,nd   );
		break;

	case CustomHW:
	case CustomHWnSW:
	default:
		printf ("Unsupported mode check your DFG file .. Exiting\n");
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

