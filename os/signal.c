/*************************************************************************\
 * File Name    : signal.c                                               *
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

#include <stdio.h>
#include <stdarg.h>
#include "signal.h"
#include "lib/memb.h"

//////////////////////////////////////////////////////////////////////////
static SIGNAL g_sig = USER_SIGNAL_BASE;
static slot_t g_hash_table[ WHEEL_MAX ];
MEMB( slotblks, struct slot_tcb, MAX_SLOT_TCB );

/*************************************************************************\
 *                                                                       *
 *   Function name   : emit                                              *
 *   Returns         : void                                              *
 *   Parameters      : sig arg                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void signal_slot_init(void)
{
    int i;

    for( i = 0; i < WHEEL_MAX; i++ )
	{
        g_hash_table[i].next = NULL;
        g_hash_table[i].sig  = 0;
    }
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : emit                                              *
 *   Returns         : void                                              *
 *   Parameters      : sig arg                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
static void hash_list_insert( slot_t *phead, slot_t *pitem )
{
    slot_t *slp;

    slp         = phead->next;
    phead->next = pitem;
    pitem->next = slp;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : emit                                              *
 *   Returns         : void                                              *
 *   Parameters      : sig arg                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
static void hash_list_remove( slot_t *pprev, slot_t *ptar )
{
    pprev->next = ptar->next;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : emit                                              *
 *   Returns         : void                                              *
 *   Parameters      : sig arg                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
SIGNAL create_signal( void )
{
	g_sig++;
    return g_sig;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : emit                                              *
 *   Returns         : void                                              *
 *   Parameters      : sig arg                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
static int hash_get( SIGNAL sig )
{
    return ( sig%WHEEL_MAX );
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : emit                                              *
 *   Returns         : void                                              *
 *   Parameters      : sig arg                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
int connect( SIGNAL sig, void *slot )
{
    slot_t *t;
    int     key;

    if( sig == 0 ) 
		return 0;
	if( slot == NULL ) 
		return 0;
	
	t = (slot_t*)memb_alloc( &slotblks );
	
    if( t == NULL )
	{
        return 0;
    }
    key = hash_get( sig );
	
	t->sig  = sig;
    t->slot = SLOT_FUNC(slot);
    t->next = NULL;
	
    hash_list_insert( &g_hash_table[key], t );
	
    return 1;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : emit                                              *
 *   Returns         : void                                              *
 *   Parameters      : sig arg                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
int disconnect( SIGNAL sig )
{
    int     key;
    slot_t *slp = NULL, *pprev;

    if( sig == 0 )
	{
        return 0;
    }
    key = hash_get( sig );
	
    if( g_hash_table[key].next == NULL )
	{
        return 0;
    }
    pprev = &g_hash_table[key];
    slp   =  g_hash_table[key].next;
	
    while(1)
	{
        if( slp == NULL )
		{
            break;
        }
        if( sig == slp->sig )
		{
            hash_list_remove( pprev, slp );
			memb_free( &slotblks, slp );
            slp = pprev->next;
        }
		else
		{
            slp   = slp->next;
            pprev = pprev->next;
        }
    }
    return 1;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : emit                                              *
 *   Returns         : void                                              *
 *   Parameters      : sig arg                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
static void slot_process( int arg_num, int *al, slot_t *cb )
{
    switch( arg_num )
	{
        case 0:
        case 1:
            (*(cb->slot))(*al);
            break;
        case 2:
            (*(cb->slot))(*al, *(al+1));
            break;
        case 3:
            (*(cb->slot))(*al, *(al+1), *(al+2));
            break;
        case 4:
            (*(cb->slot))(*al, *(al+1), *(al+2), *(al+3));
            break;
        case 5:
            (*(cb->slot))(*al, *(al+1), *(al+2), *(al+3), *(al+4));
            break;
        case 6:
            (*(cb->slot))(*al, *(al+1), *(al+2), *(al+3), *(al+4), *(al+5));
            break;
        case 7:
            (*(cb->slot))(*al, *(al+1), *(al+2), *(al+3), *(al+4), *(al+5), *(al+6));
            break;
        case 8:
            (*(cb->slot))(*al, *(al+1), *(al+2), *(al+3), *(al+4), *(al+5), *(al+6), *(al+7));
            break;
        case 9:
            (*(cb->slot))(*al, *(al+1), *(al+2), *(al+3), *(al+4), *(al+5), *(al+6), *(al+7), *(al+8));
            break;
        default:
            break;
    }

}

/*************************************************************************\
 *                                                                       *
 *   Function name   : emit                                              *
 *   Returns         : void                                              *
 *   Parameters      : sig arg                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void emit( SIGNAL sig, int arg_num, ... )
{
    int     key;
    int     count = arg_num; 
    int    *lp    = NULL;
	slot_t *slp;
	int     argTab[ arg_num+1 ];
   
    va_list ap;
	
	if( arg_num == 0 )
        return;
	
    va_start( ap, arg_num );

    lp = (int*)argTab;
	
    while( count-- != 0 )
	{
        *lp = va_arg( ap, int );
         lp++;
    }

    key = hash_get( sig );
	
    slp = g_hash_table[key].next;
	
    while( 1 )
	{
        if( slp == NULL ) 
			break;
		
        if( sig == slp->sig )
		{
            slot_process( arg_num, argTab, slp );
        }
        slp = slp->next;
    }
}
