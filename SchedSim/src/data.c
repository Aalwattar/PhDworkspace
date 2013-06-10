/*
 * data.c

 *
 *  Created on: Jun 1, 2011
 *      Author: ahmed
 */
#include <stdlib.h>
#include <stdio.h>
#include "data.h"
#include "PlatformConfig.h"
/*FIXME */
static struct nodeData TasksTable[MAX_NO_OF_NODES];
static int TaskCounter=0;
static struct TaskType TasksTypes[MAX_TASKS_TYPES];
enum SystemStates State=Start;

struct node *dfg1;





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

/*
 * TaskType set/ get functions
 * TODO check the ID range and throw an error
 */

inline unsigned int getTaskTypeHWET( int ID)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 1\n");
		exit(EXIT_FAILURE);
	}
	return TasksTypes[ID].HWET;
}

inline void setTaskTypeHWET (int ID,unsigned int  value)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 2\n");
		exit(EXIT_FAILURE);
	}
	TasksTypes[ID].HWET=value;
}


inline unsigned int getTaskTypeSWET( int ID)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 3\n");
		exit(EXIT_FAILURE);
	}
	return TasksTypes[ID].SWET;
}

inline void setTaskTypeSWET (int ID,unsigned int value)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 4\n");
		exit(EXIT_FAILURE);
	}
	TasksTypes[ID].SWET=value;
}


inline  int getTaskTypeSWPrio( int ID)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 5\n");
		exit(EXIT_FAILURE);
	}
	return TasksTypes[ID].SWPriority;
}

inline void setTaskTypeSWPrio (int ID,int value)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 6\n");
		exit(EXIT_FAILURE);
	}
	TasksTypes[ID].SWPriority=value;
}

inline unsigned  int getTaskTypeCanRun( int ID)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 7\n");
		exit(EXIT_FAILURE);
	}
	return TasksTypes[ID].CanRun;
}

inline void setTaskTypeCanRun (int ID,unsigned int value)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range [%d] 8\n",ID);
		exit(EXIT_FAILURE);
	}
	TasksTypes[ID].CanRun=value;
}

/*
 *  Task Table
 */


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


}

void setTaskSimExecTimeStart(int ID, unsigned int value )
{
	TasksTable[ID].Sim.ExecTime.start=value;

}

void setTaskSimExecTimeEnd(int ID, unsigned int value )
{
	TasksTable[ID].Sim.ExecTime.end=value;
}

void setTaskSimConfTimeStart(int ID, unsigned int value )
{
	TasksTable[ID].Sim.ConfigTime.start=value;


}

void setTaskSimConfTimeEnd(int ID, unsigned int value )
{
		TasksTable[ID].Sim.ConfigTime.end=value;

}


void setTaskSimReused(int ID, unsigned char value )
{
	TasksTable[ID].Sim.Reused=value;


}


void setTaskSimPrrUsed(int ID, enum PRRID value )
{

	TasksTable[ID].Sim.PRRUsed=value;

}


//
///*
// * This function is so dangerous , use it carefully.
// * Should not be uses.
// */
//void setTaskSimulation (int ID,struct Simulation *value)
//{ static int lock=0;
//
//if (lock){
//	fprintf(stderr,"ERROR [setTaskSimulation] Trying to access locked data\n");
//	exit(EXIT_FAILURE);
//}
//
//fprintf(stderr, "WARNING [setTaskSimulation] dangerous function should not be used\n ");
//		lock=1;
//
//		TasksTable[ID].Sim.Reused=value->Reused;
//		TasksTable[ID].Sim.PRRUsed=value->PRRUsed;
//
//		TasksTable[ID].Sim.ConfigTime.start=value->ConfigTime.start;
//		TasksTable[ID].Sim.ConfigTime.end=value->ConfigTime.end;
//
//
//		TasksTable[ID].Sim.ExecTime.start=value->ExecTime.start;
//		TasksTable[ID].Sim.ExecTime.end=value->ExecTime.end;
//
//		lock=0;
//
//}



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



