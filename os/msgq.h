/*************************************************************************\
 * File Name    : msgq.h                                                 *
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
 * 04-09-2018     charlie_weng     V1.6          add msg queue           *
 *                                                                       *
\*************************************************************************/

#ifndef __MSGQ_H__
#define __MSGQ_H__

#include "os/uXos.h"
#include "lib/ringbuf.h"


//////////////////////////////////////////////////////////////////////////  
struct MSG_Q
{
    BYTE   count;
	BYTE   msg_size;
	struct ringbuf msgQ;
};

///////////////////////////////////////////////////////////////////////////
void  msg_q_init( struct MSG_Q  *m, BYTE* msgQ, BYTE msg_size, BYTE q_size );
BYTE  msg_q_post( struct MSG_Q  *m, BYTE* msg );
///////////////////////////////////////////////////////////////////////////
#define uXOS_MSGQ_INIT( m, msgQ, msg_size, q_size )  msg_q_init( m, msgQ, msg_size, q_size )

// void  uXOS_MSGQ_WAIT( struct MSG_Q  *m, BYTE* msg );
#define uXOS_MSGQ_WAIT( m, msg )                                 \
  do {                                                           \
    uPT_WAIT_UNTIL(pt, (m)->count > 0);                          \
	ringbuf_get( &((m)-> msgQ), msg, (m)->msg_size );            \
    --(m)->count;                                                \
  } while(0)
	  
//BYTE  uXOS_MSGQ_POST( struct MSG_Q  *m, BYTE* msg );
#define uXOS_MSGQ_POST( m, msg )   msg_q_post( m, msg )

///////////////////////////////////////////////////////////////////////////

#endif
