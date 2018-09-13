/*************************************************************************\
 * File Name        : conf.h                                             *
 * --------------------------------------------------------------------- *
 * Title        : uXOS Cooperative system hardware config file           *
 * Revision        : V1.0                                                *
 * Notes        :                                                        *
 *                                                                       * 
 * --------------------------------------------------------------------- *
 * Revision History:                                                     *
 *   When           Who          Revision        Description of change   *
 * -----------   -----------     ---------      ------------------------ *
 * 19-08-2011     charlie        V1.0            Created the program     *
 *                                                                       * 
\*************************************************************************/

#ifndef __CONF_H__
#define __CONF_H__

#include "stm32f10x.h"

///////////////////////////////////////////////////////////////////////////
#define MAX_TASK_NUM         6
#define TIMER_NUMBER         6

///////////////////////////////////////////////////////////////////////////
#define CODE   				 const  //code
#define DATA          		 //data

///////////////////////////////////////////////////////////////////////////
#define  ENABLE_ISR()        __enable_irq()       
#define  DISABLE_ISR()       __disable_irq()       
#define  CLEAR_SYS_WDT()     IWDG_ReloadCounter(); 
//#define  NOP()             __wait_nop()

///////////////////////////////////////////////////////////////////////////
#endif /* __CONF_H */
