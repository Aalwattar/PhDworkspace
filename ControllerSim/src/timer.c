/*

 * timer.c
 *
 *  Created on: Jun 29, 2011
 *      Author: ahmed
 */
#include "headers.h"

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
