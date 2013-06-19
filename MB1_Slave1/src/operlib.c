/*
 * oper_lib.c
 *
 *  Created on: Jun 1, 2011
 *      Author: ahmed
 */

#include <stdio.h>
#include "operlib.h"

void DelayMS (int ms)
{
	unsigned long  i =0;
	while(i++<ms*4000);
}


int addI(int op1, int op2)
{
	printf ("before adding  %lud \r\n ",sizeof(unsigned long));
	DelayMS(ADDER_DELAY);
	print ("After adding\r\n ");
	return op1+op2;
}

int subI(int op1, int op2)
{	DelayMS(SUB_DELAY);
	return op1-op2;
}
int mulI(int op1, int op2)
{	DelayMS(MUL_DELAY);
	return op1*op2;
}
int divI(int op1, int op2)
{	DelayMS(DIV_DELAY);
	if (!op2)
	{

		return 0;
		print(" divide by zero error ... /r/n");
	}
	return op1/op2;
}

int shlI(int op1, int op2)
{	DelayMS(SHIFTR_DELAY);
	return op1>>op2;
}

int negI(int op1)
{	DelayMS(NEG_DELAY);
	return ~op1;
}
