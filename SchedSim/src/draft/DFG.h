/*
 * DFG.C
 *
 *  Created on: Jun 19, 2011
 *      Author: ahmed
 */

#ifndef DFG_C_
#define DFG_C_
#include <stdio.h>
#include "configuration.h"
//#define RUNNING 1
//#define IDLE 0

//struct ExecUnit {
//        int id;
//        u8 state;
//        u32 result;
//};
//
////extern struct ExecUnit PR1;
//extern struct ExecUnit MB1 ;




/***********************************************************************
 *		Getpr Function returns an ASCII code (character ) for the operation
 *		@param op . the operation to be translated
 *		@return character of the operation
 *		@note  none
 *
 *************************************************************************/

char GetOpr(char op);
/***********************************************************************
 *		Sim Function Simulate the operation
 *		@param op1,op2  first and second operands respectively
 			op is the operation.
 *		@return the result of perfoming the operation
 *		@note  none
 *
 *************************************************************************/

int Sim(int op1,int op2, char op);


/***********************************************************************
 *		print_DFG Prints the DFG nodes with the current results
 		and call Sim function to validate the results
 		It prints PASS or FAIL on each nodes if the validation passed/failed

 *		@param DFG pointer to the DFG graph
 *		@return nothing
 *		@note  none
 *
 *************************************************************************/
void print_DFG(void);

void Init_DFG();
#endif /* DFG_H_ */
