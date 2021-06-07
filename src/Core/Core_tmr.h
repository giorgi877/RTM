
/**
  STM32 Microcontrollers support.
  ------------------------------------------------------
	Copyright (c) (2021), LOMSA electronics
  ------------------------------------------------------	
* @file:        Core_tmr.h
* @brief:       Core(hardware) timer init driver.
* @dataIssue:   07.06.2021
* @dataModified -
* @author:      Giorgi Kurtanidze
* @version:     1.0
 **/
 
#ifndef _CORE_TMR_H
#define _CORE_TMR_H

//Dependencies.
#include "Config/RTM_config.h"

//Custom functions.
void timer_init(callback_t callback);

#endif


