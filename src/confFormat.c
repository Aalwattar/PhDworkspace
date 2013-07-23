/*
 ============================================================================
 Name        : confFormat.h
 Author      : Ahmed Al-Wattar
 Version     :
 Copyright   : Your copyright notice
 Description : convert the dfg to libconfuse's .conf format
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "data.h"
#include "parameters.h"
#include "confFormat.h"

//extern struct DFG DFGArray[NO_OF_DFGS];

void clearNodes(struct confNode *);

int writeConf(char * filename) {
	FILE * fp;
	struct confNode ndar[MAX_NO_OF_NODES]; // IO info for all nodes
	struct output outp[MAX_NO_OF_NODES];
	int isoutput[MAX_NO_OF_NODES];// flags the nodes that don't output to a register
	int isreg[MAX_NO_OF_NODES][2];	// flags the nodes that output to a register
	// FIXME - fix input file!
		struct node * DFG;
			DFG=	DFGArray[0].dfg ;

	// date stuff
	time_t curtime;
	struct tm *loctime;
	char buffer[BUFF_SIZE];

	// indices
	int count;
	int i = 0, k = 0;
	char * outstr = "r200_3333";


	fp = fopen(filename, "w");

	// Name
	fprintf(fp, "Name= \"%s\" \n", filename);

	// Date
	curtime = time(NULL);
	loctime = localtime(&curtime);
	strftime(buffer, BUFF_SIZE, "Date= \"%b %d, %Y\" \n\n", loctime);
	fprintf(fp, buffer);

	/* init ndarr node array */
	clearNodes(ndar); // make non-global somehow

	////////////////////  printing the list of all inputs (one function)

	count = 0;
	fprintf(fp, "inputs = { ");

	do {
		// put into init function
		isoutput[i] = YES;
		isreg[i][0] = NO;
		isreg[i][1] = NO;
		outp[i].node = -1;
		outp[i].dep_id = -1;
		//isoutput=YES;

		if (DFG[i].D.isAdd_op1 && DFG[i].D.isAdd_op2) { // neither are adrress
			isreg[i][0] = YES;
			isreg[i][1] = YES;
			continue;
		}

		// FIXME - logic is funky
		if (!DFG[i].D.isAdd_op1) {
			fprintf(fp, "c%d , ", count);
			ndar[i].input[0].node = count;
			ndar[i].input[0].type = INP;
			isreg[i][1] = YES; // assume second one is an address
			count++;
		}
		if (!DFG[i].D.isAdd_op2) {
			fprintf(fp, "c%d , ", count);
			isreg[i][0] = YES; // then the first is an address

			ndar[i].input[1].node = count;
			ndar[i].input[1].type = INP;
			count++;
		}

	} while (DFG[i++].next);// if 0 then no more nodes - IN THE ARRAY, NOT TREE (nodes stored as array without dependancies, not tree)

	fprintf(fp, "}\n");
////////////////////  printing the list of all inputs (one function)

	////////////////////  printing the list of all outputs (one function)
	i = 0;
	count = 0;
	fprintf(fp, "outputs = { ");
	// bubble sort!

	do {
		k = 0;
		do {

			// check if any other node uses the current node as an input (register / address)
			if (!DFG[k].D.isAdd_op1 && !DFG[k].D.isAdd_op2)
				continue; // the comparison node has no inputs

			if (DFG[k].D.isAdd_op1 && DFG[k].D.op1 == i) {
				isoutput[i] = NO;
				ndar[i].output.type = REG;
				break;
			}

			if (DFG[k].D.isAdd_op2 && DFG[k].D.op2 == i) {
				isoutput[i] = NO;
				ndar[i].output.type = REG;
				break;
			}

		} while (DFG[k++].next);

		if (isoutput[i]) {
			fprintf(fp, "o%d , ", count);
			ndar[i].output.node = count;
			ndar[i].output.type = OUT;
			count++;
		}

	} while (DFG[i++].next);
	fprintf(fp, "}\n");

	////////////////////  printing the list of all outputs (one function)

	////////////////////  printing the list of all registers (one function)
	i = 0;
	count = 0;
	fprintf(fp, "regs = {");

	do {

		if (!DFG[i].D.isAdd_op1 && !DFG[i].D.isAdd_op2)
			continue;

		if (DFG[i].D.isAdd_op1) {

			if (ndar[DFG[i].D.op1].output.node < 0) // output can go to more than one input
					{
				ndar[DFG[i].D.op1].output.node = count;
				ndar[i].input[0].node = count;
				ndar[i].input[0].type = REG;
				fprintf(fp, "r%d , ", count);
				count++;

			} else {
				ndar[i].input[0].node = ndar[DFG[i].D.op1].output.node; // take the register name that has already been initialized
				ndar[i].input[0].type = REG;
			}
// 	 outp[DFG[i].D.op1].node=i;
// 	outp[DFG[i].D.op1].dep_id=1;

		}
		if (DFG[i].D.isAdd_op2) {

			if (ndar[DFG[i].D.op2].output.node < 0) {
				ndar[DFG[i].D.op2].output.node = count;
				ndar[i].input[1].node = count;
				ndar[i].input[1].type = REG;
				fprintf(fp, "r%d , ", count);
				count++;
			} else {
				ndar[i].input[1].node = ndar[DFG[i].D.op2].output.node;
				ndar[i].input[1].type = REG;
			}

//      fprintf("r%d_2 8 ",i);
//      outp[DFG[i].D.op1].node=i;
//      	outp[DFG[i].D.op1].dep_id=2;
		}

	} while (DFG[i++].next);

	fprintf(fp, "}\n\n");
////////////////////  printing the list of all registers (one function)

	i = 0;

	do {

		fprintf(fp, "\ntask T%d{\n"
				"\t type = %d \n"
				"\t inputs = {%c%d ,%c%d }\n"
				"\t output = %c%d\n"
				"}", i, DFG[i].TypeID, ndar[i].input[0].type,
				ndar[i].input[0].node, ndar[i].input[1].type,
				ndar[i].input[1].node, ndar[i].output.type,
				ndar[i].output.node);

	} while (DFG[i++].next);

	fprintf(fp, "\n");

	fclose(fp);

	return EXIT_SUCCESS;
}

void clearNodes(struct confNode * node) {
	int i;

	for (i = 0; i < MAX_NO_OF_NODES; i++) {
		node[i].input[0].node = -1;	// if register, not -1 and instead the index of the node it comes from
		node[i].input[0].type = 0;	// not a valid enum

		node[i].input[1].node = -1;
		node[i].input[1].type = 0;

		node[i].output.node = -1; 	// the node that the output goes to
		node[i].output.type = 0;
	}
}
