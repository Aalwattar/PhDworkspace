#ifndef DATA_H_
#define DATA_H_

#include "PlatformConfig.h"






enum Mode {SWOnly=0x0001,HWOnly=0x0002,HybSW=0x0003,HybHW=0x0013, CustomHW=0x0004,
	CustomHWnSW=0x0005
};
enum Operations {OpAdd,OpSub,OPMult,OpShift,OpBlank};

enum PRModules {adder,sub,mult,shift,blank};
enum PRRID {Math0,Math1,Math2,Math3,Math4};


struct TimerTime
{
	unsigned  start;
	unsigned  end;
};

struct Simulation
{
	enum PRRID PRRUsed;
	unsigned char  Reused;
	struct TimerTime ConfigTime;
	struct TimerTime ExecTime;

};

/*
 * store time delay for software and hardware
 * TODO delete this
 */
struct Emulation
{
	unsigned int  HWdelay;
	unsigned int  SWdelay;
};
struct data {
	int op1;
	int op2;
	unsigned  isAdd_op1 : 1;
	unsigned  isAdd_op2: 1;
	unsigned :0 ;
};
struct PRRProcess {
	int No;
	enum PRRID PRR_ID[MAX_PR_MODULES];
};

/* TODO separate this structure into separate structure one is read only and the other is read/write
 *  Create different files to update the internal properties, instead for the stupid global variable tech.
 *  each functions takes one value which is ID and store or return the appropriate value.
 *  I guess it's better to separate the structures instead of having one for everything.
 *
 *
 * */

struct nodeData
{
	enum Mode mode;
	unsigned int Done ;
	int result;
	unsigned int  Q;
	struct Simulation Sim; // move to a separate struct

};
struct node {
	const unsigned int  id;
	const unsigned char  operation;
	const enum Mode mode;
	const unsigned int next;
	const struct data D;
	const unsigned int  initPrio; // initial priority to start with
	const unsigned int  CanRun;
	const int TypeID;
	const struct Emulation Emu;

};

struct TaskType{
	int ID;
	char * name;
	unsigned int  SWET;
	unsigned int  HWET;
	int SWPriority;
	int ConfigTime[MAX_PR_MODULES];
	unsigned int  CanRun;
	enum PRModules Module;

};

struct tasksTableToken
{
	struct node *dfg;
	struct taskData *TaskTable;
	void * token;
	int ID;
	int size;
};
struct DFG {
	int size;
	struct node dfg[MAX_NO_OF_NODES];
};

enum SystemStates {Start,CfgDone,TaskDone,None};

extern enum SystemStates State;
extern struct node *dfg1;
extern struct DFG DFGArray[];
//extern struct TaskType TasksTypes[];

void Init_TasksTypes(void);
void reinitTasksTable( int numberOfTasks);
void freeTasksTable(void);
inline unsigned char  isTaskDone(int ID);
inline void taskDone(int ID);
inline unsigned char  isTaskQed(int ID);
inline void taskQed(int ID);
inline int getTaskResult( int ID);
inline void setTaskResult (int ID,int value);
void getTaskSimulation( int ID,struct Simulation *value);
void setTaskSimulation (int ID,struct Simulation *value);
inline enum Mode getTaskMode( int ID);
inline void setTaskMode (int ID,enum Mode value);
inline void setTaskCounter(int value);
inline void decTaskCounter(void);
inline int getTaskCounter(void);
void setTaskSimExecTimeStart(int ID, unsigned int value );
void setTaskSimExecTimeEnd(int ID, unsigned int value );
void setTaskSimConfTimeStart(int ID, unsigned int value );
void setTaskSimConfTimeEnd(int ID, unsigned int value );
void setTaskSimReused(int ID, unsigned char value );
void setTaskSimPrrUsed(int ID, enum PRRID value );
inline unsigned int getTaskTypeHWET( int ID);
inline void setTaskTypeHWET (int ID,unsigned int  value);
inline unsigned int getTaskTypeSWET( int ID);
inline void setTaskTypeSWET (int ID,unsigned int value);
inline  int getTaskTypeSWPrio( int ID);
inline void setTaskTypeSWPrio (int ID,int value);
inline unsigned  int getTaskTypeCanRun( int ID);
inline void setTaskTypeCanRun (int ID,unsigned int value);

#endif /* DATA_H_ */
