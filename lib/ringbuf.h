#ifndef   __RINGBUF_H__
#define   __RINGBUF_H__

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


