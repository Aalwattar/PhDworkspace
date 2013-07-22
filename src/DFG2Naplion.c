/*
 ============================================================================
 Name        : DFG2Naplion.c
 Author      : Ahmed Al-Wattar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "parameters.h"
#include "data.h"

enum DepType {REG='r',OUT='o',INP='c'};
/*
 * TODO to be completely cleaned up.
 *
 */
struct output
	{
		int node;
		char * nodeName[10];
		int dep_id;
	};
	struct iotype
	{
	enum DepType type;
		int node;
	};
struct NodeStruct
{
	struct iotype input[2];
	struct iotype output;
	int size;
};


void init(struct NodeStruct *nd)
{
	int i;
	for (i=0;i<MAX_NO_OF_NODES;i++)
	{
		nd[i].input[0].node=-1;
		nd[i].input[0].type=0;

		nd[i].input[1].node=-1;
		nd[i].input[1].type=0;

		nd[i].output.node=-1;
		nd[i].output.type=0;



	}
}
int main(void) {
	int i=0,k=0;
	int isoutput[MAX_NO_OF_NODES];
	int count;
	char *outstr="r200_3333";
 struct NodeStruct ndar[MAX_NO_OF_NODES];


	struct output outp[MAX_NO_OF_NODES];

	int isreg[MAX_NO_OF_NODES][2];
	struct node * DFG=DFGArray[0].dfg;





/* init ndarr node array */
 init(ndar);

 count =0 ;
	printf("inputs ");

	do {

		isoutput[i]=YES;
	isreg[i][0]=NO;
	isreg[i][1]=NO;
    outp[i].node=-1;
    outp[i].dep_id=-1;
    //isoutput=YES;
    	if (DFG[i].D.isAdd_op1 && DFG[i].D.isAdd_op2)
    		{
    		isreg[i][0]=YES;
    		isreg[i][1]=YES;
    		continue ;
    		}
    	if (!DFG[i].D.isAdd_op1)
    	{
    	 printf("c%d 8 ",count);
    	 ndar[i].input[0].node=count;
    	 ndar[i].input[0].type=INP;
    	 isreg[i][1]=YES;
    	 count ++;
    	}
    	if(!DFG[i].D.isAdd_op2)
         {printf("c%d 8 ",count);
         isreg[i][0]=YES;

         ndar[i].input[1].node=count;
         ndar[i].input[1].type=INP;
         count++;
         }

    } while (DFG[i++].next);

  printf("\n");
////////////////////
 i=0;
 count =0;
 printf("outputs ");
 do {
		k=0;
		do {

			if (!DFG[k].D.isAdd_op1 && !DFG[k].D.isAdd_op2) continue ;

	    	if (DFG[k].D.isAdd_op1   &&  DFG[k].D.op1==i )
	    		{
	    		isoutput[i]=NO;
	    		ndar[i].output.type=REG;
	    		break;
	    		}

	    	if (DFG[k].D.isAdd_op2   &&  DFG[k].D.op2==i )
	    		{
	    		isoutput[i]=NO;
	    		ndar[i].output.type=REG;
	    		break;
	    		}


	    } while (DFG[k++].next);


  	if (isoutput[i])
  	{
  		printf("o%d 8 ",count);
  		ndar[i].output.node=count;
  		ndar[i].output.type=OUT;
  		count++;
  	}

  } while (DFG[i++].next);
 printf("\n");

 //////////////////////////////
i=0;
count=0;
	printf("regs ");

	do {



 	if (!DFG[i].D.isAdd_op1 && !DFG[i].D.isAdd_op2) continue ;

 	if (DFG[i].D.isAdd_op1)
 	{


 	 if ( ndar[DFG[i].D.op1].output.node <0)
 	 {
 		ndar[DFG[i].D.op1].output.node=count;
 		ndar[i].input[0].node=count;
 		ndar[i].input[0].type=REG;
 		printf("r%d 8 ",count);
 		count++;

 	 }
 	 else
 	 {
 		ndar[i].input[0].node=ndar[DFG[i].D.op1].output.node;
 		ndar[i].input[0].type=REG;
 	 }
// 	 outp[DFG[i].D.op1].node=i;
// 	outp[DFG[i].D.op1].dep_id=1;

 	}
 	if(DFG[i].D.isAdd_op2)
 	{

 	 	 if (ndar[DFG[i].D.op2].output.node <0)
 	 	 {
 	 		ndar[DFG[i].D.op2].output.node= count ;
 	 		ndar[i].input[1].node=count;
 	 		ndar[i].input[1].type=REG;
 	 		printf("r%d 8 ",count);
 	 		count++;
 	 	 }
 	 	 else
 	 	 {
 	 		ndar[i].input[1].node=ndar[DFG[i].D.op2].output.node;
 	 		ndar[i].input[1].type=REG;
 	 	 }

//      printf("r%d_2 8 ",i);
//      outp[DFG[i].D.op1].node=i;
//      	outp[DFG[i].D.op1].dep_id=2;
 	}

 } while (DFG[i++].next);

printf("\n");
////////////////////

i=0;

	do {

		printf("T%d TASK%d 8 %c%d %c%d %c%d\n",i, DFG[i].TypeID, ndar[i].input[0].type,ndar[i].input[0].node,
				ndar[i].input[1].type,ndar[i].input[1].node,ndar[i].output.type,ndar[i].output.node);


  } while (DFG[i++].next);

printf("end \n");


	return EXIT_SUCCESS;
}
