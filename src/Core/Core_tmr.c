
/**
  STM32 Microcontrollers support.
  ------------------------------------------------------
	Copyright (c) (2021), LOMSA electronics
  ------------------------------------------------------	
* @file:        Core_tmr.c
* @brief:       Core(hardware) timer init driver.
* @dataIssue:   07.06.2021
* @dataModified -
* @author:      Giorgi Kurtanidze
* @version:     1.0
 **/

//Dependencies.
#include "stdint.h"
#include "Config/RTM_config.h"
#include "Core_tmr.h"

#if defined PORT_STM32M3
 #include "stm32f10x_tim.h"
#elif defined PORT_STM32M4
 #include "stm32f4xx_tim.h"
#elif defined PORT_ARDUINO
 //Place your arduino dependencies.
#else
 #error No port defined!
#endif

//STM32 port defined?
#if defined PORT_STM32M3 || defined PORT_STM32M4
 //Interrupt callback pointer.
 callback_t intCallback;

/**
 * @brief: Timer 3 interrupt handler, we are using just for compare interrupt form 1 channel.
 * @Return: None.
 **/
void TIM3_IRQHandler(void)
{	
  TIM3->SR &= ~TIM_IT_CC1;
	intCallback();
}

/**
 * @brief: Timer 3 init.
 * @Param: callback[IN] - Compare interrupt callback function pointer.
 * @Return: None.
 **/
void timer_init(callback_t callback)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	  //Pass callback address.
	  intCallback = callback;
	
	  TIM_Cmd(TIM3, DISABLE); 
  	TIM_SetCounter(TIM3,0);
    
	  //Interrupt vector config.
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 			
    //Enable timer clocking.	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
	  //Time base configuration 
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 1000; //Compare interrupt every 1 miliisecond.
    TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 1000000 - 1; // set tick at 1 microseconds.
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
	  //Enable interrupt on compare(channel 1).
		TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE); 
	
	  //Start counter
    TIM_Cmd(TIM3, ENABLE);
}
#endif
//End of file.

