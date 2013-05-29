/*
 * schedulers.c
 *
 *  Created on: May 30, 2012
 *      Author: aalwatta
 */

#include "schedulers.h"
#include "queue.h"
#include "PlatformConfig.h"
/*
 * TODO remove this in a seperate file and protect it .
 * It should never be used directly
 */

static int PRRsRandArray[MAX_PR_MODULES];

//this fucntion randomize the locations of PRRs
// it expects external array PRRsRandArray[MAX_NUMBER_OF_PRRs)
void Init_Rand_Prr(int no)
{	int max=no;

	int rnd,i,k=0;
	for (i=0;i<max;i++)
		PRRsRandArray[i]=-1;

	for (i=0;i<max;i++)
	{
		while(PRRsRandArray[rnd=rand()%max]!=-1);
		PRRsRandArray[rnd]=k;
		k++;

	}

}
int AccessRandPRR(int i, int maxnumber)
{
	if (i<0 || i>=maxnumber)
	{
		print("AcessRandPRR:index out of range Exiting");
		exit(1);
	}
	return PRRsRandArray[i];
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
      //  	printf ("ID [%d] is readysecond [%d] \r\n",id,isTaskDone(dfg1[id].D.op1) && isTaskDone(dfg1[id].D.op2));
        	//print("not ready1\n\r");
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
		//print("notready\r\n");
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
			//	printf(" can run %X-->",TasksTypes[dfg1[i].TypeID].CanRun);
				TasksTypes[dfg1[i].TypeID].CanRun=dfg1[i].CanRun;

			//	printf(" can run %X \n",dfg1[i].CanRun);
				//	TasksTypes[dfg1[i].TypeID].CanRun=0xFF;
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
 * This function differ from the ReusePRR in that it check the TypeID instead of module
 * to work with different emulated type ID
 */
int ReusePRR_V2(int module)
{
#if SIMPLE_SCHED_III_REUSE
	int i;
	for (i=0;i<PRRs->size;i++)
	{
	if (PRRs[i].CurrentTypeID==module && PRRs[i].Busy==NO)
	{
#if DEBUG_PRINT
		printf("found module %d, reusing %d\r\n",module,i);
#endif
		return i;
	}
	}
#endif
	return -1;
}


inline int CanRun(unsigned int mask, unsigned int prr)
{
	return (mask & (((unsigned int)1) <<prr));
}


int FindFreePRR(unsigned int mask, struct PE *PRRs)
{
	//struct TimerValue tmpTV;

	 int count=0;
	int i, rantmp;
	int temp[PRRs->size];

	for (i=0;i<PRRs->size;i++)
		{
			if (PRRs->pe[i]->Busy== NO && CanRun(mask,i) )
			{
#if DEBUG_PRINT
			printf("found free PRR %d\r\n",i);
#endif
			temp[count++]=i;



			}
		}



	/* TODO init srand() */


if (count){
   // GetTime(&tmpTV);
    /*init seed*/
    srand(55);

	rantmp=temp[rand()%count];
	//printf("randtmp =[%d] \r\n",rantmp);
	return (rantmp);
}

	return -1;
}



/*
 * Find free original
 *
 */

int FindFreePRROrig(unsigned int mask, struct PE *PRRs)
{


	 static int count=PRRs->size-1;
	int i;

	for (i=count;i>=0;i--)
		{

			if (PRRs->pe[i]->Busy== NO && CanRun(mask,i)  )
			{
#if DEBUG_PRINT
			printf("found free PRR %d\r\n",i);
#endif

			count=i;

			return i;
			}
		}


	if (count==0 )
	{
//	//	print ("count is zero again \r\n");
		count=PRRs->size-1;
	}
//	//}






	return -1;
}

// very simple worst case
int FindFreePRRSI(unsigned int mask, struct PE *PRRs)
{


	  int count=PRRs->size-1;
	int i;

	for (i=count;i>=0;i--)
		{

			if (PRRs->pe[i]->Busy= NO && CanRun(mask,i)  )
			{
#if DEBUG_PRINT
			printf("found free PRR %d\r\n",i);
#endif

			return i;
			}
		}



	return -1;
}

int FindFreePRR_RandSI(unsigned int mask, struct PE *PRRs)
{

	  int count=PRRs->size-1;
	int i;
	int k;

	for (i=count;i>=0;i--)
		{
		  k=AccessRandPRR(i, PRRs->size);
			if (PRRs->pe[k]->Busy== NO && CanRun(mask,k)  )
			{
#if DEBUG_PRINT
			printf("found free PRR %d\r\n",i);
#endif

			return k;
			}
		}


	return -1;
}



int FindFreePRRBestCase(unsigned int mask, struct PE *PRRs)
{
  static int count=0;
	int i;
	for (i=count;i<PRRs->size;i++)
		{
			if (PRRs->pe[i]->Busy== NO && CanRun(mask,i))
			{
#if DEBUG_PRINT
			printf("found free PRR %d\r\n",i);
#endif

			return i;
			}
		}


	if (count>=PRRs->size-1 )
	{

		count=0;
	}

	return -1;
}


int FindFreePRR_PRIO(unsigned int mask, struct PE *PRRs)
{

	 int count=0;
	int i;

		for (i=0;i<PRRs->size;i++)
		{
			if (PRRs->pe[i]->Busy== NO && CanRun(mask,i))
			{ count++;
#if DEBUG_PRINT
			printf("found free PRR %d\r\n",i);
#endif
			return i;
			}
		}


	return -1;
}

/*
 * Run task
 */
int RunTask(Queue ReadyQ , struct Counts *Counters)
	{

//	struct TimerValue tmpTV;
	int task;
	struct PRRMOD PRR_T;
    struct Simulation simtmp;

#if SIMPLE_SCHED_SER
	static int i=0;
#endif
	int freePRR=5;

	unsigned int msgS[5] = { 1111, 2, 12, 10 ,20000000};
	msgS[0] = 1111;
	if (IsEmpty(ReadyQ))
		return QEmpty;
	task =Front(ReadyQ);

	  getTaskSimulation(task,&simtmp);

	PRR_T.Module= msgS[1] = dfg1[task].operation;
	PRR_T.TypeID=dfg1[task].TypeID;
	PRR_T.CanRun=TasksTypes[dfg1[task].TypeID].CanRun;
	msgS[2] = dfg1[task].D.isAdd_op1 ? getTaskResult(dfg1[task].D.op1)
			: dfg1[task].D.op1;
	msgS[3] = dfg1[task].D.isAdd_op2 ? getTaskResult(dfg1[task].D.op2)
			: dfg1[task].D.op2;
	msgS[4]=dfg1[task].Emu.HWdelay;


	switch (getTaskMode(task)) {

	case HybSW:

	case SWOnly:

		if (MB1.Busy== YES)
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

		simtmp.PRRUsed=SW_PE_ID;
		MB1.Busy = YES;
		MB1.DFGID=task;
		Counters->SWTasksCounter++;
#if DEBUG_PRINT
		print("SW operation \r\n");
#endif

		/* TODO change to non blocking one */
		//GetTime(&tmpTV);
	//	GetTime(&simtmp.ExecTV);
		msgS[4]=(unsigned int) dfg1[task].Emu.SWdelay;
		//XMbox_WriteBlocking(Mbox, (unsigned int*) (msgS), 20);
		GetElapsedTime(&tmpTV,&simtmp.ConfigTime);
#if DEBUG_PRINT
		printf(" op1 %ld  operation %ld op2 %ld \r\n", msgS[2],
				msgS[task], msgS[task]);
#endif
		//   i = dfg1[i].next;
		break;


	case HybHW:
	case HWOnly:

#if SIMPLE_SCHED_SER
		for (i=0 ; i<PRRs->size ;i++)
		{
			if (PRRs[i].Busy== NO )
			{
				freePRR=i;
				break;
			}
		}
		if (freePRR>PRRs->size-1)
		{
#if SW_HW_MIG
			if (MB1.Busy==NO && getTaskMode(task)==HybHW)
			{
				setTaskMode(task,HybSW);
				Counters->HW2SWMig++;
#if DEBUG_PRINT
			printf("Task %d migrate from ANY to SW\r\n",task);
#endif

				return 0;
			}
			else
			{
#endif
			Counters->busyCounterHW++;
			return BUSY ;
#if SW_HW_MIG
			}
#endif
		}
#endif


#if SIMPLE_SCHED_II

		if ((freePRR=ReusePRR_V2(PRR_T.TypeID )) <0)
		{
#if SCHED_II_WORSTCASE
			if((freePRR=FindFreePRROrig(PRR_T.CanRun))<0)
#elif  SCHED_II_RANDOM
			if((freePRR=FindFreePRR(PRR_T.CanRun))<0)
#else
				if((freePRR=FindFreePRRBestCase(PRR_T.CanRun))<0)
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
			PRR_T.PRR_ID=freePRR;
		//	printf(" used  %ud \r\n", freePRR);
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=NO;
			GetTime(&tmpTV);
			Config(&PRR_T, HwIcap,dfg1[task].Emu.HWdelay);
			GetElapsedTime(&tmpTV,&simtmp.ConfigTime);
	//		print("Configured \r\n");
		} else

		{
			Counters->ReuseCounter++;
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=YES;
			Dequeue(ReadyQ);

		}

#endif



#if DEBUG_PRINT
		printf("Using PRR MATH%d for task [%d]\r\n",freePRR,task);
#endif
		PRR_T.PRR_ID=freePRR;

#if	SIMPLE_SCHED_SER

		Dequeue(ReadyQ);
		simtmp.PRRUsed=freePRR;
		simtmp.Reused=NO;
		GetTime(&tmpTV);
		Config(&PRR_T, HwIcap,dfg1[task].Emu.HWdelay);
		GetElapsedTime(&tmpTV,&simtmp.ConfigTime);
#endif


		PRRs[PRR_T.PRR_ID].DFGID=task;
		PRRs[PRR_T.PRR_ID].Busy= YES;
		GetTime(&simtmp.ExecTV);
		HWMath(msgS[2], msgS[3],PRR_T.PRR_ID);
//		print("send for result\r\n");

		break;

	case CustomHW:
	case CustomHWnSW:
	default:
		print ("Unsupported mode check your DFG file .. Exiting\r\n");
		return XST_FAILURE;
	}

	setTaskSimulation(task,&simtmp);
	return SUCCESS;
}

int RunTaskSI(Queue ReadyQ , struct Counts *Counters)
	{

	struct TimerValue tmpTV;
	int task;
	struct PRRMOD PRR_T;
    struct Simulation simtmp;


	int freePRR=5;

	unsigned int msgS[5] = { 1111, 2, 12, 10 ,20000000};
	msgS[0] = 1111;
	if (IsEmpty(ReadyQ))
		return QEmpty;
	task =Front(ReadyQ);

	  getTaskSimulation(task,&simtmp);

	PRR_T.Module= msgS[1] = dfg1[task].operation;
	PRR_T.TypeID=dfg1[task].TypeID;
	PRR_T.CanRun=TasksTypes[dfg1[task].TypeID].CanRun;
	msgS[2] = dfg1[task].D.isAdd_op1 ? getTaskResult(dfg1[task].D.op1)
			: dfg1[task].D.op1;
	msgS[3] = dfg1[task].D.isAdd_op2 ? getTaskResult(dfg1[task].D.op2)
			: dfg1[task].D.op2;
	msgS[4]=dfg1[task].Emu.HWdelay;


	switch (getTaskMode(task)) {

	case HybSW:

	case SWOnly:

		if (MB1.Busy== YES)
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

		simtmp.PRRUsed=SW_PE_ID;
		MB1.Busy = YES;
		MB1.DFGID=task;
		Counters->SWTasksCounter++;
#if DEBUG_PRINT
		print("SW operation \r\n");
#endif

		/* TODO change to non blocking one */
		GetTime(&tmpTV);
		GetTime(&simtmp.ExecTV);
		msgS[4]=(unsigned int) dfg1[task].Emu.SWdelay;
		XMbox_WriteBlocking(Mbox, (unsigned int*) (msgS), 20);
		GetElapsedTime(&tmpTV,&simtmp.ConfigTime);
#if DEBUG_PRINT
		printf(" op1 %ld  operation %ld op2 %ld \r\n", msgS[2],
				msgS[task], msgS[task]);
#endif
		//   i = dfg1[i].next;
		break;


	case HybHW:
	case HWOnly:




//#if SIMPLE_SCHED_I


#if SCHED_I_RANDOM
		if((freePRR= FindFreePRR_RandSI(PRR_T.CanRun))<0)
#else
		if((freePRR=FindFreePRRSI(PRR_T.CanRun))<0)
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
				//} else if((freePRR=ReusePRR_V2(PRR_T.TypeID )) <0)
			} else
#if SIMPLE_SCHED_I_REUSE
				if(PRRs[freePRR].CurrentTypeID!=PRR_T.TypeID)
#endif
				{
			Dequeue(ReadyQ);
			PRR_T.PRR_ID=freePRR;
		//	printf(" used  %ud \r\n", freePRR);
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=NO;
			GetTime(&tmpTV);
			Config(&PRR_T, HwIcap,dfg1[task].Emu.HWdelay);
			GetElapsedTime(&tmpTV,&simtmp.ConfigTime);
	//		print("Configured \r\n");

		}
#if SIMPLE_SCHED_I_REUSE
				else

		{
			Counters->ReuseCounter++;
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=YES;
			Dequeue(ReadyQ);

		}
#endif
//#endif



#if DEBUG_PRINT
		printf("Using PRR MATH%d for task [%d]\r\n",freePRR,task);
#endif
		PRR_T.PRR_ID=freePRR;




		PRRs[PRR_T.PRR_ID].DFGID=task;
		PRRs[PRR_T.PRR_ID].Busy= YES;
		GetTime(&simtmp.ExecTV);
		HWMath(msgS[2], msgS[3],PRR_T.PRR_ID);
//		print("send for result\r\n");

		break;

	case CustomHW:
	case CustomHWnSW:
	default:
		print ("Unsupported mode check your DFG file .. Exiting\r\n");
		return XST_FAILURE;
	}

	setTaskSimulation(task,&simtmp);
	return SUCCESS;
}



/*
 * Run task schedule III
 */
int RunTaskSIII(Queue ReadyQ , struct Counts *Counters)
	{

	struct TimerValue tmpTV,tmpTV2;
	int task;
	struct PRRMOD PRR_T;
	 struct Simulation simtmp;

	int freePRR=5;

	unsigned int msgS[5] = { 1111, 2, 12, 10 ,20000000};
	msgS[0] = 1111;
	if (IsEmpty(ReadyQ))
		return QEmpty;
	task =Front(ReadyQ);
 getTaskSimulation(task,&simtmp);
 /* TODO look into this and unify all to take data from taskTypes array */
	PRR_T.Module= msgS[1] = dfg1[task].operation;
	PRR_T.TypeID=dfg1[task].TypeID;
	PRR_T.CanRun=TasksTypes[dfg1[task].TypeID].CanRun;
	msgS[2] = dfg1[task].D.isAdd_op1 ? getTaskResult(dfg1[task].D.op1)
			: dfg1[task].D.op1;
	msgS[3] = dfg1[task].D.isAdd_op2 ? getTaskResult(dfg1[task].D.op2)
			: dfg1[task].D.op2;
	msgS[4]=dfg1[task].Emu.HWdelay;


	switch (getTaskMode(task)) {

	case HybSW:

	case SWOnly:

		if (MB1.Busy== YES)
		{

#if SW_HW_MIG
			if(getTaskMode(task)==HybSW )
			{
					/*
					 * TODO probably it's better to move it to HW only if there is a free PRR or the SW priority is less that HW
					 */
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
		simtmp.PRRUsed=SW_PE_ID;
		MB1.Busy = YES;
		MB1.DFGID=task;
		Counters->SWTasksCounter++;
#if DEBUG_PRINT
		print("SW operation \r\n");
#endif

		/* TODO change to non blocking one .. In a second thought its not that important*/
		GetTime(&tmpTV);


		GetTime(&simtmp.ExecTV);
		msgS[4]=(unsigned int) dfg1[task].Emu.SWdelay;
		XMbox_WriteBlocking(Mbox, (unsigned int*) (msgS), 20);
		GetElapsedTime(&tmpTV,&simtmp.ConfigTime);
#if DEBUG_PRINT
		printf(" op1 %ld  operation %ld op2 %ld \r\n", msgS[2],
				msgS[task], msgS[task]);
#endif
		//   i = dfg1[i].next;
		break;




	case HybHW:
	case HWOnly:


#if SW_HW_MIG
		 if(TasksTypes[dfg1[task].TypeID].SWPriority==0 && MB1.Busy==NO && getTaskMode(task)==HybHW)
		 {
				setTaskMode(task,HybSW);
				Counters->HW2SWMig++;
		#if DEBUG_PRINT
		printf("Task %d migrate from HW to SW highest prio \r\n",task);
		#endif
				return 0;
		 }
#endif

		if ((freePRR=ReusePRR_V2(PRR_T.TypeID )) <0)
		{
			if((freePRR=FindFreePRR_PRIO(PRR_T.CanRun))<0)
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
#if SW_HW_MIG
				else if (TasksTypes[dfg1[task].TypeID].SWPriority <= freePRR && MB1.Busy==NO && getTaskMode(task)==HybHW )
				{
					setTaskMode(task,HybSW);
				    Counters->HW2SWMig++;
				    return 0;
#if DEBUG
				    printf("tasks [%d] moved to software due to priority \r\n",task);
#endif
				}
#endif
			Dequeue(ReadyQ);
			PRR_T.PRR_ID=freePRR;
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=NO;




			GetTime(&tmpTV);
			Config(&PRR_T, HwIcap,dfg1[task].Emu.HWdelay);


			if(PRRs[PRR_T.PRR_ID].ConfigTime==0)
						{
						GetTime(&tmpTV2);
						if (tmpTV2.ExpNo !=tmpTV.ExpNo) break;
						PRRs[PRR_T.PRR_ID].ConfigTime= tmpTV2.Value-tmpTV.Value;
						}

//			if(TasksTypes[dfg1[task].TypeID].ConfigTime[PRR_T.PRR_ID]==0)
//			{
//			GetTime(&tmpTV2);
//			if (tmpTV2.ExpNo !=tmpTV.ExpNo) break;
//			TasksTypes[dfg1[task].TypeID].ConfigTime[PRR_T.PRR_ID]= tmpTV2.Value-tmpTV.Value;
//			}
			GetElapsedTime(&tmpTV,&simtmp.ConfigTime);
		}


		else
		{
			simtmp.PRRUsed=freePRR;
			simtmp.Reused=YES;
			Dequeue(ReadyQ);
			Counters->ReuseCounter++;

		}





#if DEBUG_PRINT
		printf("Using PRR MATH%d \r\n",freePRR);
#endif
		PRR_T.PRR_ID=freePRR;



		PRRs[PRR_T.PRR_ID].DFGID=task;
		PRRs[PRR_T.PRR_ID].Busy= YES;
		GetTime(&simtmp.ExecTV);
		HWMath(msgS[2], msgS[3],PRR_T.PRR_ID);


		// i = dfg1[i].next;
		break;

	case CustomHW:
	case CustomHWnSW:
	default:
		print ("Unsupported mode check your DFG file .. Exiting\r\n");
		return XST_FAILURE;
	}
	setTaskSimulation(task,&simtmp);
	return SUCCESS;
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
