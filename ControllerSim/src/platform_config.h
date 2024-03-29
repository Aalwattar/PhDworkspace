

#ifndef __PLATFORM_CONFIG_H_
#define __PLATFORM_CONFIG_H_


#ifdef __PPC__
#define CACHEABLE_REGION_MASK 0x80008000
#endif

#if XPAR_CPU_ID != 0
#define MY_CPU_ID 1
#else
#define MY_CPU_ID XPAR_CPU_ID
#endif /* XPAR_CPU_ID != 0 */

//////////////////////////NO O PRRs/////////////////
#define AVAILABLE_PRR 5// NO OF PRRS
///////////////////////////////////////////
#define SW_PE_ID (AVAILABLE_PRR)
#define EXEC_TIME_LEARNING_RATIO 0.2
//#define BITSTREAMS_START_ADDRESS (0x83d00000+0x80000)
//#define BITSTREAMS_END_ADDRESS (0x83E00000L-0x800L)
#define BITSTREAMS_START_ADDRESS (0x50000000+0x1000000)
#define BITSTREAMS_END_ADDRESS (0x50000000L+0x1000000+0x500000-0x800L)


#define MAX_TASKS_TYPES 16
#define CONFIG_FROM_DDR 0

#define DEBUG_PRINT 0

#define MAX_NO_OF_NODES 200
#define NO_OF_DFGS	1 ///// THIS IS THE NUMBER OF DFGS NOT PRRS PAY ATTENTION
#define INDEPENDENT_DFGS 1 // clear internal tables for next DFG( different one)
#define NO_OF_DFG_REP 6 // iteration per dfg


#define CONF_REUSE 0  // Sched I ** DELETE THIS AND ALL THE RELATED CODE
#define SIMPLE_SCHED_II_REUSE 1 // Enable Reuse of SCHED III AS WELL
#define SIMPLE_SCHED_III_REUSE 1 // MUST be enabled for SCHED II REUSE to WORK
#define SIMPLE_SCHED_I_REUSE 0
#define SCHED_II_WORSTCASE 0 // pick the biggest prr first
#define SCHED_II_RANDOM 1	// pick random prr, if both are disabled the best case will be instead

#define SCHED_I_RANDOM 1 // radomize the PRRs (if this is zero the it will be automatically worstcase
#define SCHED_I_WORSTCASE 0 // worst case
#define SIMPLE_SCHED_II 1
#define SCHED_III_EN  0
#define SCHED_I_EN  0// enable sched I
#define SIMPLE_SCHED_SER 0 // either enable the SER or II or III ** OBSELETE DELETE IT AND ALL RELATED CODE
#define SW_HW_MIG 1


#define MBOX_DEVICE_ID		XPAR_MBOX_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define MBOX_INTR_ID		XPAR_INTC_0_MBOX_0_VEC_ID

#define TMRCTR_DEVICE_ID	XPAR_TMRCTR_0_DEVICE_ID
#define TMRCTR_INTERRUPT_ID	XPAR_INTC_0_TMRCTR_0_VEC_ID

#define MAILBOX_RIT	16	/* mailbox receive interrupt threshold */
#define MAILBOX_SIT	4	/* mailbox send interrupt threshold */

//#define MATH_INTR_ID  		XPAR_XPS_INTC_0_MATH_0_IP2INTC_IRPT_INTR
//#define MATH_DEVICE_ID		XPAR_MATH_0_BASEADDR

#define MATH0_BASEADDRESS XPAR_MATH_0_BASEADDR
#define MATH1_BASEADDRESS XPAR_MATH_1_BASEADDR
#define MATH2_BASEADDRESS XPAR_MATH_2_BASEADDR
#define MATH3_BASEADDRESS XPAR_MATH_3_BASEADDR
#define MATH4_BASEADDRESS XPAR_MATH_4_BASEADDR

#define MATH0_INT_ID	XPAR_XPS_INTC_0_MATH_0_IP2INTC_IRPT_INTR
#define MATH1_INT_ID	XPAR_XPS_INTC_0_MATH_1_IP2INTC_IRPT_INTR
#define MATH2_INT_ID	XPAR_XPS_INTC_0_MATH_2_IP2INTC_IRPT_INTR
#define MATH3_INT_ID	XPAR_XPS_INTC_0_MATH_3_IP2INTC_IRPT_INTR
#define MATH4_INT_ID	XPAR_XPS_INTC_0_MATH_4_IP2INTC_IRPT_INTR
//extern int TasksDoneCounter; /*TODO get rid of this global shit*/
#endif
