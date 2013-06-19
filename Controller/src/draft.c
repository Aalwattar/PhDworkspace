
//
//
//	{
//			  .id=   5 , .operation= OpAdd, .mode =HybHW , .next=  6 ,.initPrio=0,  .TypeID=1,.CanRun=0x1F,
//			  .D={.op1=  2 ,.op2=  4 , .isAdd_op1= NO, .isAdd_op2=YES },
//			  .Emu={.HWdelay=TASK_1_HW_DELAY, .SWdelay=TASK_1_SW_DELAY}
//		},
//
//		{
//			  .id=   4 , .operation= OpSub, .mode =SWOnly , .next=  5 ,.initPrio=0,  .TypeID=2,.CanRun=0x00,
//			  .D={.op1=  2 ,.op2=  1 , .isAdd_op1= NO, .isAdd_op2= NO },
//			  .Emu={.HWdelay=TASK_2_HW_DELAY, .SWdelay=TASK_2_SW_DELAY}
//		},
//
//
//		{
//			  .id=   3 , .operation= OPMult, .mode =HWOnly , .next=  4 ,.initPrio=0,  .TypeID=3,.CanRun=0x0F,
//			  .D={.op1=  2 ,.op2=  1 , .isAdd_op1= NO, .isAdd_op2= NO },
//			  .Emu={.HWdelay=TASK_3_HW_DELAY, .SWdelay=TASK_3_SW_DELAY}
//		},
//
//		{
//			  .id=   1 , .operation= OpAdd, .mode =HybHW , .next=  2 ,.initPrio=0,  .TypeID=4,.CanRun=0x03,
//			  .D={.op1=  2 ,.op2=  0 , .isAdd_op1= NO, .isAdd_op2=YES },
//			  .Emu={.HWdelay=TASK_4_HW_DELAY, .SWdelay=TASK_4_SW_DELAY}
//		},
//
//		{
//			  .id=   1 , .operation= OpAdd, .mode =HybHW , .next=  2 ,.initPrio=0,  .TypeID=5,.CanRun=0x07,
//			  .D={.op1=  2 ,.op2=  1 , .isAdd_op1= NO, .isAdd_op2= NO },
//			  .Emu={.HWdelay=TASK_5_HW_DELAY, .SWdelay=TASK_5_SW_DELAY}
//		},
//
//
//
//		{
//			  .id=   0 , .operation= OpAdd, .mode =HybHW , .next=  1 ,.initPrio=0,  .TypeID=6,.CanRun=0x01,
//			  .D={.op1=  2 ,.op2=  1 , .isAdd_op1= NO, .isAdd_op2= NO },
//			  .Emu={.HWdelay=TASK_6_HW_DELAY, .SWdelay=TASK_6_SW_DELAY}
//		},
//
//		{
//			  .id=   9 , .operation= OpSub, .mode =HybHW , .next= 10 ,.initPrio=0,  .TypeID=7,.CanRun=0x0F,
//			  .D={.op1=  3 ,.op2=  1 , .isAdd_op1=YES, .isAdd_op2= NO },
//			  .Emu={.HWdelay=TASK_7_HW_DELAY, .SWdelay=TASK_7_SW_DELAY}
//		},
//
//
//		{
//			  .id=   2 , .operation= OPMult, .mode =HybHW , .next=  3 ,.initPrio=0,  .TypeID=8,.CanRun=0x1F,
//			  .D={.op1=  2 ,.op2=  1 , .isAdd_op1= NO, .isAdd_op2= NO },
//			  .Emu={.HWdelay=TASK_8_HW_DELAY, .SWdelay=TASK_8_SW_DELAY}
//		},








/*
 * draft.c
 *
 *  Created on: Jun 19, 2011
 *      Author: ahmed
 */

/*
void MATH_Intr_DefaultHandler(void * PRR_Data) {
	//Xuint32 baseaddr;
	Xuint32 IntrStatus;
	Xuint32 IpStatus;
	struct PRRIntData * PRR_Int_Data=(struct PRRIntData *) PRR_Data;

	//baseaddr = (Xuint32) baseaddr_p;
	//baseaddr = PRR_Int_Data->Address;
	printf (" PRR_Data ID= %d and Address %x \r\n", PRR_Int_Data->ID, PRR_Int_Data->Address);

	 * Get status from Device Interrupt Status Register.

	IntrStatus = MATH_mReadReg( PRR_Int_Data->Address, MATH_INTR_DISR_OFFSET);

	printf("Device Interrupt! PRR [%s] DISR value : 0x%08x \n\r", PRRs[PRR_Int_Data->ID].name,(unsigned int )IntrStatus);
	//PR1.result = Xil_In32( PRR_Int_Data->Address);
	dfg1[PRRs[PRR_Int_Data->ID].DFGID].result =(int ) Xil_In32( PRR_Int_Data->Address);
	printf("Result: %X\n\r", dfg1[PRRs[PRR_Int_Data->ID].DFGID].result);
	//PR1.state = IDLE;


	 *  The PRR wont be accessable during that time

	PRRs[PRR_Int_Data->ID].Busy=NO;

	 * Verify the source of the interrupt is the user logic and clear the interrupt
	 * source by toggle write baca to the IP ISR register.

	if ((IntrStatus & INTR_IPIR_MASK) == INTR_IPIR_MASK) {
		xil_printf("User logic interrupt! \n\r");
		IpStatus = MATH_mReadReg( PRR_Int_Data->Address, MATH_INTR_IPISR_OFFSET);
		MATH_mWriteReg( PRR_Int_Data->Address, MATH_INTR_IPISR_OFFSET, IpStatus);

	}

}
*/

/*
int getNumber (){

	Xuint8 byte;
	Xuint8 uartBuffer[16];
	Xboolean validNumber;
	int digitIndex;
	int digit, number, sign;
	int c;

	while(1){
		byte = 0x00;
		digit = 0;
		digitIndex = 0;
		number = 0;
		validNumber = XTRUE;

		//get bytes from uart until RETURN is entered

		while(byte != 0x0d){
			byte = XUartLite_RecvByte(XPAR_UARTLITE_0_BASEADDR );
			uartBuffer[digitIndex] = byte;
			XUartLite_SendByte(XPAR_UARTLITE_0_BASEADDR ,byte);
			digitIndex++;
		}

		//calculate number from string of digits

		for(c = 0; c < (digitIndex - 1); c++){
			if(c == 0){
				//check if first byte is a "-"
				if(uartBuffer[c] == 0x2D){
					sign = -1;
					digit = 0;
				}
				//check if first byte is a digit
				else if((uartBuffer[c] >> 4) == 0x03){
					sign = 1;
					digit = (uartBuffer[c] & 0x0F);
				}
				else
					validNumber = XFALSE;
			}
			else{
				//check byte is a digit
				if((uartBuffer[c] >> 4) == 0x03){
					digit = (uartBuffer[c] & 0x0F);
				}
				else
					validNumber = XFALSE;
			}
			number = (number * 10) + digit;
		}
		number *= sign;
		if(validNumber == XTRUE){
			return number;
		}
		print("This is not a valid number.\n\r");
	}
}

void get_operands(void)
{
	int first, second, result;

	print("First operand: ");
	first = getNumber();
	print("\r\n");
	print("Second operand: ");
	second = getNumber();
	print("\r\n");
	Xil_Out32(XPAR_MATH_0_BASEADDR,first);
	Xil_Out32(XPAR_MATH_0_BASEADDR+4,second);
	result=Xil_In32(XPAR_MATH_0_BASEADDR);
	xil_printf("Result: %d\n\r",result);
	print("\r\n");
}
void menu(void)
{
	 xil_printf("--------------------------------------------------------\r\n");
	 xil_printf("      Press m or M for multiplication configuration\n\r");
	 xil_printf("      Press i or I for shifting configuration\n\r");
	 xil_printf("      Press s or S for subtraction configuration\n\r");
	 xil_printf("      Press a or A for addition configuration\n\r");
	 xil_printf("      Press b or B for blanking configuration\n\r");
	 xil_printf("      Press t or T for test\n\r");
	 xil_printf("      Press o or O to enter operands and display result\n\r");
	 xil_printf("      Press q or Q to quit the demo\n\r");
}*/

/*
volatile u32 tmp1=444,tmp2=4,i;
volatile u32 tmp3;
for (i=0 ; i<100000000;i++)
{
//	tmp1=444;
//			tmp2=4;
	  Xil_Out32(MATH0_BASEADDRESS,tmp1);
	        Xil_Out32(MATH0_BASEADDRESS+4,tmp2);
//	        //Xil_Out32(XPAR_MATH_0_BASEADDR+8,0x30000000);
	        tmp3=Xil_In32(XPAR_MATH_0_BASEADDR);
//	        //xil_printf("Result: %X\n\r",result1);
//			tmp3=tmp1+tmp2;
}
*/


  /*do {










        	if (MB1.state ==IDLE  && PRRs[Math0].Busy == NO && PRRs[Math1].Busy == NO &&
                   PRRs[Math2].Busy == NO && PRRs[Math3].Busy == NO && PRRs[Math4].Busy == NO	)

                {

                        PRR_T.Module= msgS[1] = dfg1[i].operation;

                        msgS[2] = dfg1[i].D.isAdd_op1 ? dfg1[dfg1[i].D.op1].result
                                  : dfg1[i].D.op1;
                        msgS[3] = dfg1[i].D.isAdd_op2 ? dfg1[dfg1[i].D.op2].result
                                  : dfg1[i].D.op2;


                        FIXME rewrite everything here
                        switch (dfg1[i].mode) {

                        case SW:
                                FIXME change the MB.state to yes/no
                               if (MB1.state== RUNNING || IsNodeReady(i)== NO)
                              // {		print ("skip sw \r\n");
                                        break;
                              // }
                                simT[RECON][i].Sec=0;
                                simT[RECON][i].ThSec=0;
                                MB1.state = RUNNING;
                                MB1.id=i;
                                print("SW operation \r\n");
                                 TODO change to non blocking one
                                XMbox_WriteBlocking(&Mbox, (u32*) (msgS), 16);
                                printf(" op1 %ld  operation %ld op2 %ld \r\n", msgS[2],
                                       msgS[1], msgS[3]);
                                i = dfg1[i].next;
                                break;
                        case CustomHW:

                                if (dfg1[i].PRRP.No==1 ) {
                                        PRR_T.PRR_ID=dfg1[i].PRRP.PRR_ID[0];

                                } else if (dfg1[i].PRRP.No!=0) {
                                        print ("This feature is not supported only one PRR is allowed taking the first one \r\n");
                                } else {
                                        print ("DFG error PRR Number can't be zero ... Exiting\r\n ");
                                        return XST_FAILURE;
                                }

                                if (PRRs[PRR_T.PRR_ID].Busy|| (IsNodeReady(i)== NO)) break;
                                	//{
                                	//print ("skip HW \r\n");
                                	//break ;
                                //}
                                PRRs[PRR_T.PRR_ID].DFGID=i;
                                PRRs[PRR_T.PRR_ID].Busy= YES;

                               // print("HW accelerator \r\n");
                                GetTime(&tmpTV);
                                XIntc_Stop(&IntcInst);
                                Config(&PRR_T, &HwIcap);
                                XIntc_Start(&IntcInst, XIN_REAL_MODE);
                                GetElapsedTime(&tmpTV,&simT[RECON][i]);
                                HWMath(msgS[2], msgS[3],PRR_T.PRR_ID);


                                i = dfg1[i].next;
                                break;
                        case HW:
                        case Any:
                        case CustomHWnSW:
                        default:
                                print ("Unsupported mode check you DFG file .. Exiting\r\n");
                                return XST_FAILURE;
                        }




                }

        } while (i);*/
        /*TODO change this , totally stupid */
      //  while (MB1.state == RUNNING || PRRs[0].Busy || PRRs[1].Busy || PRRs[2].Busy || PRRs[3].Busy || PRRs[4].Busy );

        /************************************
         *  Stop the timer
         **********************************/
