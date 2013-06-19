/*
 * mbox_lib.c
 *
 *  Created on: May 31, 2011
 *      Author: Ahmed Al-Wattar
 */
/***************************** Include Files **********************************/
#include "xmbox.h"
#include "xstatus.h"
#include "xparameters.h"
#include <stdio.h>
#include "platform_config.h"
#include "mbox_lib.h"
/***************** Macros (Inline Functions) Definitions *********************/

#define printf xil_printf	/* A smaller footprint printf */
char *Role[2] = { "Master",
		         "Slave" };


int mbox_init(XMbox * Mbox)
{
	XMbox_Config *ConfigPtr;
	int Status;

	 printf ("(%s):\tStarts.\r\n", Role[MY_CPU_ID]);

		/*
		 * Lookup configuration data in the device configuration table.
		 * Use this configuration info down below when initializing this
		 * component.
		 */
		ConfigPtr = XMbox_LookupConfig(MBOX_DEVICE_ID);
		if (ConfigPtr == (XMbox_Config *)NULL) {
			printf ("(%s):\tLookupConfig Failed.%8.8x\r\n",
				Role[MY_CPU_ID], MBOX_DEVICE_ID	 );
			return XST_FAILURE;
		}

		/*
		 * Perform the rest of the initialization
		 */
		Status = XMbox_CfgInitialize(Mbox, ConfigPtr, ConfigPtr->BaseAddress);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}
 return XST_SUCCESS;
}

