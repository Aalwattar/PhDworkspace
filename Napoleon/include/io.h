/*************************************************************************/
// Author  : Ziad Abuowaimer
// Version : 0.1
// Project : A DFG Off-Line Task Scheduler for FPGA
// Team    : Ziad Abuowaimer
/*************************************************************************/

/*************************************************************************/
// Filename: io.h
// Description:
// Header file which defines the functions defined in io.c
/*************************************************************************/

/*This file contains the function declarations in the file io.c*/

extern void print_error(int);
extern int parse_aif(FILE *, t_task *, t_task_interface *);
extern int parse_res(FILE *, t_task_type *);
extern void display_array(short int *, short int, short int *);
extern void display_task(t_task *, t_task_interface *);
extern void display_task_type(t_task_type *);
extern void set_task_parameter(t_task *, t_task_type *);

