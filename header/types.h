/*************************************************************************/
// Author  : Ziad Abuowaimer
// Version : 0.1
// Project : A DFG Off-Line Task Scheduler for FPGA
// Team    : Ziad Abuowaimer
/*************************************************************************/

/*************************************************************************/
// Filename: types.h
// Description:
// Header file which defines the types used in this system
/*************************************************************************/

#define __INVALID 0

#define __INPUT    1
#define __OUTPUT   2
#define __REGISTER 3

//These are the various modes for the opqueue function
#define __INIT    0
#define __ENQUEUE 1
#define __DEQUEUE 2

#define __ADD      1
#define __MULT     2
#define __SUB      3
#define __DIV      4
#define __LT       5
#define __GT       6
#define __LTE      7
#define __GTE      8

typedef struct s_config{
 char aif_fname[__SIZE_FILENAME];
 char res_fname[__SIZE_FILENAME];
 char log_fname[__SIZE_FILENAME];
 char opt_fname[__SIZE_FILENAME];
} t_config;

typedef struct s_task{
 char name[__SIZE_TASKNAME];
   int type;
   int exec_sched;
   int reconfig_sched;
   int leftmost_column;
  int bottommost_row;
   int latency;
   int columns;
   int rows;
   int reconfig_time;
   int width;
   int input1;
   int input2;
   int output;
   int impl;
   int reconfig_pwr;
   int exec_pwr;
} t_task;

typedef struct s_task_interface{
 char name[__SIZE_TASK_INTFCNAME];
   int mode;
   int width;
   int reg_out;
} t_task_interface;

typedef struct s_task_Type{
   int latency;
   int reconfig_time;
   int columns;
   int rows;
} t_task_type;

typedef struct s_task_Impl{
   int id;
   int type;
   int impl;
   int latency;
   int reconfig_time;
   int columns;
   int rows;
   int reconfig_pwr;
   int exec_pwr;
} t_task_impl;

