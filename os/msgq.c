/*************************************************************************\
 * File Name    : msgq.c                                                 *
 * --------------------------------------------------------------------- *
 * Title        : uXOS --- Cooperative system                            *
 * Revision     : V1.6                                                   *
 * Notes        :                                                        *
 * --------------------------------------------------------------------- *
 * Revision History:                                                     *
 *   When             Who         Revision       Description of change   *
 * -----------    -----------    ---------      ------------------------ *
 * 12-11-2010     charlie_weng     V1.0          Created the program     *
 * 28-02-2013     charlie_weng     V1.3          add etimer,stimer       *
 * 04-09-2018     charlie_weng     V1.6          add msg queue           * 
 *                                                                       *
\*************************************************************************/

#include "uxos.h"
#include "msgq.h"

/*************************************************************************\
 *                                                                       *
 *   Function name   : msg_q_init                                        *
 *   Returns         : TICK                                              *
 *   Parameters      : NONE                                              *
 *   Purpose         : get system Tick                                   *
 *                                                                       *
\*************************************************************************/
void msg_q_init( struct MSG_Q  *m, BYTE* msgQ, BYTE msg_size, BYTE q_size )
{
	m->count    = 0;
	m->msg_size = msg_size;
    ringbuf_init( &(m->msgQ), msgQ, q_size*(m->msg_size) );
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : msg_q_post                                        *
 *   Returns         : 1 -- succ 0 -- fail                               *
 *   Parameters      : NONE                                              *
 *   Purpose         : get system Tick                                   *
 *                                                                       *
\*************************************************************************/
BYTE msg_q_post( struct MSG_Q  *m, BYTE* msg )   
{
	if( m->msg_size > ringbuf_free_elements(&( m-> msgQ)) )
		return 0;
	
	ringbuf_put( &((m)-> msgQ), msg, m->msg_size );
	++(m)->count; 
	
	return 1;
}

