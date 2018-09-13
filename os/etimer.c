/*************************************************************************\
 * File Name    : uXOS.c                                                 *
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
 * 04-09-2018     charlie_weng     V1.6          reconsitution           *  
 *                                                                       *
\*************************************************************************/

#include "uxos.h"
#include "etimer.h"

/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_set                                        *
 *   Returns         : TICK                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         : get system Tick                                   *
 *                                                                       *
\*************************************************************************/
void etimer_set(TM *tm, TICK timeout )
{
    tm->curr_tick = get_tick();
    tm->interval  = timeout;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_expired                                    *
 *   Returns         : TICK                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         : get system Tick                                   *
 *                                                                       *
\*************************************************************************/
BYTE etimer_expired( TM *tm )
{
    if( (TICK)( get_tick() - tm->curr_tick ) >= tm->interval )
		return 1;
    else
        return 0;	
}


