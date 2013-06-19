/*****************************************************************************
* Filename:          /home/ahmed/works/PHD_RS/sched/PR_test/edk/drivers/math_v2_00_a/src/math.h
* Version:           2.00.a
* Description:       math Driver Header File
* Date:              Wed Jun 15 12:56:15 2011 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef MATH_H
#define MATH_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"
#include "xparameters.h"
#include "headers.h"

/************************** Constant Definitions ***************************/

;

struct ExecUnit {
      //  int id;
      //  u8 state;
      //u32 result;

				const char * name;
               const u8 ID;
               enum PRModules Current;
               u8 Busy;
               Xuint32 DFGID;
};

//extern struct ExecUnit PR1;
extern struct ExecUnit MB1 ;



/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 * -- SLV_REG2 : user logic slave module register 2
 */
#define MATH_USER_SLV_SPACE_OFFSET (0x00000000)
#define MATH_SLV_REG0_OFFSET (MATH_USER_SLV_SPACE_OFFSET + 0x00000000)
#define MATH_SLV_REG1_OFFSET (MATH_USER_SLV_SPACE_OFFSET + 0x00000004)
#define MATH_SLV_REG2_OFFSET (MATH_USER_SLV_SPACE_OFFSET + 0x00000008)

/**
 * Software Reset Space Register Offsets
 * -- RST : software reset register
 */
#define MATH_SOFT_RST_SPACE_OFFSET (0x00000100)
#define MATH_RST_REG_OFFSET (MATH_SOFT_RST_SPACE_OFFSET + 0x00000000)

/**
 * Software Reset Masks
 * -- SOFT_RESET : software reset
 */
#define SOFT_RESET (0x0000000A)

/**
 * Interrupt Controller Space Offsets
 * -- INTR_DISR  : device (peripheral) interrupt status register
 * -- INTR_DIPR  : device (peripheral) interrupt pending register
 * -- INTR_DIER  : device (peripheral) interrupt enable register
 * -- INTR_DIIR  : device (peripheral) interrupt id (priority encoder) register
 * -- INTR_DGIER : device (peripheral) global interrupt enable register
 * -- INTR_ISR   : ip (user logic) interrupt status register
 * -- INTR_IER   : ip (user logic) interrupt enable register
 */
#define MATH_INTR_CNTRL_SPACE_OFFSET (0x00000200)
#define MATH_INTR_DISR_OFFSET (MATH_INTR_CNTRL_SPACE_OFFSET + 0x00000000)
#define MATH_INTR_DIPR_OFFSET (MATH_INTR_CNTRL_SPACE_OFFSET + 0x00000004)
#define MATH_INTR_DIER_OFFSET (MATH_INTR_CNTRL_SPACE_OFFSET + 0x00000008)
#define MATH_INTR_DIIR_OFFSET (MATH_INTR_CNTRL_SPACE_OFFSET + 0x00000018)
#define MATH_INTR_DGIER_OFFSET (MATH_INTR_CNTRL_SPACE_OFFSET + 0x0000001C)
#define MATH_INTR_IPISR_OFFSET (MATH_INTR_CNTRL_SPACE_OFFSET + 0x00000020)
#define MATH_INTR_IPIER_OFFSET (MATH_INTR_CNTRL_SPACE_OFFSET + 0x00000028)

/**
 * Interrupt Controller Masks
 * -- INTR_TERR_MASK : transaction error
 * -- INTR_DPTO_MASK : data phase time-out
 * -- INTR_IPIR_MASK : ip interrupt requeset
 * -- INTR_RFDL_MASK : read packet fifo deadlock interrupt request
 * -- INTR_WFDL_MASK : write packet fifo deadlock interrupt request
 * -- INTR_IID_MASK  : interrupt id
 * -- INTR_GIE_MASK  : global interrupt enable
 * -- INTR_NOPEND    : the DIPR has no pending interrupts
 */
#define INTR_TERR_MASK (0x00000001UL)
#define INTR_DPTO_MASK (0x00000002UL)
#define INTR_IPIR_MASK (0x00000004UL)
#define INTR_RFDL_MASK (0x00000020UL)
#define INTR_WFDL_MASK (0x00000040UL)
#define INTR_IID_MASK (0x000000FFUL)
#define INTR_GIE_MASK (0x80000000UL)
#define INTR_NOPEND (0x80)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a MATH register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the MATH device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void MATH_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define MATH_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a MATH register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the MATH device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 MATH_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define MATH_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from MATH user logic slave registers.
 *
 * @param   BaseAddress is the base address of the MATH device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void MATH_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 MATH_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define MATH_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (MATH_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MATH_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (MATH_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MATH_mWriteSlaveReg2(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (MATH_SLV_REG2_OFFSET) + (RegOffset), (Xuint32)(Value))

#define MATH_mReadSlaveReg0(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (MATH_SLV_REG0_OFFSET) + (RegOffset))
#define MATH_mReadSlaveReg1(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (MATH_SLV_REG1_OFFSET) + (RegOffset))
#define MATH_mReadSlaveReg2(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (MATH_SLV_REG2_OFFSET) + (RegOffset))

/**
 *
 * Reset MATH via software.
 *
 * @param   BaseAddress is the base address of the MATH device.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void MATH_mReset(Xuint32 BaseAddress)
 *
 */
#define MATH_mReset(BaseAddress) \
 	Xil_Out32((BaseAddress)+(MATH_RST_REG_OFFSET), SOFT_RESET)

/************************** Function Prototypes ****************************/


/**
 *
 * Enable all possible interrupts from MATH device.
 *
 * @param   baseaddr_p is the base address of the MATH device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void MATH_EnableInterrupt(void * baseaddr_p);

/**
 *
 * Example interrupt controller handler.
 *
 * @param   baseaddr_p is the base address of the MATH device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
//void MATH_Int_DefaultHandler(int );

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the MATH instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus MATH_SelfTest(void * baseaddr_p);

inline Xuint32 CalcuateExecTime(Xuint32 orig,Xuint32 new);
void CalcSWPrio(u32 ID);
void MATH0_Intr_Handler(void*);
void MATH1_Intr_Handler(void *);
void MATH2_Intr_Handler(void *);
void MATH3_Intr_Handler(void *);
void MATH4_Intr_Handler(void *);

#endif /** MATH_H */
