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

#define SCHED_I_EN 0
#define SCHED_III_EN 0
#define SIMPLE_SCHED_II 1

#define NO_OF_PRRS 5
#define NO_OF_GPPS 1

#define MAX_PR_MODULES 5
#define MAX_TASKS_TYPES 8
#define MAX_NO_OF_NODES 200
#define YES 1
#define NO 0

#define MAX_QUEUE_TASKS 256

#define SW_HW_MIG 1




#define DEBUG_PRINT 0




#define CONFIG_TIME 20

/*
 * TASK Operation execution time.
 */
#define TASK_1_HW_DELAY 200U  //  Same HW/SW
#define TASK_1_SW_DELAY (TASK_1_HW_DELAY )
#define TASK_2_HW_DELAY 200U  //   HW/0.75SW
#define TASK_2_SW_DELAY (TASK_2_HW_DELAY /2)
#define TASK_3_HW_DELAY 800U  //  HW/1.5 SW
#define TASK_3_SW_DELAY (TASK_3_HW_DELAY *1.2 )
#define TASK_4_HW_DELAY 200U  //  HW /2.5 SW
#define TASK_4_SW_DELAY (TASK_4_HW_DELAY *3 )
#define TASK_5_HW_DELAY 150U
#define TASK_5_SW_DELAY (TASK_5_HW_DELAY*2 )
#define TASK_6_HW_DELAY 100U
#define TASK_6_SW_DELAY (TASK_6_HW_DELAY *4)
#define TASK_7_HW_DELAY 300U
#define TASK_7_SW_DELAY (TASK_7_HW_DELAY )
#define TASK_8_HW_DELAY 150U
#define TASK_8_SW_DELAY (TASK_8_HW_DELAY )
#define TASK_9_HW_DELAY 200U
#define TASK_9_SW_DELAY (TASK_9_HW_DELAY )
#define TASK_10_HW_DELAY 200U
#define TASK_10_SW_DELAY (TASK_10_HW_DELAY )





#endif /* PLATFROMCONFIG_H_ */
