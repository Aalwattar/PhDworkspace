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



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "platform.h"
#include "xmbox.h"
#include "xstatus.h"
#include "xparameters.h"
#include "mbox_lib.h"
#include "xuartlite.h"
#include "operlib.h"

struct Dnode
{
	u32 operation;
	int op1;
	int op2;
	int result;
	unsigned long delay;
};

u32 get_data (u32 *);
XUartLite UartLite;
int main()
{


	//  Variables definition
	XMbox Mbox;

	u32 result;
	int MsgSize = 4;

	u32  *RecvMsg=(u32 *) malloc(sizeof (u32) * MsgSize);
	/// initialization
	init_platform();
#if DEBUG_PRINT
	XUartLite_Initialize(&UartLite, XPAR_UARTLITE_0_DEVICE_ID);
#endif
	////////

#if DEBUG_PRINT
	print("starting  Slave...... \n\r");
#endif
	if (mbox_init (&Mbox)!=XST_SUCCESS)
	{
#if DEBUG_PRINT
		print ("Xbox initialization failure ... Exiting /r/n");
#endif
		return XST_FAILURE;
	}



	while(1)
	{
#if DEBUG_PRINT
		printf ("CPU 1 ReadBlocking call\n");
#endif
		XMbox_ReadBlocking (&Mbox, (u32*)(RecvMsg), 20);
#if DEBUG_PRINT
		printf ("(%s):\tSuccessfully Rcvd the message --> \r\n\r\n\t--[%ld %ld %ld %ld %lu ]--\r\n\r\n", Role[MY_CPU_ID], RecvMsg[0],RecvMsg[1],RecvMsg[2],RecvMsg[3],RecvMsg[4]);
#endif
		result=get_data(RecvMsg);


		/* Write a message to the Mbox */
#if DEBUG_PRINT
		printf ("CPU 1 WriteBlocking call\n");
#endif
		XMbox_WriteBlocking(&Mbox, (u32*)&result, 4);

		// Clean up
	}
	cleanup_platform();

	return 0;
}

u32 get_data (u32 * Msg)
{
	struct Dnode DFGNode;
	DFGNode.op1= (int) Msg[2];
	DFGNode.op2 =(int) Msg[3];
	DFGNode.operation = (u32) Msg[1];
	DFGNode.delay=(unsigned long)Msg[4];

	if (Msg[0]!=1111)
	{
#if DEBUG_PRINT
		print("unknown packet ...\r\n");

#endif
		return 0;
	}
	//printf("delay is %ld",DFGNode.delay);

	switch (DFGNode.operation)

	{
	case 0:

		DFGNode.result = (int) addI(DFGNode.op1,DFGNode.op2 ,DFGNode.delay);

		break;
	case 1:
		DFGNode.result = (int) subI(DFGNode.op1,DFGNode.op2,DFGNode.delay );

		break;
	case 2:

		DFGNode.result = (int) mulI(DFGNode.op1,DFGNode.op2,DFGNode.delay );

		break;
	case 3:

		DFGNode.result = (int) shlI(DFGNode.op1,DFGNode.op2 ,DFGNode.delay);

		break;
	case 4:
		DFGNode.result = (int) divI(DFGNode.op1,DFGNode.op2 ,DFGNode.delay);


		break;
	case 5:

		DFGNode.result = (int) negI(DFGNode.op1,DFGNode.delay);

		break;
	default:
#if DEBUG_PRINT
		print ("Unknown operators .\r\n");
#endif
		return 0;

	}
#if DEBUG_PRINT
	printf( " oper1 [%d] operation [%ld] oper2 [%d]  and result =[%d]  with delay =[%lu]\r\n", DFGNode.op1,DFGNode.operation,DFGNode.op2,DFGNode.result,DFGNode.delay);
#endif
	return DFGNode.result;
}
