
/**
  STM32 Microcontrollers support.
  ------------------------------------------------------
	Copyright (c) (2021), LOMSA electronics
  ------------------------------------------------------	
* @file:        RTM_config.h
* @brief:       RTM(Real time multithreading) configuration file.
* @dataIssue:   07.06.2021
* @dataModified -
* @author:      Giorgi Kurtanidze
* @version:     -
 **/
 
#ifndef _RTM_CONFIG_H
#define _RTM_CONFIG_H

//Dependencies.
#include "stdint.h"

//Define your max. available threads(each thread takes approx. 14 byte, from your RAM).
#define MAX_AVAILABLE_THREAD 5

//Define your platform port.
//#define PORT_STM32M3 1
//#define PORT_STM32M4 1
#define PORT_ARDUINO 1

//This is part of my framework, not thread timer part.
//RTC source definitions.
#define TIME_NO_RTC       0x0
#define TIME_RTC_SOFTWARE 0x1
#define TIME_RTC_HARDWARE 0x2
#define TIME_RTC TIME_NO_RTC

//Callback function pointer.
typedef void (*callback_t)(void);
typedef uint8_t threadCtx_t;

#endif

//End of file.

