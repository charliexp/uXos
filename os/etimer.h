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

#ifndef __ETIMER_H__
#define __ETIMER_H__

#include "osconfig.h"
#include "os/opt.h"

///////////////////////////////////////////////////////////////////////////
typedef struct 
{
	TICK  curr_tick;
	TICK  interval;
}TM;

///////////////////////////////////////////////////////////////////////////
void  etimer_set( TM *tm, TICK timeout );
BYTE  etimer_expired( TM *tm );
///////////////////////////////////////////////////////////////////////////

#endif
