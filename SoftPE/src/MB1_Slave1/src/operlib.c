/*
 * oper_lib.c
 *
 *  Created on: Jun 1, 2011
 *      Author: ahmed
 */

#include <stdio.h>
#include "operlib.h"

inline void DelayMS (unsigned long ms)
{
	volatile unsigned long  i =(unsigned long)(ms/144);
	while(i--);
}


int addI(int op1, int op2 , unsigned long delay)
{
	//printf ("before adding  %lud \r\n ",sizeof(unsigned long));
	DelayMS(delay);
	//print ("After adding\r\n ");
	return op1+op2;
}

int subI(int op1, int op2 , unsigned long delay)
{	DelayMS(delay);
	return op1-op2;
}
int mulI(int op1, int op2, unsigned long delay)
{	DelayMS(delay);
	return op1*op2;
}
int divI(int op1, int op2, unsigned long delay)
{	DelayMS(delay);
	if (!op2)
	{

		return 0;
		print(" divide by zero error ... /r/n");
	}
	return op1/op2;
}

int shlI(int op1, int op2, unsigned long delay)
{	DelayMS(delay);
	return op1>>op2;
}

int negI(int op1, unsigned long delay)
{	DelayMS(delay);
	return ~op1;
}
