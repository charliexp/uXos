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
 * 24-6-2015     charlie_weng      V1.0          modify this file        *
 *                                                                       *
\*************************************************************************/
#ifndef   __RINGBUF_H__
#define   __RINGBUF_H__

#include "osconfig.h"
#include "os/opt.h"

//////////////////////////////////////////////////////////////////////////
struct ringbuf 
{
	BYTE  *data;
	WORD   mask;
	WORD   put_ptr;
	WORD   get_ptr;
};

//////////////////////////////////////////////////////////////////////////
BYTE  ringbuf_init(struct ringbuf *r, BYTE *buffer, WORD size );
WORD  ringbuf_put( struct ringbuf *r, BYTE* buffer, WORD len );
WORD  ringbuf_get( struct ringbuf *r, BYTE *buffer, WORD len  );
WORD  ringbuf_size(struct ringbuf *r);
WORD  ringbuf_elements(struct ringbuf *r);
BYTE  ringbuf_empty( struct ringbuf *r );
WORD  ringbuf_free_elements( struct ringbuf *r );

#endif


