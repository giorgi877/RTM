
/**
  STM32 Microcontrollers support.
  ------------------------------------------------------
	Copyright (c) (2021), LOMSA electronics
  ------------------------------------------------------	
* @file:        RTM.h
* @brief:       RTM(Real time multithreading) ultra lightweight driver, for STM32 and Arduino platforms
* @dataIssue:   07.06.2021
* @dataModified -
* @author:      Giorgi Kurtanidze
* @version:     1.0
 **/
 
#ifndef _RTM_H
#define _RTM_H

//Dependencies.
#include "stdint.h"
#include "Config/RTM_config.h"
#include "Tools/thread_tmr.h"

#ifdef __cplusplus
 extern "C" {
#endif

/*Thread context struct*/
typedef struct
{
	uint8_t state;
	uint32_t threadPeriod;	
	callback_t threadCallback;
	threadTmr_context threadTmr;
}thread_context;

//Custom functions.
void    RTM_thread_init(void);
uint8_t RTM_thread_create(uint8_t *threadCtxP, uint32_t threadPeriod, callback_t taskCallback);
void    RTM_thread_suspend(threadCtx_t currentThreadCtx);
void    RTM_thread_resume(threadCtx_t currentThreadCtx);
void    RTM_thread_handler(void);

#ifdef __cplusplus
}
#endif

#endif

//End of file.

