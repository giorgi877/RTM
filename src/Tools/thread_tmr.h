
/**
  STM32 Microcontrollers support.
  ------------------------------------------------------
	Copyright (c) (2021), LOMSA electronics
  ------------------------------------------------------	
* @file:        thread_tmr.h
* @brief:       Hardware timer based thread implementation.
* @dataIssue:   11.11.2017
* @dataModified -
* @author:      Giorgi Kurtanidze
* @version:     1.10
 **/

#ifndef _THREAD_TMR_H
#define _THREAD_TMR_H

//Dependencies.
#include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif

//States.
typedef enum
{ 
 THREAD_TMR_STOP,
 THREAD_TMR_START,
 THREAD_TMR_IDLE,
 THREAD_TMR_BUSY,
 THREAD_TMR_DONE	
}threadState_t;
	 	 
typedef struct
{
	uint32_t lastTick; //Last tick value.
	threadState_t  state;//Thread state. 
}threadTmr_context;

//Custum functions.
void           threadTmr_init(void);
void           threadTmr_start(threadTmr_context *ctx);
void           threadTmr_stop(threadTmr_context *ctx);
void           threadTmr_resume(threadTmr_context *ctx);
uint32_t       threadTmr_get(threadTmr_context *ctx);
threadState_t  threadTmr_check_cont(threadTmr_context *ctx, uint32_t timeMs);
threadState_t  threadTmr_check(threadTmr_context *ctx, uint32_t timeMs);
void           threadTmr_delay(threadTmr_context *ctx, uint32_t timeMs);
uint32_t       threadTmr_get_secondsCnt(void);
void           threadTmr_set_secondsCnt(uint32_t s);
uint8_t        threadTmr_is_running(threadTmr_context *ctx);

#ifdef __cplusplus
}
#endif

#endif

//End of file.

