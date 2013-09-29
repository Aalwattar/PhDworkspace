/*
 * tasksTemplate.h
 *
 *  Created on: Jun 14, 2012
 *      Author: ahmed
 */

#ifndef TASKSTEMPLATE_H_
#define TASKSTEMPLATE_H_

#define TASKS_TYPES 16
//#define MAX_NODES 100

const char *TasksTemplate[TASKS_TYPES] ={
	"\t\t{"
	"\n\t\t\t  .id= %01 , .operation= OpAdd, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=1,.CanRun=0x1F,"
	"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
	"\n\t\t\t  .Emu={.HWdelay=TASK_1_HW_DELAY, .SWdelay=TASK_1_SW_DELAY}"
	"\n\t\t}"
,
"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpSub, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=2,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_2_HW_DELAY, .SWdelay=TASK_2_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OPMult, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=3,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_3_HW_DELAY, .SWdelay=TASK_3_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpAdd, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=4,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_4_HW_DELAY, .SWdelay=TASK_4_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpAdd, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=5,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_5_HW_DELAY, .SWdelay=TASK_5_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpAdd, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=6,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_6_HW_DELAY, .SWdelay=TASK_6_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpSub, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=7,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_7_HW_DELAY, .SWdelay=TASK_7_SW_DELAY}"
"\n\t\t}"
,
"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OPMult, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=8,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_8_HW_DELAY, .SWdelay=TASK_8_SW_DELAY}"
"\n\t\t}"

,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpAdd, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=9,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_1_HW_DELAY, .SWdelay=TASK_1_SW_DELAY}"
"\n\t\t}"
,
"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpSub, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=10,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_2_HW_DELAY, .SWdelay=TASK_2_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OPMult, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=11,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_3_HW_DELAY, .SWdelay=TASK_3_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpAdd, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=12,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_4_HW_DELAY, .SWdelay=TASK_4_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpAdd, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=13,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_5_HW_DELAY, .SWdelay=TASK_5_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpAdd, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=14,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_6_HW_DELAY, .SWdelay=TASK_6_SW_DELAY}"
"\n\t\t}"
,

"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OpSub, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=15,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_7_HW_DELAY, .SWdelay=TASK_7_SW_DELAY}"
"\n\t\t}"
,
"\t\t{"
"\n\t\t\t  .id= %01 , .operation= OPMult, .mode =HybHW , .next=%02 ,.initPrio=0,  .TypeID=16,.CanRun=0x1F,"
"\n\t\t\t  .D={.op1=%03 ,.op2=%04 , .isAdd_op1=%05, .isAdd_op2=%06 },"
"\n\t\t\t  .Emu={.HWdelay=TASK_8_HW_DELAY, .SWdelay=TASK_8_SW_DELAY}"
"\n\t\t}"
};


#define F_ID "%01"
#define F_NEXT "%02"
#define F_OP1 "%03"
#define F_OP2 "%04"
#define F_ISADD_OP1 "%05"
#define F_ISADD_OP2 "%06"

#endif
