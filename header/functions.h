/*************************************************************************/
// Author  : Ziad Abuowaimer
// Version : 0.1
// Project : A DFG Off-Line Task Scheduler for FPGA
// Team    : Ziad Abuowaimer
/*************************************************************************/

/*************************************************************************
 Filename: functions.h
 Description:
  This file contains the function declarations in the file functions.c, 
  which are referred to in main.c

*************************************************************************/

extern int create_graph(t_task *, t_task_interface *,   int *);
extern int create_reuse_mat(t_task *,   int *);

