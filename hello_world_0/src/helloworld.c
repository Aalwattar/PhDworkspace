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

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#define MAX 5
static int PRRsRandArray[MAX];

//this fucntion randomize the locations of PRRs
// it expects external array PRRsRandArray[MAX_NUMBER_OF_PRRs)
void init_rand_prr(int no)
{	int max=no;

	int rnd,i,k=0;
	for (i=0;i<max;i++)
		PRRsRandArray[i]=-1;

	for (i=0;i<max;i++)
	{
		while(PRRsRandArray[rnd=rand()%max]!=-1);
		PRRsRandArray[rnd]=k;
		k++;

	}

}
int main()
{ int i;
    init_platform();

    print("Hello World 0 First one \n\r");
    init_rand_prr(MAX);
    for (i=0;i<MAX;i++)
    	xil_printf("%d, ",PRRsRandArray[i]);
	print("\n\r");
    cleanup_platform();

    return 0;
}
