#ifndef DATA_H_
#define DATA_H_

#include "PlatformConfig.h"




#define TASK_1_HW_DELAY 20LU  // add Same HW/SW
#define TASK_1_SW_DELAY (TASK_1_HW_DELAY )
#define TASK_2_HW_DELAY 20LU  // sub  HW/0.75SW
#define TASK_2_SW_DELAY (TASK_2_HW_DELAY /2)
#define TASK_3_HW_DELAY 80LU  // mult HW/1.5 SW
#define TASK_3_SW_DELAY (TASK_3_HW_DELAY *1.2 )
#define TASK_4_HW_DELAY 20LU  // shift HW /2.5 SW
#define TASK_4_SW_DELAY (TASK_4_HW_DELAY *3 )
#define TASK_5_HW_DELAY 15LU  // add
#define TASK_5_SW_DELAY (TASK_5_HW_DELAY*2 )
#define TASK_6_HW_DELAY 10LU  // add
#define TASK_6_SW_DELAY (TASK_6_HW_DELAY *4)
#define TASK_7_HW_DELAY 30LU  // add
#define TASK_7_SW_DELAY (TASK_7_HW_DELAY )
#define TASK_8_HW_DELAY 15LU  // add
#define TASK_8_SW_DELAY (TASK_8_HW_DELAY )
#define TASK_9_HW_DELAY 20LU  // add
#define TASK_9_SW_DELAY (TASK_9_HW_DELAY )
#define TASK_10_HW_DELAY 20LU  // add
#define TASK_10_SW_DELAY (TASK_10_HW_DELAY )


enum Mode {SWOnly=0x0001,HWOnly=0x0002,HybSW=0x0003,HybHW=0x0013, CustomHW=0x0004,
	CustomHWnSW=0x0005
};
enum Operations {OpAdd,OpSub,OPMult,OpShift,OpBlank};

enum PRModules {adder,sub,mult,shift,blank};
enum PRRID {Math0,Math1,Math2,Math3,Math4};


struct TimerTime
{
	unsigned long start;
	unsigned long end;
};

struct Simulation
{
	enum PRRID PRRUsed;
	unsigned char  Reused;
	struct TimerTime ConfigTime;
	struct TimerTime ExecTime;
	struct TimerTime ExecTV;

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
/* TODO DFG must be read from external memory
 *  A agreed structure must be developed
 *  and specification for the file format
 */
enum SystemStates {Start,CfgDone,TaskDone,None};

extern enum SystemStates State;

extern struct node *dfg1;

extern struct DFG DFGArray[];

extern struct TaskType TasksTypes[];

void Init_TasksTypes(void);
void initTasksTable( int nubmerOfTasks);
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

#endif /* DATA_H_ */
