/*
 * timer.h
 *
 *  Created on: Jun 29, 2011
 *      Author: Ahmed Al-Wattar
 */

#ifndef TIMER_H_
#define TIMER_H_

struct TimerTime {
        u32 Min;
        u32 Sec;
        u32 ThSec;	/* thousands of a second*/
};
struct TimerValue {
        u32 ExpNo;	/* nuber of times Timer expired */
        u32 Value;	/* Value of Counter */
};
//int InitTimer();


int InitTimer(XIntc *intcInstPtr);
int TimerRegInt(XIntc *IntcInstancePtr);
void TimerCounterHandler(void *CallBackRef, u8 TmrCtrNumber);
float conv_cycles_to_secs (u32 cycles);

void ResetTimer();
void GetTime(struct TimerValue *tv);
void GetElapsedTime(struct TimerValue *tv, struct TimerTime *tt);
void AddTime(struct TimerTime *tv, struct TimerTime *tt);
void DisableTimer();
u32 GetTickTimer();
void TimerStart();
void TimerStop();
void TickTimerStart();
void TickTimerStop();

#endif /* TIMER_H_ */
