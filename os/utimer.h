/*************************************************************************\
 * File Name    : uXOS.h                                                 *
 * --------------------------------------------------------------------- *
 * Title        : uXOS --- Cooperative system                            *
 * Revision     : V1.0                                                   *
 * Notes        :                                                        *
 * --------------------------------------------------------------------- *
 * Revision History:                                                     *
 *   When             Who         Revision       Description of change   *
 * -----------    -----------    ---------      ------------------------ *
 * 12-11-2010     charlie_weng     V1.0          Created the program     *
 * 28-02-2013     charlie_weng     V1.3          add etimer,stimer       *
 *                                                                       *
\*************************************************************************/

#ifndef __UTIMER_H__
#define __UTIMER_H__

#include "config.h"
#include "os/opt.h"

///////////////////////////////////////////////////////////////////////////
#define  UTIMER_EN         				0x80
#define	 UTIMER_ONESHOT   	 			0x00      // 0
#define	 UTIMER_PERIODIC   				0x01      // 1
	

#define  TIMER_CTRL_GET_TIME        	0
#define  TIMER_CTRL_SET_TIME        	1 
#define  TIMER_CTRL_SET_ONESHOT     	2
#define  TIMER_CTRL_SET_PERIODIC    	3

typedef  void (*cb)(void);
////////////////////////////////////////////////////////////////////////////
struct uTimer
{
	void  *next;
	BYTE  cfg;
	TICK  interval;
	TICK  curr_tick;
    cb    callback;
};
///////////////////////////////////////////////////////////////////////////
void   		   utimer_init( void );
void   		   utimer_stop( struct uTimer *timer );
void   		   utimer_start( struct uTimer *timer );
void   		   utimer_delete( struct uTimer *timer );
struct uTimer* utimer_create( void (*cb)(void),TICK timeout, BYTE period );
void   		   utimer_ctrl( struct uTimer *timer,BYTE cmd, void* arg );
struct uTimer* utimer_callback( void (*cb)(void), TICK timeout, BYTE option );
///////////////////////////////////////////////////////////////////////////

#endif
