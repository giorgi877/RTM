
/**
  STM32 Microcontrollers support.
  ------------------------------------------------------
	Copyright (c) (2021), LOMSA electronics
  ------------------------------------------------------	
* @file:        RTM.c
* @brief:       RTM(Real time multithreading) ultra lightweight driver, for STM32 and Arduino platforms
* @dataIssue:   07.06.2021
* @dataModified -
* @author:      Giorgi Kurtanidze
* @version:     1.0
 **/
 
//Dependencies.
#include "RTM.h"
#include <string.h>

//Declare task contexts.
thread_context thread[MAX_AVAILABLE_THREAD];

//Static variables.
static threadCtx_t threadCtx = 0;

//Thread states.
enum
{
	THREAD_IDLE,
	THREAD_RUN
};

/**
 * @brief: RTM init contexts and core timer(for STM32).
 * @Return: None.
 **/
void RTM_thread_init(void)
{
	threadCtx_t ctx;
	//Set all context init state.
	for(ctx = 0; ctx < MAX_AVAILABLE_THREAD; ctx++)
	{
		memset(&thread[ctx], 0, sizeof(thread_context));
	}
	//Init core(hardware) timer.
	threadTmr_init();	
}

/**
 * @brief: RTM create new thread.
 * @Param: threadCtxP[IN] - Thread context index pointer(set NULL, if not used).
 * @Param: threadPeriod[IN] - Thread execution period.
 * @Param: taskCallback[IN] - Thread callback function.
 * @Return: 1 - context overcount, 0 - no error.
 **/
uint8_t RTM_thread_create(uint8_t *threadCtxP, uint32_t threadPeriod, callback_t taskCallback)
{
	//Thread context overflow?
	if(threadCtx >= MAX_AVAILABLE_THREAD) return 1;

	//Context init.
	if(threadCtxP != NULL) *threadCtxP = threadCtx;
	thread[threadCtx].state = THREAD_RUN;
	thread[threadCtx].threadPeriod = threadPeriod;
	thread[threadCtx].threadCallback = taskCallback;
	
	//Run the thread timer.
	threadTmr_start(&thread[threadCtx].threadTmr);	

	//Next context.
	threadCtx++;
	return 0;
}

/**
 * @brief: RTM suspend thread execution.
 * @Param: currentThreadCtx[IN] - Current thread context index.
 * @Return: None.
 **/
void RTM_thread_suspend(threadCtx_t currentThreadCtx)
{
	thread[currentThreadCtx].state = THREAD_IDLE;
}

/**
 * @brief: RTM resume thread execution.
 * @Param: currentThreadCtx[IN] - Current thread context index.
 * @Return: None.
 **/
void RTM_thread_resume(threadCtx_t currentThreadCtx)
{
	thread[currentThreadCtx].state = THREAD_RUN;
}

/**
 * @brief: RTM check thread timer and try to run thread.
 * @Param: currentThreadCtx[IN] - Current thread context index.
 * @Return: 1 - context overcount, 0 - no error.
 **/
uint8_t RTM_thread_run(threadCtx_t currentThreadCtx)
{
	//Thread context overflow?
	if(currentThreadCtx >= MAX_AVAILABLE_THREAD) return 1;
	
	//Thread period expired?
	if(threadTmr_check(&thread[currentThreadCtx].threadTmr, thread[currentThreadCtx].threadPeriod) == THREAD_TMR_DONE)
	{ 	
		//Run thread callback.
    thread[currentThreadCtx].threadCallback(); 
		//Start thread timer again.
	  threadTmr_start(&thread[currentThreadCtx].threadTmr);	
	}
	return 0;
}

/**
 * @brief: RTM thread handler(run multiple threads).
 * @Return: None.
 **/
void RTM_thread_handler(void)
{
	threadCtx_t ctx = 0;
	
	//Thread handler loop.
	//Try to run all initialized threads.
	do
	{
		//Thread is in run state?
		if(thread[ctx].state == THREAD_RUN)
		{
			//Try to run current thread.
			RTM_thread_run(ctx);	
		}
	}while(++ctx < MAX_AVAILABLE_THREAD);
}

//End of file.

