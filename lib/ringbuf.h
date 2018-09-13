/*************************************************************************\
 * File Name    : ringbuf.h                                              *
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

#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include "os/opt.h"
   
#define PTR_OFFSET( ptr, offset )  (void*)( (DWORD)(ptr) + (offset) )

//////////////////////////////////////////////////////////////////////////
struct ringbuf 
{
	void  *data;
	WORD   mask;
	WORD   put_ptr;
	WORD   get_ptr;
};

//////////////////////////////////////////////////////////////////////////
BYTE  ringbuf_init(struct ringbuf *r, void *buffer, WORD size );
WORD  ringbuf_put( struct ringbuf *r, void* buffer, WORD len );
WORD  ringbuf_get( struct ringbuf *r, void * buffer, WORD len  );
WORD  ringbuf_size(struct ringbuf *r);
WORD  ringbuf_elements(struct ringbuf *r);
WORD  ringbuf_free_elements( struct ringbuf *r );

#endif 
