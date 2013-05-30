/*
 * data.c
 *
 *  Created on: Jun 1, 2011
 *      Author: ahmed
 */
#include "data.h"
#include "PlatformConfig.h"
/*FIXME */
static struct nodeData TasksTable[MAX_NO_OF_NODES];
static int TaskCounter=0;
struct TaskType TasksTypes[MAX_TASKS_TYPES];
enum SystemStates State=Start;

struct node *dfg1;



void initTasksTable( int numberOfTasks)
{
	int i;

	for (i=0;i<numberOfTasks;i++)
	{
		TasksTable[i].Done=NO;
		TasksTable[i].Q=NO;

	}
}

void reinitTasksTable( int numberOfTasks)
{
	int i;

	for (i=0;i<numberOfTasks;i++)
	{
		TasksTable[i].Done=NO;
		TasksTable[i].Q=NO;
		TasksTable[i].result=0;
		TasksTable[i].Sim.Reused=0;
		TasksTable[i].Sim.PRRUsed=0;

		TasksTable[i].Sim.ConfigTime.start=0;
		TasksTable[i].Sim.ConfigTime.end=0;


		TasksTable[i].Sim.ExecTime.start=0;
		TasksTable[i].Sim.ExecTime.end=0;



		TasksTable[i].Sim.ExecTV.start=0;
		TasksTable[i].Sim.ExecTV.end=0;


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

	value->ConfigTime.start=TasksTable[ID].Sim.ConfigTime.start;
	value->ConfigTime.end=TasksTable[ID].Sim.ConfigTime.end;


	value->ExecTime.start=TasksTable[ID].Sim.ExecTime.start;
	value->ExecTime.end=TasksTable[ID].Sim.ExecTime.end;



	value->ExecTV.start=TasksTable[ID].Sim.ExecTV.start;
	value->ExecTV.end=TasksTable[ID].Sim.ExecTV.end;
}


void setTaskSimulation (int ID,struct Simulation *value)
{

		TasksTable[ID].Sim.Reused=value->Reused;
		TasksTable[ID].Sim.PRRUsed=value->PRRUsed;

		TasksTable[ID].Sim.ConfigTime.start=value->ConfigTime.start;
		TasksTable[ID].Sim.ConfigTime.end=value->ConfigTime.end;


		TasksTable[ID].Sim.ExecTime.start=value->ExecTime.start;
		TasksTable[ID].Sim.ExecTime.end=value->ExecTime.end;



		TasksTable[ID].Sim.ExecTV.start=value->ExecTV.start;
		TasksTable[ID].Sim.ExecTV.end=value->ExecTV.end;
}



void Init_TasksTypes(void)
{
	int i,k;
	for (i =0;i<MAX_TASKS_TYPES;i++)
	{
		TasksTypes[i].ID=i;
		TasksTypes[i].name=0;
		TasksTypes[i].SWET=0;
		TasksTypes[i].HWET=0;
		TasksTypes[i].SWPriority=MAX_PR_MODULES/2 ;
		for (k=0;k<MAX_PR_MODULES;k++)
		{
			TasksTypes[i].ConfigTime[k]=0;
		}
		TasksTypes[i].CanRun=0XFF;
		TasksTypes[i].Module=1;

	}
}



