/*
 * data.c
 *
 *  Created on: Jun 1, 2011
 *      Author: ahmed
 */
#include "xstatus.h"
#include "data.h"
/*FIXME */
static struct nodeData TasksTable[MAX_NO_OF_NODES];
static int TaskCounter=0;
struct TaskType TasksTypes[MAX_TASKS_TYPES];
enum SystemStates State=Start;

struct node *dfg1;
//struct node *DFGArray[];


void initTasksTable( int numberOfTasks)
{
	int i;
//	TasksTable=(struct nodeData *) calloc(numberOfTasks,sizeof(struct nodeData));
//	if (TasksTable==NULL)
//
//	{
//		print("initTasksTable: ERRROR allocating memory \n EXITING \n");
//		exit(1);
//	}
//
	for (i=0;i<numberOfTasks;i++)
	{
		TasksTable[i].Done=NO;
		TasksTable[i].Q=NO;

	}
}

void reinitTasksTable( int numberOfTasks)
{
	int i;
//	TasksTable=(struct nodeData *) calloc(numberOfTasks,sizeof(struct nodeData));
//	if (TasksTable==NULL)
//
//	{
//		print("initTasksTable: ERRROR allocating memory \n EXITING \n");
//		exit(1);
//	}
//
	for (i=0;i<numberOfTasks;i++)
	{
		TasksTable[i].Done=NO;
		TasksTable[i].Q=NO;
		TasksTable[i].result=0;
		TasksTable[i].Sim.Reused=0;
		TasksTable[i].Sim.PRRUsed=0;

		TasksTable[i].Sim.ConfigTime.Min=0;
		TasksTable[i].Sim.ConfigTime.Sec=0;
		TasksTable[i].Sim.ConfigTime.ThSec=0;

		TasksTable[i].Sim.ExecTime.Min=0;
		TasksTable[i].Sim.ExecTime.Sec=0;
		TasksTable[i].Sim.ExecTime.ThSec=0;


		TasksTable[i].Sim.ExecTV.ExpNo=0;
		TasksTable[i].Sim.ExecTV.Value=0;


	}
}
inline void setTaskCounter(int value)
{
	TaskCounter=value;
}
inline void decTaskCounter(void)
{
	TaskCounter--;
}
inline int getTaskCounter(void)
{
	return TaskCounter;
}

inline void freeTasksTable(void)
{
	//free(TasksTable);
}

inline unsigned char  isTaskDone(int ID)
{
	return TasksTable[ID].Done;
}

inline void taskDone(int ID)
{
	 TasksTable[ID].Done=YES;
}

inline unsigned char  isTaskQed(int ID)
{
	//printf("quee %d\r\n",ID);
	return TasksTable[ID].Q;
}
inline void taskQed(int ID)
{
	TasksTable[ID].Q=YES;
}

inline int getTaskResult( int ID)
{
	return TasksTable[ID].result;
}

inline void setTaskResult (int ID,int value)
{
	TasksTable[ID].result=value;
}

//inline u32 getTaskPrio( int ID)
//{
//	return TasksTable[ID].prio;
//}
//
//inline void setTaskPrio (int ID,u32 value)
//{
//	TasksTable[ID].prio=value;
//}

inline enum Mode getTaskMode( int ID)
{
	return TasksTable[ID].mode;
}

inline void setTaskMode (int ID,enum Mode value)
{
	TasksTable[ID].mode=value;
}

void getTaskSimulation( int ID,struct Simulation *value)
{
	value->Reused=TasksTable[ID].Sim.Reused ;
	value->PRRUsed=TasksTable[ID].Sim.PRRUsed ;

	value->ConfigTime.Min=TasksTable[ID].Sim.ConfigTime.Min;
	value->ConfigTime.Sec=TasksTable[ID].Sim.ConfigTime.Sec;
	value->ConfigTime.ThSec=TasksTable[ID].Sim.ConfigTime.ThSec;

	value->ExecTime.Min=TasksTable[ID].Sim.ExecTime.Min;
	value->ExecTime.Sec=TasksTable[ID].Sim.ExecTime.Sec;
	value->ExecTime.ThSec=TasksTable[ID].Sim.ExecTime.ThSec;


	value->ExecTV.ExpNo=TasksTable[ID].Sim.ExecTV.ExpNo;
	value->ExecTV.Value=TasksTable[ID].Sim.ExecTV.Value;
}


void setTaskSimulation (int ID,struct Simulation *value)
{

		TasksTable[ID].Sim.Reused=value->Reused;
		TasksTable[ID].Sim.PRRUsed=value->PRRUsed;

		TasksTable[ID].Sim.ConfigTime.Min=value->ConfigTime.Min;
		TasksTable[ID].Sim.ConfigTime.Sec=value->ConfigTime.Sec;
		TasksTable[ID].Sim.ConfigTime.ThSec=value->ConfigTime.ThSec;

		TasksTable[ID].Sim.ExecTime.Min=value->ExecTime.Min;
		TasksTable[ID].Sim.ExecTime.Sec=value->ExecTime.Sec;
		TasksTable[ID].Sim.ExecTime.ThSec=value->ExecTime.ThSec;


		TasksTable[ID].Sim.ExecTV.ExpNo=value->ExecTV.ExpNo;
		TasksTable[ID].Sim.ExecTV.Value=value->ExecTV.Value;
}



void Init_TasksTypes(void)
{
	int i,k;
	for (i =0;i<MAX_TASKS_TYPES;i++)
	{
		TasksTypes[i].ID=i;
		TasksTypes[i].name=NULL;
		TasksTypes[i].SWET=0;
		TasksTypes[i].HWET=0;
		TasksTypes[i].SWPriority=SW_PE_ID/2 ;
		for (k=0;k<MAX_PR_MODULES;k++)
		{
			TasksTypes[i].ConfigTime[k]=0;
		}
		TasksTypes[i].CanRun=0XFF;
		TasksTypes[i].Module=adder;

	}
}



//  __attribute__ ((aligned (4))){0,0,1,1,{1,5,0,0},0},{1,3,1,3,{8,1,0,0},0},{2,2,1,4,{8,0,0,1},0} ,{3,2,0,2,{5,1,0,1},0},{4,0,0,0,{3,2,1,1},0}};
