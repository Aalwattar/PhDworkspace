/*****************************************************************************
* Filename:          /home/ahmed/works/PHD_RS/sched/PR_test/edk/drivers/math_v2_00_a/src/math.c
* Version:           2.00.a
* Description:       math Driver Source File
* Date:              Wed Jun 15 12:56:15 2011 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "headers.h"


struct ExecUnit MB1 = { .ID=1, .Busy=NO};

/* TODO Change to MATH_INTR_IPER_OFFSET to common constants */
/************************** Function Definitions ***************************/

/**
 *
 * Enable all possible interrupts from MATH device.
 *
 * @param   baseaddr_p is the base address of the MATH device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void MATH_EnableInterrupt(void * baseaddr_p)
{
        Xuint32 baseaddr;
        baseaddr = (Xuint32) baseaddr_p;

        /*
         * Enable all interrupt source from user logic.
         */
        MATH_mWriteReg(baseaddr, MATH_INTR_IPIER_OFFSET, 0x00000001);

        /*
         * Enable all possible interrupt sources from device.
         */
        MATH_mWriteReg(baseaddr, MATH_INTR_DIER_OFFSET,
                       INTR_TERR_MASK
                       | INTR_DPTO_MASK
                       | INTR_IPIR_MASK
                      );

        /*
         * Set global interrupt enable.
         */
        MATH_mWriteReg(baseaddr, MATH_INTR_DGIER_OFFSET, INTR_GIE_MASK);
}




void MATH_DisableInterrupt(void * baseaddr_p)
{
        Xuint32 baseaddr;
        baseaddr = (Xuint32) baseaddr_p;

        /*
         * Enable all interrupt source from user logic.
         */
        MATH_mWriteReg(baseaddr, MATH_INTR_IPIER_OFFSET, 0x00000000);

        /*
         * Enable all possible interrupt sources from device.
         */
//  MATH_mWriteReg(baseaddr, MATH_INTR_DIER_OFFSET,
//    INTR_TERR_MASK
//    | INTR_DPTO_MASK
//    | INTR_IPIR_MASK
//    );

        /*
         * Set global interrupt enable.
         */
        MATH_mWriteReg(baseaddr, MATH_INTR_DGIER_OFFSET, 0x0);
}


inline Xuint32 CalcuateExecTime(Xuint32 orig,Xuint32 new)
{Xuint32 tmp;
	double ratio;
	if (!orig)
		return new;
	ratio=(((double) (Xint32) ((Xint32)new- (Xint32)orig)) * EXEC_TIME_LEARNING_RATIO);
tmp=((Xint32) orig+ (Xint32) ratio);
//printf("org [%lu] new[%lu] ratio [%lf] result [%lu]\r\n",orig,new ,ratio,tmp);
	return tmp;
}


/* TODO ADD Config time to the PRR instead of to the taskstypes it would be faster
 * This if the SW task ET is shorter than MATH0+Config but longer than MATH0 alone*/
void CalcSWPrio(u32 ID)
{ int i;
	if(TasksTypes[ID].SWET==0 || TasksTypes[ID].HWET==0 ) return;
	//printf("I am here SW[%d] HW[%d] ID[%d] \r\n",TasksTypes[ID].SWET,TasksTypes[ID].HWET,ID);
	if(TasksTypes[ID].SWET<TasksTypes[ID].HWET)
	{
		TasksTypes[ID].SWPriority =0;
		return;
	}

//	for (i=0 ; i<AVAILABLE_PRR ;i++)
//	{
//
//		if(TasksTypes[ID].ConfigTime[i]==0) continue;
//		if (TasksTypes[ID].SWET <= (TasksTypes[ID].HWET+TasksTypes[ID].ConfigTime[i]))
//		{
//			TasksTypes[ID].SWPriority=i;
//			return;
//		} else if(TasksTypes[ID].SWPriority <i)
//		{
//			TasksTypes[ID].SWPriority=i;
//		}
//
//
//	}

	for (i=0 ; i<AVAILABLE_PRR ;i++)
	{

		if(PRRs[i].ConfigTime==0) continue;
		//printf ("I got to here \r\n");
		if (TasksTypes[ID].SWET <= (TasksTypes[ID].HWET+PRRs[i].ConfigTime))
		{
			TasksTypes[ID].SWPriority=i;
			return;
		} else if(TasksTypes[ID].SWPriority <i)
		{
			TasksTypes[ID].SWPriority=i;
		}


	}

}
/**
 *
 * Example interrupt controller handler for MATH device.
 * This is to show example of how to toggle write back ISR to clear interrupts.
 *
 * @param   baseaddr_p is the base address of the MATH device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */

/* TODO check the peripheral interrupt status */

void MATH_Int_DefaultHandler(enum PRRID PRR_ID)
{
      //  Xuint32 IntrStatus;
      //  Xuint32 IpStatus;
        Xuint32 baseaddr=PRRs[PRR_ID].HW.BaseAddr;
        struct TimerValue tmpTV;
        Xuint32 tmp=0;
        struct Simulation simtmp;
        getTaskSimulation(PRRs[PRR_ID].DFGID,&simtmp);
        /*
         * Get status from Device Interrupt Status Register.
         */

      //  IntrStatus = MATH_mReadReg( baseaddr, MATH_INTR_DISR_OFFSET);

       // printf("Device Interrupt! PRR [%s] DISR value : 0x%08x \n\r", PRRs[PRR_ID].name,(unsigned int )IntrStatus);

        setTaskResult(PRRs[PRR_ID].DFGID,(int ) Xil_In32( baseaddr));
       // printf("store results for task[%d]\r\n",PRRs[PRR_ID].DFGID);
//        dfg1[PRRs[PRR_ID].DFGID].result =(int ) Xil_In32( baseaddr);
        taskDone(PRRs[PRR_ID].DFGID);
//        dfg1[PRRs[PRR_ID].DFGID].Done =YES;
        decTaskCounter();
//        TasksDoneCounter--;
#if DEBUG_PRING
        printf("Device [%X] Result: %d\n\r ", baseaddr,dfg1[PRRs[PRR_ID].DFGID].result);
#endif

        /* TODO	 *  Enable the BUSY signal during configuration
         *  The PRR wont be accessible during that time
         */
        PRRs[PRR_ID].Busy=NO;

        State=TaskDone;
        //GetElapsedTime(&dfg1[PRRs[PRR_ID].DFGID].Sim.ExecTV,&dfg1[PRRs[PRR_ID].DFGID].Sim.ExecTime);
        GetElapsedTime(&simtmp.ExecTV,&simtmp.ExecTime);

#if SCHED_III_EN
        GetTime(&tmpTV);
        if (tmpTV.ExpNo==simtmp.ExecTV.ExpNo)
        {
        	tmp=tmpTV.Value-simtmp.ExecTV.Value;
        	TasksTypes[dfg1[PRRs[PRR_ID].DFGID].TypeID].HWET= CalcuateExecTime(TasksTypes[dfg1[PRRs[PRR_ID].DFGID].TypeID].HWET,tmp);
        	CalcSWPrio(dfg1[PRRs[PRR_ID].DFGID].TypeID);
        }

#endif

        setTaskSimulation(PRRs[PRR_ID].DFGID,&simtmp);

}



void MATH0_Intr_Handler(void * ID )
{
//	Xuint32 baseaddr;
//	baseaddr = (Xuint32) baseaddr_p;
//	struct PRRIntData * PRR_Int_Data;
//			  PRR_Int_Data->Address =PRRs[Math0].HW.BaseAddr;
//			  PRR_Int_Data->ID = PRRs[Math0].ID;

        MATH_Int_DefaultHandler(Math0);
}

void MATH1_Intr_Handler(void* ID)
{

        MATH_Int_DefaultHandler(Math1);
}

void MATH2_Intr_Handler(void *ID)
{


        MATH_Int_DefaultHandler(Math2);
}

void MATH3_Intr_Handler(void*ID )
{

        MATH_Int_DefaultHandler(Math3);
}

void MATH4_Intr_Handler(void * ID)
{

        MATH_Int_DefaultHandler(Math4);
}
