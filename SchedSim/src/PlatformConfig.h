/*
 * PlatfromConfig.h
 *
 *  Created on: 2013-05-24
 *      Author: aalwatta
 */

#ifndef PLATFROMCONFIG_H_
#define PLATFROMCONFIG_H_

#define NO_OF_DFG_REP 1
#define NO_OF_DFGS 1
#define INDEPENDENT_DFGS 0

#define RCS_SCHED_I 0
#define RCS_SCHED_III 1
#define RCS_SCHED_II 0

#define NO_OF_PRRS 5
#define NO_OF_GPPS 3

#define MAX_PR_MODULES 5
#define MAX_TASKS_TYPES 8
#define MAX_NO_OF_NODES 200
#define YES 1
#define NO 0

#define MAX_QUEUE_TASKS 256

#define SW_HW_MIG 1

#define EXEC_TIME_LEARNING_RATIO 0.2


#define DEBUG_PRINT 0





/*
 * TASK Operation execution time.
 */
#define TASK_1_HW_DELAY 20U  //  Same HW/SW
#define TASK_1_SW_DELAY (TASK_1_HW_DELAY )
#define TASK_2_HW_DELAY 20U  //   HW/0.75SW
#define TASK_2_SW_DELAY (TASK_2_HW_DELAY /2)
#define TASK_3_HW_DELAY 80U  //  HW/1.5 SW
#define TASK_3_SW_DELAY (TASK_3_HW_DELAY *1.2 )
#define TASK_4_HW_DELAY 20U  //  HW /2.5 SW
#define TASK_4_SW_DELAY (TASK_4_HW_DELAY *3 )
#define TASK_5_HW_DELAY 15U
#define TASK_5_SW_DELAY (TASK_5_HW_DELAY*2 )
#define TASK_6_HW_DELAY 10U
#define TASK_6_SW_DELAY (TASK_6_HW_DELAY *4)
#define TASK_7_HW_DELAY 30U
#define TASK_7_SW_DELAY (TASK_7_HW_DELAY )
#define TASK_8_HW_DELAY 15U
#define TASK_8_SW_DELAY (TASK_8_HW_DELAY )
#define TASK_9_HW_DELAY 20U
#define TASK_9_SW_DELAY (TASK_9_HW_DELAY )
#define TASK_10_HW_DELAY 20U
#define TASK_10_SW_DELAY (TASK_10_HW_DELAY )

/*
 *FIXME PRR Configuration time for each PRR
 * DO NOT FORGET TO ADD THOSE to the init of the ConfigTime[] Array in schedulers.c
 * if you increase the number of PRR add them to the ConfiTime[] Array.
 *
 */
#define PRR_0_CONFIG_TIME 20
#define PRR_1_CONFIG_TIME 30
#define PRR_2_CONFIG_TIME 50
#define PRR_3_CONFIG_TIME 70
#define PRR_4_CONFIG_TIME 20
/*
#define PRR_5_CONFIG_TIME 20
#define PRR_6_CONFIG_TIME 20
#define PRR_7_CONFIG_TIME 20
#define PRR_8_CONFIG_TIME 20
*/

#endif /* PLATFROMCONFIG_H_ */
