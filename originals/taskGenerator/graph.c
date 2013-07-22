/*
 * graph.c
 *
 *  Created on: Jun 20, 2012
 *      Author: Ahmed Al-Wattar
 */

#include <stdio.h>
#include <stdlib.h>
#include  "dfggen.h"
#include "graph.h"


#define HEADER "digraph finite_state_machine { \n\
				rankdir=UD;\n \
				size=\"3,1\" \n \
				label=\"\\n\\n\\n Task graph 1 \\n Ahmed Al-Wattar\\n\" \n\n\
		node [shape = circle, fillcolor=lightblue, style=filled];\n"

#define FOOTER "}\n"

int writeIndependentNodes(FILE * fp,struct genmatrix * matstruct)
{	int i,k, counter=0;
	int ind;
	int err;

	for (i=0;i<matstruct->size;i++)
	{
		if (matstruct->hasRelation[i]==0)
		{	ind=1;
			for (k=0; i< matstruct->dim ;k++)
			{
				if(matstruct->matrix[i][k]!=0)
						{
							ind=0;
							break;
						}

			}
			if (ind)
			{
				err=fprintf(fp,"\t\tTask_%d ;\n",i);
				if (err<0)
				{
					fprintf(stderr,"writeIndependentNodes: Error Writing into file \n Exiting \n");
					exit(1);
				}
				counter++;
			   // fprintf(stdout,"\t\tTask_%d ;\n",i);
			}
		}


	}
 return counter;
}

int writeNodesRelations(FILE * fp,struct genmatrix * matstruct)
{	int i,k, counter=0;
	int err;

	for (i=0;i<matstruct->size;i++)
	{


		for (k=0; k< matstruct->dim ;k++)
		{
			if(matstruct->matrix[i][k])
			{

				err=fprintf(fp,"\t\tTask_%d -> Task_%d ;\n",matstruct->matrix[i][k]-1,i);
				if (err<0)
				{
					fprintf(stderr,"writeINodesRelations: Error Writing into file \n Exiting \n");
					exit(1);
				}
				counter++;

			}

		}



}
return counter;
}


void writeNodesTypes(FILE * fp,struct genmatrix * matstruct)
{	int i;
	char label[128];
	int err;
	int id;

	for (i=0;i<matstruct->size;i++)
	{
		id=matstruct->types[i];
		sprintf(label,"[%d]\\n%s",i,GTasksTypes.task[id].name);

		err=fprintf(fp,"\t\tTask_%d [shape=oval, label=\"%s\" , fillcolor=\"%s\"];\n"
						,i,label,GTasksTypes.task[id].color);
		if (err<0)
		{
			fprintf(stderr,"writeINodesTypes: Error Writing into file \n Exiting \n");
			exit(1);
		}


	}

};





void writeGraphDot(FILE *fp, struct genmatrix * matstruct)
{
	fprintf(fp,HEADER);
	writeNodesTypes(fp,matstruct);
	writeIndependentNodes(fp, matstruct);
	writeNodesRelations(fp,matstruct);
	fprintf(fp,FOOTER);
}
