/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include "headers.h"
#include "timer.h"
#include "queue.h"
#include "schedulers.h"


//int TasksDoneCounter =LAST_NODE+1 ;
#define PROCESSING 0
#define RECON		1
static int SetupIntrSystem(XIntc *IntcInstancePtr, XMbox *MboxInstPtr,
		u16 IntcDevId, u16 MboxIntrId);



static XMbox Mbox;
static XHwIcap HwIcap; /* The instance of the HWICAP device */

//Queue RecQ;

static struct Counts Counters={0,0,0,0,0,0};

int main(void)
{

	Queue ReadyQ;


	/* Variable definitions */
	XMbox_Config *ConfigPtr;
	XUartLite UartLite;

	static XIntc IntcInst;
	int i,w;

	int k ;
	//struct TimerValue tmpTV;
	struct TimerTime tmpT;
	struct TimerValue totalTV;


	int Status;
	XSysAce SysAce;
	XHwIcap_Config *ICAPConfigPtr;


	/* initialization stage */

	init_platform();
	XUartLite_Initialize(&UartLite, XPAR_UARTLITE_0_DEVICE_ID);






	Status = XSysAce_Initialize(&SysAce, XPAR_SYSACE_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		print("Failed initializing SysACE \r\n");
		return XST_FAILURE;
	}


	ICAPConfigPtr = XHwIcap_LookupConfig(XPAR_XPS_HWICAP_0_DEVICE_ID);
	if (ICAPConfigPtr == NULL) {
		print("Failed configuring ICAP  \r\n");
		return XST_FAILURE;
	}

	Status = XHwIcap_CfgInitialize(&HwIcap, ICAPConfigPtr,
			ICAPConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {

		print("Failed initializing ICAP \r\n");
		return XST_FAILURE;
	}
	print("HWICAP Initialized\r\n");

	//key = XUartLite_RecvByte(XPAR_UARTLITE_0_BASEADDR );


	///////////PR END//////////////////////////////////////
	/*
	 * TODO remove all init to a separate file
	 * TODO Remove all print/printf commands or add #define debug preprocessor
	 */
#if CONFIG_FROM_DDR
	Init_CF_Memory();
	print("Bitstreams moved to memroy ..\r\n");
#endif




	if (mbox_init(&Mbox, ConfigPtr) != XST_SUCCESS) {
		print("Xbox initialization failure ... Exiting /r/n");
		return XST_FAILURE;
	}

	for (k=0 ; k<PRR_NUMBER; k++) {
		MATH_EnableInterrupt((void *) PRRs[k].HW.BaseAddr);
	}
	if (
			SetupIntrSystem(&IntcInst, &Mbox, INTC_DEVICE_ID, MBOX_INTR_ID)
			!= XST_SUCCESS) {
		print("Interrupt initialization failure ... Exiting /r/n");
		return XST_FAILURE;
	}

	/*
	 * Init Timers and enable the their corresponding interrupts
	 */
	Status= InitTimer(&IntcInst);

	if (Status != XST_SUCCESS) {

		print("Timer INIT in main failed .. \r\n");
		return XST_FAILURE;

	}

	/*
	 * Init QUEUES
	 */
	/* TODO check if I Am freeing the queue */
	ReadyQ=CreateQueue(MAX_QUEUE_TASKS);
	//  RecQ=CreateQueue(MAX_QUEUE_TASKS);
	//Init_DFG();

	ResetTimer();
	TimerStart();
	TickTimerStart();





	//   inbyte();



	/* End initialization */

	/* start real work */
	Init_TasksTypes();
	printf("\r\n*******************************************************************************\r\n");
	printf("*******************************************************************************\r\n");
	printf("***PLATFORM PRRS[%d] , SCHED [%s] , REUSE[YES], SWPE[%s] ****\r\n", AVAILABLE_PRR,
			SCHED_III_EN? "III": SIMPLE_SCHED_II? "II":"I",SW_HW_MIG?"YES":"NO");
	printf("*******************************************************************************\r\n");
	printf("*******************************************************************************\r\n\r\n");

	for (i=0;i<NO_OF_DFGS;i++)
	{
		printf("\r\n\r\n\r\n*******************************************************************************\r\n");
		printf("Processing: DFG[%d] with [%d] nodes please wait .....\r\n", i, DFGArray[i].size);
		printf("*******************************************************************************\r\n");
#if INDEPENDENT_DFGS

			reInit_PRRs(PRR_NUMBER);
#endif
		for (w=0;w<NO_OF_DFG_REP;w++)
		{
#if  SCHED_I_EN
			Init_Rand_Prr(AVAILABLE_PRR);
#endif
			printf("\r\n Processing: DFG[%d] with [%d] nodes ITERATION [%d] \r\n", i, DFGArray[i].size,w);
			ResetTimer();
			MakeEmpty(ReadyQ);
			dfg1=DFGArray[i].dfg;
			/* Reset Configuration Counter */
			ResetConfigCount();
			setTaskCounter(DFGArray[i].size);
			reinitTasksTable(DFGArray[i].size);

			GetTime(&totalTV);

			GLOBAL_TV.Value=totalTV.Value;

			/*reset Counters structure */

			rstCounters(&Counters);
			/*init seed*/
			srand(totalTV.Value);


			do {
				switch(State)
				{
				case CfgDone:
#if  SCHED_I_EN
					RunTaskSI(ReadyQ,&Mbox,&Counters, &HwIcap);
#elif SCHED_III_EN
					RunTaskSIII(ReadyQ,&Mbox,&Counters, &HwIcap);
#elif SIMPLE_SCHED_II
					RunTask(ReadyQ,&Mbox,&Counters, &HwIcap);
#endif
					State=TaskDone;
					break;
				case TaskDone:
					SchedSimple(ReadyQ,DFGArray[i].size);
					//SchedSimpleRc();


#if  SCHED_I_EN
					RunTaskSI(ReadyQ,&Mbox,&Counters, &HwIcap);
#elif SCHED_III_EN

					RunTaskSIII(ReadyQ,&Mbox,&Counters, &HwIcap);
#elif SIMPLE_SCHED_II
					RunTask(ReadyQ,&Mbox,&Counters, &HwIcap);
#endif
					State=TaskDone;
					break;
				case Start:
					SchedSimple(ReadyQ,DFGArray[i].size);
					// SchedSimpleRc();
					State=TaskDone;
					break;
				case None:
					break;
				default :
					print("ERROR: Unknown state ...\r\n");
					break ;

				}


				//}while (dfg1[LAST_NODE].Done==NO );
			}while (getTaskCounter() );

			//	printf("dfg last is [%s]\r\n",isTaskDone(LAST_NODE)==YES?"YES":"NO");
			GetElapsedTime(&totalTV,&tmpT);

			print_DFG( );

			printf("Process complete in {%2lu:%.3lu} Second \r\n",tmpT.Sec,tmpT.ThSec );
			printf ("Number of configuration= %lu SW Busy [%lu] HW Busy [%lu]\r\n",GetConfigCount(),Counters.busyCounterSW,Counters.busyCounterHW);
			printf("SW2HW MIG [%lu]  HW2SW Mig [%lu] #of Reuse [%lu]  #SW tasks [%lu] \r\n",Counters.SW2HWMig,Counters.HW2SWMig,Counters.ReuseCounter,Counters.SWTasksCounter);
		}
	}
	printf("Cleaning up ....\r\n\r\n\r\n");

	TimerStop();
	freeTasksTable();
	cleanup_platform();


	return 0;
}

///////////////////////////////////



/*****************************************************************************/
/** TODO move this function to a separate file
 *
 * This function setups the interrupt system such that interrupts can occur
 * for the Mailbox device. This function is application specific since the
 * actual system may or may not have an interrupt controller. The Mailbox
 * device could be directly connected to a processor without an Interrupt
 * controller.  The  user should modify this function to fit the application.
 *
 * @param	IntcInstancePtr is a pointer to the instance of the INTC
 *		component.
 * @param	MboxInstInstPtr is a pointer to the instance of the Mailbox.
 * @param	MboxIntrId is the interrupt Id and is typically
 *		XPAR_<INTC_instance>_<MBOX_instance>_IP2INTC_IRPT_INTR
 *		value from xparameters.h.
 *
 * @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
 *
 * @note		None.
 *
 *****************************************************************************/
int SetupIntrSystem(XIntc *IntcInstancePtr, XMbox *MboxInstPtr,
		u16 IntcDevId, u16 MboxIntrId)
{
	int Status;
	int i;
	//struct PRRIntData * PRR_Int_Data;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use
	 */
	Status = XIntc_Initialize(IntcInstancePtr, IntcDevId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Sets the Threshold
	 */

	//		XMbox_SetSendThreshold(MboxInstPtr, MAILBOX_SIT);
	XMbox_SetReceiveThreshold(MboxInstPtr, MAILBOX_RIT);


	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler
	 * performs the specific interrupt processing for the device
	 */
	Status = XIntc_Connect(IntcInstancePtr, MboxIntrId,
			(XInterruptHandler) MailboxIntrHandler, (void *) MboxInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/* FIXME  lots of work in here */

	/*
	 *	Connect the PRR regions interrupts to their corresponding ISR
	 */
	if (XIntc_Connect(IntcInstancePtr, PRRs[Math0].HW.IntID,
			(XInterruptHandler) MATH0_Intr_Handler,
			(void*) Math0) != XST_SUCCESS) {
		print("Failed to register PRR MATH0  ISR exiting ..\r\n");
		return XST_FAILURE;
	}

	if (XIntc_Connect(IntcInstancePtr, PRRs[Math1].HW.IntID,
			(XInterruptHandler) MATH1_Intr_Handler , (void*) Math1
	) != XST_SUCCESS) {
		print("Failed to register PRR MATH1  ISR exiting ..\r\n");
		return XST_FAILURE;
	}
	if (XIntc_Connect(IntcInstancePtr, PRRs[Math2].HW.IntID,
			(XInterruptHandler) MATH2_Intr_Handler , (void*) Math2
	) != XST_SUCCESS) {
		print("Failed to register PRR MATH2  ISR exiting ..\r\n");
		return XST_FAILURE;
	}
	if (XIntc_Connect(IntcInstancePtr, PRRs[Math3].HW.IntID,
			(XInterruptHandler) MATH3_Intr_Handler , (void*) Math3
	) != XST_SUCCESS) {
		print("Failed to register PRR MATH3  ISR exiting ..\r\n");
		return XST_FAILURE;
	}
	if (XIntc_Connect(IntcInstancePtr, PRRs[Math4].HW.IntID,
			(XInterruptHandler) MATH4_Intr_Handler , (void*) Math4
	) != XST_SUCCESS) {
		print("Failed to register PRR MATH4  ISR exiting ..\r\n");
		return XST_FAILURE;
	}



	/*
	 * The interrupt bits are not for FSL interface.
	 */
	if (MboxInstPtr->Config.UseFSL == 0)
		XMbox_SetInterruptEnable(MboxInstPtr, XMB_IX_RTA); //XMB_IX_STA| XMB_IX_RTA | XMB_IX_ERR


	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts. Specify real mode so that
	 * the Mbox can generate interrupts through
	 * the interrupt controller
	 */
	Status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupt for the Mbox
	 */
	XIntc_Enable(IntcInstancePtr, MboxIntrId);

	for (i=0 ; i<PRR_NUMBER; i++) {
		XIntc_Enable(IntcInstancePtr, PRRs[i].HW.IntID);
	}




#ifndef TESTAPP_GEN
	Xil_ExceptionInit();

	Xil_ExceptionEnable();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XIntc_InterruptHandler, IntcInstancePtr);
#endif /* TESTAPP_GEN */

	return XST_SUCCESS;
}


