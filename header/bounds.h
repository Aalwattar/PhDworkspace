/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : July 3, 2013
 *************************************************************************/

/*************************************************************************
 * Filename    : bounds.h
 * 
 * Description : Header file which defines the data structure sizes
 *************************************************************************/

#ifndef BOUNDS_H
#define BOUNDS_H

#define __SIZE_FILENAME             500
#define __SIZE_TASKNAME 	    500
#define __SIZE_TASK_INTFCNAME 	    500
#define __SIZE_MAX_BUFF 	    10000
#define __NUM_MAX_TASK_INTFC 	    1000
#define __NUM_MAX_TASKS 	    1000
#define __NUM_MAX_TASK_TYPES 	    9
#define __NUM_MAX_TASK_IMPLS 	    50
#define __NUM_FPGA_COLUMNS 	    5
#define __NUM_FPGA_ROWS 	    5
#define __NUM_FPGA_RECONFIGARATOR   1
#define __SIZE_Q 		    1000

#endif /* BOUNDS_H */