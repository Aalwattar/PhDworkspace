/*
 ============================================================================
 Name        : resultsparser.c
 Author      : Ahmed Al-Wattar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Mode {Empty, Reconfig, Exec};
struct node {
	unsigned int ExTime;
	unsigned int ConfTime;
	unsigned int ExecTV;
	unsigned int ConfigTV;
	unsigned int Type;
	unsigned int PRR;
	char Reuse;
	unsigned int ID ;
};

struct Draw {
	enum Mode mode;
	unsigned int Type;
	unsigned int ID;

};


#define SCALING 100

#define MAX_NO_OF_NODES 200

void initDraw(struct Draw   ** graph, int time , int noPRR)
{	int i=0,j;


	for (i=0; i< time; i++)
	{
		graph[i]=(struct Draw *) malloc (sizeof (struct Draw) * noPRR);
		 if (!graph[i])
		    {
		    	printf("ERROR: initDraw  Error allocating memroy \r\n");
		    	exit (1);

		    }
		  for (j=0; j<noPRR; j++)
		  {

			  graph[i][j].ID=0;
			  graph[i][j].Type=0;
			  graph[i][j].mode=Empty;

		  }

	}



}

unsigned int checkMaxSize(struct node DFG [], int size )
{ int i;
	unsigned int max=0, exec=0;
	for (i=0; i< size ; i++  )
	{
		if (max<DFG[i].ConfigTV) max= DFG[i].ConfigTV;
		if (max<DFG[i].ExecTV)
			{max= DFG[i].ExecTV;
			 exec=DFG[i].ExTime;
			}

	}

	return max+exec;
}

void generateGraph(struct Draw **graph, struct node DFG[],int time, int dfgSize)
{

 int i,j;


 for (i=0 ;i < dfgSize ; i++ )
 {
	 if (DFG[i].Reuse)
	 {
		 for (j=DFG[i].ConfigTV/SCALING; j< (DFG[i].ConfigTV+DFG[i].ConfTime)/SCALING; j++)
		 {


			 graph[j][DFG[i].PRR].ID=DFG[i].ID;

			 graph[j][DFG[i].PRR].Type=DFG[i].Type;
			 graph[j][DFG[i].PRR].mode=Reconfig;


		 }
	 }



		 for (j=DFG[i].ExecTV/SCALING; j< (DFG[i].ExecTV+DFG[i].ExTime)/SCALING; j++)
		 {
			 graph[j][DFG[i].PRR].ID=DFG[i].ID;
			 graph[j][DFG[i].PRR].Type=DFG[i].Type;
			 graph[j][DFG[i].PRR].mode=Exec;

		 }

 }
}

void drawGraph(struct Draw **graph, int time)
{

 int i,j;
 int count;
 char *c0="aadfasf";
 char *c1="aadfasf";
 char *c2="aadfasf";
 char *c3="aadfasf";
 char *c4="aadfasf";
 printf("      -PRR1- -PRR2- -PRR3- -PRR4- -PRR5- \n");
 for (i=0 ;i < time ; i++ )
 {



//	sprintf(c0,"%u",graph[i][0].ID);
//	sprintf(c1,"%u",graph[i][1].ID);
//	sprintf(c2,"%u",graph[i][2].ID);
//	sprintf(c3,"%u",graph[i][3].ID);
//	sprintf(c4,"%u",graph[i][4].ID);
	printf("%4d--\t%u%s\t%u%s\t%u%s\t%u%s\t%u%s\n", i+1,
			graph[i][0].ID+1, graph[i][0].mode==Reconfig?"#####":graph[i][0].mode==Exec?"*****":".....",
			graph[i][1].ID+1, graph[i][1].mode==Reconfig?"#####":graph[i][1].mode==Exec?"*****":".....",
			graph[i][2].ID+1, graph[i][2].mode==Reconfig?"#####":graph[i][2].mode==Exec?"*****":".....",
			graph[i][3].ID+1, graph[i][3].mode==Reconfig?"#####":graph[i][3].mode==Exec?"*****":".....",
			graph[i][4].ID+1, graph[i][4].mode==Reconfig?"#####":graph[i][4].mode==Exec?"*****":"....."
										);

//			 graph[j][DFG[i].PRR].ID=DFG[i].ID;
//			 graph[j][DFG[i].PRR].Type=DFG[i].Type;
//			 graph[j][DFG[i].PRR].mode=Reconfig;
//





 }
}

int main(void) {
	enum State {
		NONE, Finishing, FindDFG, FindIteration
	};
	enum State state = FindDFG;
	FILE *fpsrc, *fpdst;
	int time;
	struct node DFG[MAX_NO_OF_NODES];
	char buf[255];
	struct Draw **graph=0;
 int noNodes=0;
 int tmp;
 int i=0;





	fpsrc = fopen("C:/Users/aalwatta.CFS/workspace/DFGdraw/Debug/dfg.txt", "r");

	fpdst = fopen("result0.csv", "a");

	if (!fpsrc || !fpdst) {
		puts("error opening files");
		exit(1);
	}

	while (!feof(fpsrc)) {
		fgets(buf, 255, fpsrc);



		switch(state)
		{


			case FindDFG:

			tmp = sscanf(buf,
					" Processing: DFG[%*d] with [%d] nodes please wait .....",
					&noNodes);

			if (tmp > 0) {

				state = FindIteration;

				printf("Number of Nodes is [%d]\n",noNodes);
				}



				//fprintf(fpdst,
				//		"Iteration,TotalTime,TotalExTime,#of Conf,# of Reuse,ConfTime,HW2SW,Busy,SW Tasks\n");

			break;
			case FindIteration:

		//	do {
			tmp =
					sscanf(buf,
							"T[%u] R[%u] Node [%u] -->  %*d %*c   %*d =  %*d   Sim result [PASS]  RECONF  %*d:%u Exec  %*d:%u PRR[%u] Reuse[%*c%*c%c] Prio[%*d] Type[%u]\n",
							&(DFG[i].ExecTV) , &(DFG[i].ConfigTV),&(DFG[i].ID),&(DFG[i].ConfTime),&(DFG[i].ExTime),&(DFG[i].PRR),&(DFG[i].Reuse),&(DFG[i].Type));
			if (tmp > 0) {
				printf("ExecTV [%5u]  ConfigTV [%5u] ID [%3u] ConfigTime [%3u] ExecTime [%3u] PRR [%1u]  Reuse [%1d]  Type [%1u] \n",DFG[i].ExecTV , DFG[i].ConfigTV,DFG[i].ID,DFG[i].ConfTime,DFG[i].ExTime,DFG[i].PRR,DFG[i].Reuse=='S' ?1:0 ,DFG[i].Type );
				i++;
				break;
			 }


		//	}while (noNodes--);


		printf("Finish processing data \n");
		state = Finishing;
		break;

			case Finishing:
			time=checkMaxSize(DFG,noNodes);
			time =  (time )/SCALING;

			 graph = (struct Draw **) malloc(sizeof (struct Draw *) * time);
			     if (!graph)
			     {
			     	printf("ERROR: initDraw  Error allocating memroy \r\n");
			     	exit (1);

			     }

			initDraw(graph, time, 5);
			printf ("time is %d \n", time );

			generateGraph(graph, DFG, time, noNodes);

		drawGraph(graph, time);
				exit(0);
		default:
		//fprintf(stder, "Uknown STATE !!!\n");
		break;
		}
	}



fclose( fpsrc);
fclose( fpdst);


return 0;
}
