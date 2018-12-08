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

#include "uXOS.h"
#include "lib/list.h"
#include "lib/memb.h"
#include "utimer.h"

//////////////////////////////////////////////////////////////////////////
LIST( timerlist );
MEMB( utimerblks, struct uTimer, TIMER_NUMBER );

/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_insert                                     *
 *   Returns         : NONE                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
static void utimer_insert( struct uTimer **timer )
{
	struct uTimer *t,*l,*p = NULL,*r = NULL;
	
    t = *timer;	
	
	if( list_head( timerlist ) == NULL )
	{
		list_push( timerlist, t );	
		return;
	}
	for( l = (struct uTimer*)list_head(timerlist); l != NULL; l = (struct uTimer*)l->next )
	{
		if( (STICK)(t->curr_tick - l->curr_tick) <= 0 )
		{
			list_insert( timerlist, r, t );
			return;
		}
		else
		{
			r = l;
		}
		if( l->next == NULL ) p = l;
	}
	t->next = NULL;
	p->next = t;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_create                                     *
 *   Returns         : NONE                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
struct uTimer* utimer_create( void (*cb)(void), TICK timeout, BYTE option )
{
	struct uTimer *t, *r;

	r = NULL;
	for( t = list_head(timerlist); t != NULL; t = t->next )
	{
		if( t->callback == cb )
		{
			if( r == NULL )
				*timerlist = t->next;   
			else
				r->next = t->next; 
			t->next = NULL;
			t->cfg &= (BYTE)~UTIMER_EN;
			return t;
		}
		r = t;
	}
    
	t = (struct uTimer*)memb_alloc( &utimerblks );

	t->interval = timeout;
    t->cfg      = option;
	t->callback = cb;
	
    return t;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_delete                                     *
 *   Returns         : NONE                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void utimer_delete(struct uTimer *timer)
{
	list_remove( timerlist, timer );
	memb_free( &utimerblks,timer );
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_start                                      *
 *   Returns         : NONE                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void utimer_start(struct uTimer *timer)
{
	timer->cfg      |= UTIMER_EN;
    timer->curr_tick = get_tick() + timer->interval;
	utimer_insert(&timer);
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_stop                                       *
 *   Returns         : NONE                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void utimer_stop(struct uTimer *timer)
{
	timer->cfg &= (BYTE)~UTIMER_EN;
	list_remove(timerlist, timer);
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_ctrl                                       *
 *   Returns         : NONE                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void utimer_ctrl(struct uTimer *timer, BYTE cmd, void* arg)
{
    switch (cmd)
    {
		case TIMER_CTRL_GET_TIME:

			*(TICK *)arg = timer->interval;
			break;

		case TIMER_CTRL_SET_TIME:

			utimer_stop(timer);
			timer->interval = *(TICK *)arg;
			utimer_start(timer);
			break;

		case TIMER_CTRL_SET_ONESHOT:

			timer->cfg &= ~UTIMER_PERIODIC;
			break;

		case TIMER_CTRL_SET_PERIODIC:

			utimer_stop(timer);
			timer->cfg |= UTIMER_PERIODIC;
			utimer_start(timer);
			break;
	}
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_tick                                       *
 *   Returns         : NONE                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
static void utimer_tick( void )
{
	struct uTimer  *l;
	
	l = (struct uTimer*)list_head(timerlist);
	
	while( l != NULL )
	{
		if( (STICK)(get_tick() - (l->curr_tick)) < 0 )
		{
			return;
		}
		else
		{			
			if( l->callback )
				l->callback();
			
			if( l->cfg & UTIMER_PERIODIC )
			{			
				l = list_pop( timerlist );
				l->curr_tick = get_tick()+l->interval;
				utimer_insert( &l );
                l = list_head( timerlist );				
			}
			else
			{
				l = list_pop( timerlist );
				memb_free( &utimerblks, l );
                l = list_head( timerlist );	
			}
		}	
	}
}	

/*************************************************************************\
 *                                                                       *
 *   Function name   : utimer_init                                       *
 *   Returns         : NONE                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void utimer_init(void)
{
	list_init( timerlist );
	memb_init( &utimerblks );
	os_set_tmr_cb( utimer_tick );
}

/*************************************************************************\
 *                                                                       *   
 *   Function name   : utimer_callback                                   *
 *   Returns         : uTimer*                                           *
 *   Parameters      : callback,timeout,option                           *
 *   Purpose         :                                                   *
 *                                                                       * 
\*************************************************************************/
struct uTimer* utimer_callback(void (*cb)(void), TICK timeout, BYTE option )
{    
    struct uTimer* t;
    
    t = utimer_create( cb, timeout, option );
    utimer_start(t); 
    return t;
}   

///////////////////////////////////////////////////////////////////////////

