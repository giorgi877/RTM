
/**
  STM32 Microcontrollers support.
  ------------------------------------------------------
	Copyright (c) (2021), LOMSA electronics
  ------------------------------------------------------	
* @file:        thread_tmr.c
* @brief:       Hardware timer based thread implementation.
* @dataIssue:   11.11.2017
* @dataModified -
* @author:      Giorgi Kurtanidze
* @version:     1.10
 **/

//Dependencies.
#include "stdint.h"
#include "Tools/thread_tmr.h"
#include "Core/Core_tmr.h"
//Arduino port defined?
#ifdef PORT_ARDUINO
 #include <Arduino.h>
#endif

//Static variables.
volatile static uint32_t tick = 0;
//Software RTC enabled?
#if (TIME_RTC == TIME_RTC_SOFTWARE)	
 volatile static uint32_t secondsCnt = 0;
#endif

//STM32 port defined?
#if defined PORT_STM32M3 || defined PORT_STM32M4
/**
 * @brief: timer tick interrupt.
 * @Return: None.
 **/
void tickCnt(void)
{
 ++tick;
 //Software RTC enabled?
 #if (TIME_RTC == TIME_RTC_SOFTWARE)	
  if(tick && !(tick % 1000U)) ++secondsCnt; 	
 #endif
}

//Software RTC enabled?
#if (TIME_RTC == TIME_RTC_SOFTWARE)	
/**
 * @brief: Set seconds counter value(used for software timer counter for RTC).
 * @Param: s[IN] - Seconds value.
 * @Return: None.
 **/
void threadTmr_set_secondsCnt(uint32_t s)
{
  secondsCnt = s;
}

/**
 * @brief: Get seconds counter value(used for software timer counter for RTC).
 * @Return: Seconds value.
 **/
uint32_t threadTmr_get_secondsCnt(void)
{
	return secondsCnt;
}
#endif
#endif

/**
 * @brief: Hardware timer init.
 * @Return: None.
 **/
void threadTmr_init(void)
{
	//STM32 port defined?
  #if defined PORT_STM32M3 || defined PORT_STM32M4
   //Init core timer.
   timer_init(tickCnt);	
	#endif
  //For arduino port, no timer config is necessary, timer is already initialized for millis().
}

/**
 * @brief: Start thread timer.
 * @Param: ctx[IN] - thread context.
 * @Return: None.
 **/
void threadTmr_start(threadTmr_context *ctx)
{
	//Arduino port defined?
  #if defined PORT_ARDUINO
	 //Update tick counter from arduino millis.
	 tick = millis();  
	#endif
	ctx->state = THREAD_TMR_START;	
	ctx->lastTick = tick;
}

/**
 * @brief: Stop thread timer.
 * @Param: ctx[IN] - thread context.
 * @Return: None.
 **/
void threadTmr_stop(threadTmr_context *ctx)
{
	ctx->state = THREAD_TMR_STOP;	
}

/**
 * @brief: Resume thread timer.
 * @Param: ctx[IN] - thread context.
 * @Return: None.
 **/
void threadTmr_resume(threadTmr_context *ctx)
{
	ctx->state = THREAD_TMR_START;	
}

/**
 * @brief: Get passed milliseconds value after timer start.
 * @Param: ctx[IN] - thread context.
 * @Return: milliseconds value.
 **/
uint32_t threadTmr_get(threadTmr_context *ctx)
{
 //Arduino port defined?
 #if defined PORT_ARDUINO
	//Update tick counter from arduino millis.
	tick = millis();  
 #endif
 return (tick - ctx->lastTick);	
}

/**
 * @brief: Check thread expiration period in continuous mode.
 * @Param: ctx[IN] - thread context.
 * @Param: timeMs[IN] - thread expiration value in milliseconds.
 * @Return: THREAD_TMR_DONE - if thread expired, THREAD_TMR_IDLE - thread is not running, THREAD_TMR_BUSY - thread busy.
 **/
threadState_t threadTmr_check_cont(threadTmr_context *ctx, uint32_t timeMs)
{
	uint32_t passedMs;
	//Thread is in stop state?     	
	if(ctx->state == THREAD_TMR_STOP) return THREAD_TMR_IDLE;
	
	//Get passed milliseconds.
	passedMs = threadTmr_get(ctx);

	//Thread time has been expired?
  if(passedMs >= timeMs)
  {
		return THREAD_TMR_DONE;
	}		
	//Thread busy state.
	return THREAD_TMR_BUSY;
}

/**
 * @brief: Check thread expiration period in one shot mode.
 * @Param: ctx[IN] - thread context.
 * @Param: timeMs[IN] - thread expiration value in milliseconds.
 * @Return: THREAD_TMR_DONE - if thread expired, THREAD_TMR_IDLE - thread is not running, THREAD_TMR_BUSY - thread busy.
 **/
threadState_t threadTmr_check(threadTmr_context *ctx, uint32_t timeMs)
{
	threadState_t state;
	
	state = threadTmr_check_cont(ctx, timeMs);
	//Thread period expired?
	if(state == THREAD_TMR_DONE) ctx->state = THREAD_TMR_STOP;
	
	return state;
}

/**
 * @brief: delay function(block mode).
 * @Param: ctx[IN] - thread context.
 * @Param: timeMs[IN] - thread expiration value in milliseconds.
 * @Return: None.
 **/
void threadTmr_delay(threadTmr_context *ctx, uint32_t timeMs)
{
	//Start thread.
	threadTmr_start(ctx);
	//Wait while...
	while(threadTmr_get(ctx) < timeMs);
}

/**
 * @brief: Check whether thread is running or not.
 * @Param: ctx[IN] - thread context.
 * @Return: thread running state(1 - is running, 0 - in idle state).
 **/
uint8_t threadTmr_is_running(threadTmr_context *ctx)
{
	if(ctx->state == THREAD_TMR_START) return 1;
	                                   return 0;
}

//End of file.

