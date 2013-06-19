/*

 * timer.c
 *
 *  Created on: Jun 29, 2011
 *      Author: ahmed
 */
#include "headers.h"
#include "xtmrctr.h"
#include "timer.h"
static volatile int TimerExpired=0; /* Stores the # times the timer expired (updated by ISR ONLY)*/
static volatile int TickTimer=0;
/* Shouldn't be accessed directly only through it's associated functions */
static XTmrCtr TmrCtrInstance;   /* Timer Instance (controller ) */

#define TMR_CTR_NO 0			/* Timer number of that instance */
#define RESET_VALUE 0			/* Timer Reset Value */

/* Those function are used to update the TimerExpired Variable */
static u32 GetTimerExp(void);
static void ResetTimerExp(void);
static void IncTimerExp(void);
static void IncTickTimer(void);
static void TickTimerTriggered(void);

/******************************************************************************/
/**
*
* This function Initilizes the Timer controller, Enable the ISR and
*  Configure the Timer for AutoReload
*
* @param	IntcInstancePtr is a reference to the Interrupt Controller
*		driver Instance.

*
* @return	XST_SUCCESS / XST_FAILURE for success or failure respectively
*
* @note		None.
*
******************************************************************************/
int InitTimer(XIntc *intcInstPtr) //(XTmrCtr *TmrCtrInstancePtr,u32 devID, u8 timerNo)
{
        int status;
        // static XTmrCtr TmrCtrInstance;
        XTmrCtr *TmrCtrInstancePtr=&TmrCtrInstance;
        u32 devID = TMRCTR_DEVICE_ID;
        /*
         * Initialize the timer counter so that it's ready to use,
         * specify the device ID that is generated in xparameters.h
         */
        status = XTmrCtr_Initialize(TmrCtrInstancePtr, devID);
        if (status != XST_SUCCESS ) {
        	/*
        	 * status=
        	 * XST_DEVICE_IS_STARTED(5),
        	 * XST_DEVICE_NOT_FOUND (2)
        	 * XST_SUCCESS (0)
        	 *
        	 */
                printf("Timer init Failed [%d] .. \r\n" ,status );
                return XST_FAILURE;
        }

        /*
         * Perform a self-test to ensure that the hardware was built
         * correctly, use the 1st timer in the device (0)
         */
        status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TMR_CTR_NO);
        if (status != XST_SUCCESS) {
                print("Timer Selftest failed check hardware ..\r\n");
                return XST_FAILURE;
        }

        /* Register the Timer interrupt and enable interrupts */
        status = TimerRegInt(intcInstPtr);
        if (status != XST_SUCCESS) {
                print("Timer INT Reg Failed .. \r\n");
                return XST_FAILURE;
        }
        printf("finished init kinda status %d\r\n",status);
        /*
        		 * Setup the handler for the timer counter that will be called from the
        		 * interrupt context when the timer expires, specify a pointer to the
        		 * timer counter driver instance as the callback reference so the handler
        		 * is able to access the instance data
        		 */
        XTmrCtr_SetHandler(TmrCtrInstancePtr, TimerCounterHandler,
                           TmrCtrInstancePtr);

        /*
         * Enable the interrupt of the timer counter so interrupts will occur
         * and use auto reload mode such that the timer counter will reload
         * itself automatically and continue repeatedly, without this option
         * it would expire once only
         */
        XTmrCtr_SetOptions(TmrCtrInstancePtr, TMR_CTR_NO,
                           XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

        XTmrCtr_SetOptions(TmrCtrInstancePtr, 1,
                            XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

        /*
         * Set a reset value for the timer counter such that it will expire
         * eariler than letting it roll over from 0, the reset value is loaded
         * into the timer counter when it is started
         */
        XTmrCtr_SetResetValue(TmrCtrInstancePtr, TMR_CTR_NO, RESET_VALUE);
        XTmrCtr_SetResetValue(TmrCtrInstancePtr, 1, 1000000); //every 10 ms



        return XST_SUCCESS;
}


/******************************************************************************/
/**
*
* This function Register (connect) the timer interrupt and Enable it
*
* @param	IntcInstancePtr is a reference to the Interrupt Controller
*		driver Instance.
*
* @return	XST_SUCCESS / XST_FAILURE for success or failure respectively
*
* @note		None.
*
******************************************************************************/
int TimerRegInt(XIntc *IntcInstancePtr)
{
        int status;

        /*
         * Connect a device driver handler that will be called when an interrupt
         * for the device occurs, the device driver handler performs the specific
         * interrupt processing for the device (for Timer)
         */

        status = XIntc_Connect(IntcInstancePtr, TMRCTR_INTERRUPT_ID,
                               (XInterruptHandler)XTmrCtr_InterruptHandler,
                               (void *) (&TmrCtrInstance));

        if (status != XST_SUCCESS) {
                print("Failed to register  the Timer ISR exiting ..\r\n");
                return XST_FAILURE;
        }

        /*
         * Enable the interrupt for the timer counter
         */
        XIntc_Enable(IntcInstancePtr, TMRCTR_INTERRUPT_ID);
        return XST_SUCCESS;
}

/*****************************************************************************
* This function Adds the time of two TimerValue structures and store the
*  the result in the first timer structure
*
* @param	A pointer to the TimerValue struct that has been initialized by GetTime
* 			function.(result will be stored here)
* @param 	tt TimerValue structure to be added
*
* @return		None
*
* @note		*tv must be initialized first by GetTime function
* 				TimerTime struct has an extra (min) member that has not been used.
*
******************************************************************************/

void AddTime(struct TimerTime *tv, struct TimerTime *tt)
{
	tv->Sec +=tt->Sec;
	if ((tv->ThSec += tt->ThSec)>=1000)
	{
		tv->Sec++;
		tv->ThSec-=1000;
	}

}

/*****************************************************************************/
/**
* This function is the handler which performs processing for the timer counter.
* It is called from an interrupt context such that the amount of processing
* performed should be minimized.  It is called when the timer counter expires
* if interrupts are enabled.
*
* This handler provides an example of how to handle timer counter interrupts
* but is application specific.
*
* @param	CallBackRef is a pointer to the callback function
* @param	TmrCtrNumber is the number of the timer to which this
*		handler is associated with.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void TimerCounterHandler(void *CallBackRef, u8 TmrCtrNumber)
{
        XTmrCtr *InstancePtr = (XTmrCtr *)CallBackRef;

        /*
         * Check if the timer counter has expired, checking is not necessary
         * since that's the reason this function is executed, this just shows
         * how the callback reference can be used as a pointer to the instance
         * of the timer counter that expired, increment a shared variable so
         * the main thread of execution can see the timer expired
         */
        if (XTmrCtr_IsExpired(InstancePtr, TmrCtrNumber)) {
        	if (TmrCtrNumber==0)
                IncTimerExp();		/* Inc the timer counter */
        	else if (TmrCtrNumber==1)
        	{
        		IncTickTimer();
        		TickTimerTriggered();
        	}

                //printf("Timer expired %d \r\n",TimerExpired);
        }




}


/*****************************************************************************
* This function retrieves the Tick Timer Counter It's been used a way to
* hide the variable
*
* @param	None
*
* @return	u32 TimerTick Value
*
* @note		None.
*
******************************************************************************/
u32 GetTickTimer(void)
{
        return TickTimer;
}

/*****************************************************************************
* This function retrieves the TimerExpired Counter It's been used a way to
* hide the variable
*
* @param	None
*
* @return	u32 TimerExpired Value
*
* @note		None.
*
******************************************************************************/
static u32 GetTimerExp(void)
{
        return TimerExpired;
}
/*****************************************************************************
* This function resets (=0) the TimerExpired Counter It's been used a way to
* hide the variable
*
* @param	None
*
* @return	none
*
* @note		None.
*
******************************************************************************/

void ResetTimerExp(void)
{
        TimerExpired =0;
}

/*****************************************************************************
* This function Increments the TimerExpired Counter It's been used a way to
* hide the variable
*
* @param	None
*
* @return	None
*
* @note		None.
*
******************************************************************************/
static void IncTimerExp(void)
{
        TimerExpired++;
}

/*****************************************************************************
* This function Increments the TimerExpired Counter It's been used a way to
* hide the variable
*
* @param	None
*
* @return	None
*
* @note		None.
*
******************************************************************************/
static void IncTickTimer(void)
{
        TickTimer++;
}
/*****************************************************************************
* This function retrieves the current Timer values (Timer counts stored in the
*  TimerValue.Value, while number of timer expiration stored in TimerValue.ExpNo
*
* @param	A pointer to the TimerValue struct to store the results there
*
* @return		None
*
* @note		None
*
******************************************************************************/

void GetTime(struct TimerValue *tv)
{
        tv->Value= XTmrCtr_GetValue(&TmrCtrInstance, TMR_CTR_NO);
        tv->ExpNo = GetTimerExp();

}

/*****************************************************************************
* This function calculates the elapsed time and store the elapsed time in Second
* thousands of seconds in the tt (TimerTime struct)
*
* @param	A pointer to the TimerValue struct that has been initialized by GetTime
* 			function.
* @param 	tt 		Return the stored time difference.
*
* @return		None
*
* @note		*tv must be initialized first by GetTime function
* 				TimerTime struct has an extra (min) member that has not been used.
*
******************************************************************************/
void GetElapsedTime(struct TimerValue *tv, struct TimerTime *tt)
{
        struct TimerValue tmpTV;
        float t;

        GetTime(&tmpTV );
        if (GetTimerExp() !=tmpTV.ExpNo ) {
                GetTime(&tmpTV );
        }
        tmpTV.ExpNo -= tv->ExpNo;
        tmpTV.Value -= tv->Value;
        t=conv_cycles_to_secs (tmpTV.Value) +  tmpTV.ExpNo * conv_cycles_to_secs(0xFFFFFFFF);
        tt->Sec=(u32) t;
        tt->ThSec = (u32) ((t-tt->Sec) * 1000);
}


/*****************************************************************************
* This function Reset the timer and the expiration counter
* (TMR_CTR_NO) determine which timer
*
* @param	None
*
* @return		None
*
* @note		None.
*
******************************************************************************/
void ResetTimer()
{
        // StopTimer();
        XTmrCtr_Reset(&TmrCtrInstance, TMR_CTR_NO);
        ResetTimerExp();

}

/*****************************************************************************
 * This function should disable the timer (CURRENTY ONLY STOPS IT )
 * (TMR_CTR_NO) determine which timer
 *
 * @param	None
 *
 * @return		None
 *
 * @note		None.
 * FIXME Compete this function
 ******************************************************************************/
void DisableTimer()
{
        /*
         * Disable the interrupt for the timer counter
         */
        //XIntc_Disable(IntcInstancePtr, TMRCTR_INTERRUPT_ID);
        XTmrCtr_Stop(&TmrCtrInstance, TMR_CTR_NO);

}
/*****************************************************************************
 * This function Starst the timer
 * (TMR_CTR_NO) determine which timer
 *
 * @param	None
 *
 * @return		None
 *
 * @note		None.
 *
 ******************************************************************************/
void TimerStart()
{
        XTmrCtr_Start (&TmrCtrInstance, TMR_CTR_NO);
}
/*****************************************************************************
 * This function Starst the timer
 * (TMR_CTR_NO) determine which timer
 *
 * @param	None
 *
 * @return		None
 *
 * @note		None.
 *
 ******************************************************************************/
void TickTimerStart()
{
        XTmrCtr_Start (&TmrCtrInstance, 1);
}
/*****************************************************************************
 * This function Stops the timer
 * (TMR_CTR_NO) determine which timer
 *
 * @param	None
 *
 * @return		None
 *
 * @note		None.
 *
 ******************************************************************************/
void TimerStop()
{
        XTmrCtr_Stop(&TmrCtrInstance, TMR_CTR_NO);
}
/*****************************************************************************
 * This function Stops the timer
 * (TMR_CTR_NO) determine which timer
 *
 * @param	None
 *
 * @return		None
 *
 * @note		None.
 *
 ******************************************************************************/
void TickTimerStop()
{
        XTmrCtr_Stop(&TmrCtrInstance, 1);
}

/******************************************************************************/
/**
*
* This function disables the interrupts for the Timer.
*
* @param	IntcInstancePtr is a reference to the Interrupt Controller
*		driver Instance.
* @param	IntrId is XPAR_<INTC_instance>_<Timer_instance>_VEC_ID
*		value from xparameters.h.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void TmrCtrDisableIntr(XIntc* IntcInstancePtr, u16 IntrId)
{
        /*
         * Disable the interrupt for the timer counter
         */
        XIntc_Disable(IntcInstancePtr, IntrId);

        return;
}
/*****************************************************************************
 * This function covert the number of cycles (timer cycles )
 * the number of seconds
 * (TMR_CTR_NO) determine which timer
 * Xilinx(R)
 *
 * @param	 u32 number of cycles
 *
 * @return		float number of seconds
 *
 * @note		None.
 *
 ******************************************************************************/

float conv_cycles_to_secs (u32 cycles)
{
#ifdef __MICROBLAZE__
        return ((float)cycles / XPAR_CPU_CORE_CLOCK_FREQ_HZ);
#else
        return ((float)cycles / XPAR_CPU_PPC440_CORE_CLOCK_FREQ_HZ);
#endif
}


 void TickTimerTriggered()
{

};
