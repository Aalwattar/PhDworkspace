/*
 * mbox_lib.c
 *
 *  Created on: May 31, 2011
 *      Author: Ahmed Al-Wattar
 */
/***************************** Include Files **********************************/

#include "headers.h"


/***************** Macros (Inline Functions) Definitions *********************/

#define printf xil_printf	/* A smaller footprint printf */
char *Role[2] = { "Master",
                  "Slave"
                };



int mbox_init(XMbox * Mbox, XMbox_Config *ConfigPtr)
{
        //XMbox_Config *ConfigPtr;
        int Status;

        printf ("(%s):\tStarts.\r\n", Role[MY_CPU_ID]);

        /*
         * Lookup configuration data in the device configuration table.
         * Use this configuration info down below when initializing this
         * component.
         */
        ConfigPtr = XMbox_LookupConfig(MBOX_DEVICE_ID);
        if (ConfigPtr == (XMbox_Config *)NULL) {
                printf ("(%s):\tLookupConfig Failed.%8.8x\r\n",
                        Role[MY_CPU_ID], MBOX_DEVICE_ID	 );
                return XST_FAILURE;
        }

        /*
         * Perform the rest of the initialization
         */
        Status = XMbox_CfgInitialize(Mbox, ConfigPtr, ConfigPtr->BaseAddress);
        if (Status != XST_SUCCESS) {
                return XST_FAILURE;
        }
        return XST_SUCCESS;
}


/*****************************************************************************/
/**
 *
 * This is the interrupt For the mailbox .
 *
 * @param 	CallBackRef is a callback reference passed in by the upper layer
 *		when setting the interrupt handler, and is passed back to the
 *		upper layer when the interrupt handler is called.
 *
 * @return	None.
 *
 * @note		None.
 *
 ******************************************************************************/
void MailboxIntrHandler(void *CallbackRef)
{
        u32 Mask;
        XMbox *MboxInstPtr = (XMbox *) CallbackRef;
        u32 RecvMSG;
        struct TimerValue tmpTV;
        Xuint32 tmp=0;

        struct Simulation simtmp;
        getTaskSimulation(MB1.DFGID,&simtmp);

        Mask = XMbox_GetInterruptStatus(MboxInstPtr);

        if (Mask & XMB_IX_RTA) {
                //IntrRTACount++;
                XMbox_ReadBlocking(MboxInstPtr, (u32*) (&RecvMSG), 4);

            //    dfg1[MB1.DFGID].result = (int) RecvMSG;
                setTaskResult(MB1.DFGID,(int) RecvMSG);
                taskDone(MB1.DFGID);
              //  dfg1[MB1.DFGID].Done =YES;
                //TasksDoneCounter--;/* TODO get rid of this global shit*/
                decTaskCounter();
        }
        //
        //	if (Mask & XMB_IX_ERR) {
        //		IntrERRCount++;
        //	}

        XMbox_ClearInterrupt(MboxInstPtr, Mask);
        MB1.Busy=NO;
        State=TaskDone;
        GetElapsedTime(&simtmp.ExecTV,&simtmp.ExecTime);
#if SCHED_III_EN
        GetTime(&tmpTV);
        if (tmpTV.ExpNo==simtmp.ExecTV.ExpNo)
        {
        	tmp=tmpTV.Value-simtmp.ExecTV.Value;
        	TasksTypes[dfg1[MB1.DFGID].TypeID].SWET= CalcuateExecTime(TasksTypes[dfg1[MB1.DFGID].TypeID].SWET,tmp);
        	CalcSWPrio(dfg1[MB1.DFGID].TypeID);
        }
#endif
        setTaskSimulation(MB1.DFGID,&simtmp);
}
