/*
 * DFG.c
 *
 *  Created on: Jun 19, 2011
 *      Author: Ahmed Al-Wattar
 */

/*FIXME they are duplicated in main remove them */
#define PROCESSING 0
#define RECON		1

#include "headers.h"
//#include "timer.h"
/******************************************************
* TODO Move DFG functions to a seperate header file
*		DFG Functions
*
******************************************************/
//struct ExecUnit {
//	int id;
//	u8 state;
//	u32 result;
//};


//struct ExecUnit MB1 = { 0, 0, 0 };



/***********Print the DFG**********************/
//#define  OP1V(x,I) (((x)[D.isAdd_op1) ? (x)[(x)->D.op1].result: (x)->D.op1)
//#define  OP2V(x,I) (((x)->D.isAdd_op2) ? (x)[(x)->D.op2].result: (x)->D.op2)


/***********************************************************************
 *		Getpr Function returns an ASCII code (character ) for the operation
 *		@param op . the operation to be translated
 *		@return character of the operation
 *		@note  none
 *
 *************************************************************************/

char GetOpr(char op)
{
        switch (op) {
        case 0:

                return '+';
        case 1:
                return '-';
        case 2:
                return '*';
        case 3:
                return '>';
        case 4:
                return '/';
        case 5:
                return '~';
        default:
                return '!';

        };
};


/***********************************************************************
 *		Sim Function Simulate the operation
 *		@param op1,op2  first and second operands respectively
 			op is the operation.
 *		@return the result of perfoming the operation
 *		@note  none
 *
 *************************************************************************/

int OperationSim(int op1,int op2, char op)
{
        switch (op) {
        case 0:

                return op1+op2;
        case 1:
                return op1-op2;
        case 2:
                return op1*op2;
        case 3:
                return op1>>op2;
        case 4:
                if (op2==0) {
                        print ("SIM: Devide by zero ERROR \r\n");
                        return 0;
                }

                return op1/op2;

        case 5:
                return ~op1;
        default:
                print ("SIM: Unknown operator\r\n");
                return 0;

        };
};



/***********************************************************************
 *		print_DFG Prints the DFG nodes with the current results
 		and call Sim function to validate the results
 		It prints PASS or FAIL on each nodes if the validation passed/failed

 *		@param DFG pointer to the DFG graph
 *		@return nothing
 *		@note  none
 *
 *************************************************************************/
void print_DFG(void)
{
        struct node * DFG=dfg1;
        int op1, op2;
        int i=0;
        struct Simulation Sim;
        struct TimerTime totalConfT={.Sec=0,.ThSec=0};
        struct TimerTime  totalExecT={.Sec=0,.ThSec=0};
        struct TimerTime  totalEnCT={.Sec=0,.ThSec=0};
        do {
				getTaskSimulation(i,&Sim);
                op1=DFG[i].D.isAdd_op1 ? getTaskResult(DFG[i].D.op1):DFG[i].D.op1;
                op2=DFG[i].D.isAdd_op2 ? getTaskResult(DFG[i].D.op2):DFG[i].D.op2;

                printf ("Node [%2ld] -->%4d %1c%4d =%4d   Sim result [%s]  %s %2lu:%.3lu Exec %2lu:%.3lu PRR[%d] Reuse[%3s] Prio[%d] Type[%d] \r\n",
								DFG[i].id,op1,GetOpr(DFG[i].operation),
								op2,getTaskResult(i),OperationSim(op1,op2,DFG[i].operation)==getTaskResult(i)? "PASS": "FAIL",
                        		Sim.PRRUsed!=SW_PE_ID?"RECONF": "SW COM",
                        		Sim.ConfigTime.Sec, Sim.ConfigTime.ThSec,
                        		Sim.ExecTime.Sec, Sim.ExecTime.ThSec,
                        		//Sim.PRRUsed!=SW_PE_ID?'0'+Sim.PRRUsed:'S',
                        		Sim.PRRUsed,
                        		Sim.Reused?"YES":"NO",
                        		TasksTypes[DFG[i].TypeID].SWPriority,
                        		DFG[i].TypeID	);

                AddTime(&totalConfT,&Sim.ConfigTime);
                AddTime(&totalExecT,&Sim.ExecTime);


        } while (DFG[i++].next);

        AddTime(&totalEnCT,&totalExecT);
        AddTime(&totalEnCT,&totalConfT);

        printf("Total Exec Time [%2lu:%.3lu], Total Conf Time [%2lu:%.3lu] , Total Conf and Exec [%2lu:%.3lu] \r\n",
        		totalExecT.Sec,totalExecT.ThSec,totalConfT.Sec,totalConfT.ThSec,totalEnCT.Sec,totalEnCT.ThSec );

}

//
///***********************************************************************
// *		print_DFG Prints the DFG nodes with the current results
// 		and call Sim function to validate the results
// 		It prints PASS or FAIL on each nodes if the validation passed/failed
//
// *		@param DFG pointer to the DFG graph
// *		@return nothing
// *		@note  none
// *
// *************************************************************************/
//void Init_DFG()
//{
//        struct node * DFG=dfg1;
//
//        int i=0;
//        do {
//
//             DFG[i].Sim.ConfigTime.Sec= DFG[i].Sim.ConfigTime.ThSec=0;
//        } while (DFG[i++].next);
//
//}
