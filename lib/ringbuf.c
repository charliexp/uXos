/*************************************************************************\
 * File Name    : ringbuf.c                                              *
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

#include <string.h>
#include "osconfig.h"
#include "ringbuf.h"

/*************************************************************************\
 *                                                                       *
 *   Function name   : QueueInit                                         *
 *   Returns         : void                                              *
 *   Parameters      : *pQueue,*pBuffer,size                             *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
BYTE ringbuf_init(struct ringbuf *r, BYTE *buffer, WORD size )
{
	r->get_ptr  = 0;	
	r->put_ptr  = 0;
    r->data     = buffer;
    r->mask     = size;
	
	return 1;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : QueueAddData                                      *
 *   Returns         : void                                              *
 *   Parameters      : callback function                                 *
 *   Purpose         : set callback                                      *
 *                                                                       *
\*************************************************************************/
WORD  ringbuf_put( struct ringbuf *r, BYTE* buffer, WORD len )	 
{
	WORD i = 0;
	
	WORD l = r->mask - ringbuf_elements(r);
	
	if( l > len ) l = len;
	
	for( i = 0; i < l; i++ )
	{	
        DISABLE_ISR();		
		r->data[r->put_ptr] = buffer[i];
		r->put_ptr = (r->put_ptr+1)%r->mask;
        ENABLE_ISR();        
	}
	
	return l;	
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : QueueGetData                                      *
 *   Returns         : void                                              *
 *   Parameters      : callback function                                 *
 *   Purpose         : set callback                                      *
 *                                                                       *
\*************************************************************************/
WORD  ringbuf_get( struct ringbuf *r, BYTE * buffer, WORD len  )
{
	WORD i = 0;
	
	WORD l = ringbuf_elements(r);
    
	if( l > len ) l = len; 

	for( i = 0; i < l; i++ )
	{
        DISABLE_ISR();
		*(buffer+i) = r->data[r->get_ptr];		
		r->get_ptr  = (r->get_ptr+1)%r->mask;
        ENABLE_ISR();
	}  
	return l;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : QueueCheckEmpty                                   *
 *   Returns         : void                                              *
 *   Parameters      : callback function                                 *
 *   Purpose         : set callback                                      *
 *                                                                       *
\*************************************************************************/
BYTE  ringbuf_empty( struct ringbuf *r )
{
    BYTE ret;
    
    DISABLE_ISR();
    ret = (r->get_ptr == r->put_ptr) ? 1:0;
    ENABLE_ISR();
 
	return ret;
}


/*************************************************************************\
 *                                                                       *
 *   Function name   : QueueCheckEmpty                                   *
 *   Returns         : void                                              *
 *   Parameters      : callback function                                 *
 *   Purpose         : set callback                                      *
 *                                                                       *
\*************************************************************************/
WORD  ringbuf_size(struct ringbuf *r)
{
	return r->mask;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : getQueueLength                                    *
 *   Returns         : void                                              *
 *   Parameters      : callback function                                 *
 *   Purpose         : set callback                                      *
 *                                                                       *
\*************************************************************************/
WORD  ringbuf_elements(struct ringbuf *r)
{
    WORD  end; 
    WORD  l;
    
    DISABLE_ISR();
    if( r->put_ptr < r->get_ptr )
        end = r->put_ptr + r->mask;
    else
        end = r->put_ptr;
    l = (end - r->get_ptr);
    ENABLE_ISR();
    
    return l;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : getQueueLength                                    *
 *   Returns         : void                                              *
 *   Parameters      : callback function                                 *
 *   Purpose         : set callback                                      *
 *                                                                       *
\*************************************************************************/
WORD  ringbuf_free_elements( struct ringbuf *r )
{
	return r->mask - ringbuf_elements(r);
}

///////////////////////////////////////////////////////////////////////////
