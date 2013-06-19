/*
 * mbox_lib.h
 *
 *  Created on: May 31, 2011
 *      Author: ahmed
 */

#ifndef MBOX_LIB_H_
#define MBOX_LIB_H_
#include "xintc.h"

#include "xil_exception.h"


int mbox_init(XMbox * ,XMbox_Config *);




/*****************************************************************************/
/**
 *
 * This is the interrupt For the mailbox .
 *
 * @param 	CallBackRef is a callback reference passed in by the upper layer
 *		when setting the interrupt handler, and is passed back to the
 *		upper layer when the interrupt handler is called.
 *
 * @return	None.
 *
 * @note		None.
 *
 ******************************************************************************/
void MailboxIntrHandler(void *CallbackRef);

extern char *Role[2];

#endif /* MBOX_LIB_H_ */
