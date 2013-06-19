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

#define MBOX_DEVICE_ID		XPAR_MBOX_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define MBOX_INTR_ID		XPAR_INTC_0_MBOX_0_INTERRUPT_0_VEC_ID


#endif
