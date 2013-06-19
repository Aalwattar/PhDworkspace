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

	XUartLite_Initialize(&UartLite, XPAR_UARTLITE_0_DEVICE_ID);
	////////
	print("starting  Slave...... \n\r");
	if (mbox_init (&Mbox)!=XST_SUCCESS)
	{
		print ("Xbox initialization failure ... Exiting /r/n");
		return XST_FAILURE;
	}



	while(1)
	{
		printf ("CPU 1 ReadBlocking call\n");
		XMbox_ReadBlocking (&Mbox, (u32*)(RecvMsg), 16);

		printf ("(%s):\tSuccessfully Rcvd the message --> \r\n\r\n\t--[%ld %ld %ld %ld ]--\r\n\r\n", Role[MY_CPU_ID], RecvMsg[0],RecvMsg[1],RecvMsg[2],RecvMsg[3]);

		result=get_data(RecvMsg);


		/* Write a message to the Mbox */

		printf ("CPU 1 WriteBlocking call\n");
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

	if (Msg[0]!=1111)
	{
		print("unknown packet ...\r\n");
		return 0;
	}


	switch (DFGNode.operation)

	{
	case 0:

		DFGNode.result = (int) addI(DFGNode.op1,DFGNode.op2 );

		break;
	case 1:
		DFGNode.result = (int) subI(DFGNode.op1,DFGNode.op2 );

		break;
	case 2:

		DFGNode.result = (int) mulI(DFGNode.op1,DFGNode.op2 );

		break;
	case 3:

		DFGNode.result = (int) shlI(DFGNode.op1,DFGNode.op2 );

		break;
	case 4:
		DFGNode.result = (int) divI(DFGNode.op1,DFGNode.op2 );


		break;
	case 5:

		DFGNode.result = (int) negI(DFGNode.op1);

		break;
	default:
		print ("Unknown operators .\r\n");
		return 0;

	}
	printf( " oper1 [%d] operation [%ld] oper2 [%d] and result =[%d] \r\n", DFGNode.op1,DFGNode.operation,DFGNode.op2,DFGNode.result);
	return DFGNode.result;
}
